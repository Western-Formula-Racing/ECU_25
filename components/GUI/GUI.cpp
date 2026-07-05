#include "GUI.h"
static const char *TAG = "GUI"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

GUI *GUI::instancePtr = nullptr;
SemaphoreHandle_t GUI::mutex = xSemaphoreCreateMutex();

u_int8_t* GUI::sendable_packet_buffer = nullptr; // Adjust size as needed (Default: 10 sendables with 65 bytes each)
GUI::SendableEntry GUI::sendables[MAX_SENDABLES] = {};
size_t GUI::sendable_count = 0;
GUI::RecievableEntry GUI::recievables[MAX_RECIEVABLES] = {};
size_t GUI::recievable_count = 0;

char* GUI::recievables_json_buffer = nullptr;
size_t GUI::json_buffer_size = 3;

GUI::GUI()
{
    size_t bytes_total, bytes_free;

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = "spiffs",
        .max_files = 2,
        .format_if_mount_failed = true,
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    // Check if vfs is registered
    if (ret != ESP_OK) {
        ret = esp_spiffs_info(conf.partition_label, &bytes_total, &bytes_free);

        ESP_LOGI(TAG, "SPIFFS partition registered. Total: %d, Free: %d", bytes_total, bytes_free);

        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to get spiffs partition info (%s)", esp_err_to_name(ret));
        } else {
            ESP_LOGI(TAG, "spiffs partition info - Total: %d, Free: %d", bytes_total, bytes_free);
        }

        ESP_LOGI(TAG, "spiffs partition registered. Total: %d, Free: %d", bytes_total, bytes_free);
    } else 
    {
        ESP_LOGE(TAG, "Failed to register spiffs partition`` (%s)", esp_err_to_name(ret));
    }

    ESP_ERROR_CHECK(start_soft_ap());
    ESP_ERROR_CHECK(start_webserver());
    ESP_LOGI(TAG, "GUI Initialized");
}

esp_err_t GUI::handle_root(httpd_req_t *req)
{
    FILE* f_html = fopen("/spiffs/index.html.gz", "r");

    if (f_html == NULL)
    {
        ESP_LOGE(TAG, "Could not find html file in spiffs");
        return ESP_FAIL;
    }

    char* buffer = new char[HTML_SIZE];
    memset(buffer, 0, HTML_SIZE);
    fread(buffer, 1, HTML_SIZE, f_html);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    httpd_resp_send(req, buffer, HTML_SIZE);
    fclose(f_html);
    delete[] buffer;
    return ESP_OK;
}

esp_err_t GUI::handle_js(httpd_req_t *req)
{
    FILE* f_js = fopen("/spiffs/script.js.gz", "r");

    if (f_js == NULL)
    {
        ESP_LOGE(TAG, "Could not find js file in spiffs");
        return ESP_FAIL;
    }

    char* buffer = new char[JS_SIZE];
    memset(buffer, 0, JS_SIZE);
    fread(buffer, 1, JS_SIZE, f_js);
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    httpd_resp_send(req, buffer, JS_SIZE);
    fclose(f_js);
    delete[] buffer;
    return ESP_OK;
}

esp_err_t GUI::handle_update(httpd_req_t *req) 
{
    serialize_sndb_to_buffer(); // Populate sendable_packet_buffer with current sendables data
    httpd_resp_set_type(req, "application/octet-stream");
    httpd_resp_send(req, reinterpret_cast<char*>(sendable_packet_buffer), sizeof(sendable_packet_buffer) * 65);
    printf("Sent update packet to client\n");

    return ESP_OK;
}

esp_err_t GUI::handle_recievable(httpd_req_t *req)
{
    char* req_content = new char[RECV_BUFFER_SIZE];
    httpd_req_recv(req, req_content, RECV_BUFFER_SIZE);

    // Parse content
    char* buffer = new char[ENTRY_BUFFER_SIZE];
    memset(buffer, 0, ENTRY_BUFFER_SIZE);

    int i = 0;
    int entry = 0;

    char* key = new char[ENTRY_BUFFER_SIZE];
    char* s_value = new char[ENTRY_BUFFER_SIZE];
    char* type = new char[ENTRY_BUFFER_SIZE];

    while (req_content[i] != '\0' && entry < 3)
    {
        if (req_content[i] == '\n') 
        {
            if (entry == 0)
            {
                snprintf(key, ENTRY_BUFFER_SIZE-1, "%s", buffer);
            } else if (entry == 1)
            {
                snprintf(s_value, ENTRY_BUFFER_SIZE-1, "%s", buffer);
            } else if (entry == 2)
            {
                snprintf(type, ENTRY_BUFFER_SIZE-1, "%s", buffer);
            }
            
            strcpy(buffer, "");
            entry++;
        } else 
        {
            size_t len = strlen(buffer);
            buffer[len] = req_content[i];
            buffer[len + 1] = '\0';
        }

        i++;
    }

    delete[] buffer;

    // Edit recievable value
    if (is_recievable_registered(key)) {
        if (strcmp(type, "recievable>int") == 0 || strcmp(type, "recievable>option>int") == 0)
        {
            int value = atoi(s_value);
            BaseRecievable* rcbl = get_recievable(key);
            if (strcmp(type, "recievable>option>int") == 0) 
            {
                Option<int>* casted_rcbl = dynamic_cast<Option<int>*>(rcbl);
                if (!casted_rcbl)
                {
                    ESP_LOGW(TAG, "Requested recievable '%s' is not of type int", key);
                    httpd_resp_set_type(req, "application/text");
                    httpd_resp_send(req, "Recievable type mismatch", 24);
                    return ESP_OK;
                }
                casted_rcbl->set_value(value);
            } else 
            {
                ValueRecievable<int>* casted_rcbl = dynamic_cast<ValueRecievable<int>*>(rcbl);
                if (!casted_rcbl)
                {
                    ESP_LOGW(TAG, "Requested recievable '%s' is not of type int", key);
                    httpd_resp_set_type(req, "application/text");
                    httpd_resp_send(req, "Recievable type mismatch", 24);
                    return ESP_OK;
                }
                casted_rcbl->set_value(value);
            }
        } else if (strcmp(type, "recievable>float") == 0 || strcmp(type, "recievable>option>float") == 0)
        {
            float value = atof(s_value);
            BaseRecievable* rcbl = get_recievable(key);
            if (strcmp(type, "recievable>option>float") == 0) {
                Option<float>* casted_rcbl = dynamic_cast<Option<float>*>(rcbl);
                if (!casted_rcbl) 
                {
                    ESP_LOGW(TAG, "Requested recievable '%s' is not of type float", key);
                    httpd_resp_set_type(req, "application/text");
                    httpd_resp_send(req, "Recievable type mismatch", 24);
                    return ESP_OK;
                }
                casted_rcbl->set_value(value);
            } else {
                ValueRecievable<float>* casted_rcbl = dynamic_cast<ValueRecievable<float>*>(rcbl);
                if (!casted_rcbl) 
                {
                    ESP_LOGW(TAG, "Requested recievable '%s' is not of type float", key);
                    httpd_resp_set_type(req, "application/text");
                    httpd_resp_send(req, "Recievable type mismatch", 24);
                    return ESP_OK;
                }
                casted_rcbl->set_value(value);
            }
        } else if (strcmp(type, "recievable>string") == 0 || strcmp(type, "recievable>option>string") == 0)
        {
            BaseRecievable* rcbl = get_recievable(key);
            if (strcmp(type, "recievable>option>string") == 0) {
                Option<char*>* casted_rcbl = dynamic_cast<Option<char*>*>(rcbl);
                if (!casted_rcbl) 
                {
                    ESP_LOGW(TAG, "Requested recievable '%s' is not of type string", key);
                    httpd_resp_set_type(req, "application/text");
                    httpd_resp_send(req, "Recievable type mismatch", 24);
                    return ESP_OK;
                }
                casted_rcbl->set_value(s_value);
            } else {
                ValueRecievable<char*>* casted_rcbl = dynamic_cast<ValueRecievable<char*>*>(rcbl);
                if (!casted_rcbl) 
                {
                    ESP_LOGW(TAG, "Requested recievable '%s' is not of type string", key);
                    httpd_resp_set_type(req, "application/text");
                    httpd_resp_send(req, "Recievable type mismatch", 24);
                    return ESP_OK;
                }
                casted_rcbl->set_value(s_value);
            }
        }
    }

    printf("Recievable changed. key: %s, value: %s\n", key, s_value);

    httpd_resp_set_type(req, "application/text");
    httpd_resp_send(req, "Successful", 11);

    return ESP_OK;
}

esp_err_t GUI::handle_fetch_recievables(httpd_req_t *req)
{
    GUI::serialize_rcvb_to_json(); // Populate recievables_json_buffer with current recievables json data
    char* resp_str = GUI::recievables_json_buffer;

    // Send response
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, resp_str, strlen(resp_str));

    delete[] resp_str;

    return ESP_OK;
}

esp_err_t GUI::handle_triggered_command(httpd_req_t *req) {
    char req_content[BUTTON_BUFFER_SIZE];
    memset(req_content, 0, BUTTON_BUFFER_SIZE);
    httpd_req_recv(req, req_content, BUTTON_BUFFER_SIZE); // Returns button recievable key

    // Get recievable
    if (is_recievable_registered(req_content))
    {
        BaseRecievable* button_recievable = get_recievable(req_content);
        ButtonRecievable* button_recievable_casted = dynamic_cast<ButtonRecievable*>(button_recievable);
        if (button_recievable_casted != nullptr) 
        {
            button_recievable_casted->call();
            httpd_resp_set_type(req, "application/text");
            httpd_resp_send(req, "Successful", 11);
        } else 
        {
            ESP_LOGW(TAG, "Requested recievable '%s' is not a ButtonRecievable", req_content);
            httpd_resp_set_type(req, "application/text");
            httpd_resp_send(req, "Recievable not callable", 24);
            return ESP_OK;
        }
    }

    printf("Button Triggered. Key: %s", req_content);

    return ESP_OK;
}

esp_err_t GUI::start_soft_ap(void)
{
    ESP_LOGI(TAG, "NVS Flash Init");
    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_LOGI(TAG, "TCP/IP Init");
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_LOGI(TAG, "Eventloop create");
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .ssid_len = strlen(WIFI_SSID),
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .max_connection = 1,
            .beacon_interval = 100, // ms
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE)); // Disable power save mode

    ESP_LOGI(TAG, "launch_soft_ap finished. SSID: '%s' password: '%s'", WIFI_SSID, WIFI_PASS);

    return ESP_OK;
}

esp_err_t GUI::stop_soft_ap(void)
{
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_deinit());
    return ESP_OK;
}

esp_err_t GUI::start_webserver(void)
{
    if (server != NULL)
    {
        ESP_LOGI(TAG, "Webserver already running");
        return ESP_OK;
    }

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    config.server_port = SERVER_PORT;
    config.task_priority = 20;
    config.stack_size = 8192;

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t root_uri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = GUI::handle_root,
            .user_ctx = NULL};

        httpd_uri_t js_uri = {
            .uri = "/script.js",
            .method = HTTP_GET,
            .handler = GUI::handle_js,
            .user_ctx = NULL};

        httpd_uri_t update_uri = {
            .uri = "/update",
            .method = HTTP_GET,
            .handler = GUI::handle_update,
            .user_ctx = NULL};

        httpd_uri_t recievable_uri = {
            .uri = "/recievable",
            .method = HTTP_PUT,
            .handler = GUI::handle_recievable,
            .user_ctx = NULL};

        httpd_uri_t fetch_revievables_uri = {
            .uri = "/fetch_recievables",
            .method = HTTP_GET,
            .handler = GUI::handle_fetch_recievables,
            .user_ctx = NULL};

        httpd_uri_t button_uri = {
            .uri = "/trigger",
            .method = HTTP_PUT,
            .handler = GUI::handle_triggered_command,
            .user_ctx = NULL};

        httpd_register_uri_handler(server, &root_uri);
        httpd_register_uri_handler(server, &js_uri);
        httpd_register_uri_handler(server, &update_uri);
        httpd_register_uri_handler(server, &recievable_uri);
        httpd_register_uri_handler(server, &fetch_revievables_uri);
        httpd_register_uri_handler(server, &button_uri);

        return ESP_OK;
    }

    return ESP_FAIL;
}

esp_err_t GUI::stop_webserver(void)
{
    if (server)
    {
        esp_err_t result = httpd_stop(server);

        if (result == ESP_OK)
        {
            server = NULL;
            return ESP_OK;
        }
        
        return ESP_FAIL;
    }

    return ESP_OK;
}

void GUI::register_sendable(char *key, BaseSendable* sendable)
{
    if (is_sendable_registered(key))
    {
        ESP_LOGW(TAG, "Sendable \"%s\" is already registered.", key);
        return;
    }

    if (sendable_count < MAX_SENDABLES)
    {
        strncpy(sendables[sendable_count].key, key, sizeof(sendables[sendable_count].key) - 1);
        sendables[sendable_count].key[sizeof(sendables[sendable_count].key) - 1] = '\0';
        sendables[sendable_count].sendable = sendable;
        sendable_count++;
    }
    else
    {
        ESP_LOGW(TAG, "Sendable registry is full, cannot register \"%s\".", key);
    }
}

void GUI::register_recievable(char *key, BaseRecievable* recievable)
{
    if (is_recievable_registered(key))
    {
        ESP_LOGW(TAG, "Recievable \"%s\" is already registered", key);
        return;
    }

    if (recievable_count < MAX_RECIEVABLES)
    {
        strncpy(recievables[recievable_count].key, key, sizeof(recievables[recievable_count].key) - 1);
        recievables[recievable_count].key[sizeof(recievables[recievable_count].key) - 1] = '\0';
        recievables[recievable_count].recievable = recievable;
        recievable_count++;

        GUI::json_buffer_size += recievable->get_json_size() + 35;
    }
    else
    {
        ESP_LOGW(TAG, "Recievable registry is full, cannot register \"%s\".", key);
    }
}

BaseRecievable *GUI::get_recievable(char *key)
{
    for (size_t i = 0; i < recievable_count; ++i)
    {
        if (strcmp(recievables[i].key, key) == 0)
        {
            return recievables[i].recievable;
        }
    }
    return nullptr;
}

BaseSendable *GUI::get_sendable(char *key)
{
    for (size_t i = 0; i < sendable_count; ++i)
    {
        if (strcmp(sendables[i].key, key) == 0)
        {
            return sendables[i].sendable;
        }
    }
    return nullptr;
}

void GUI::remove_sendable(char *key) 
{
    for (size_t i = 0; i < sendable_count; ++i)
    {
        if (strcmp(sendables[i].key, key) == 0)
        {
            for (size_t j = i; j + 1 < sendable_count; ++j)
            {
                sendables[j] = sendables[j + 1];
            }
            sendable_count--;
            return;
        }
    }

    ESP_LOGW(TAG, "Sendable \"%s\" is not registered.", key);
}

bool GUI::is_sendable_registered(char *key)
{
    return get_sendable(key) != nullptr;
}

bool GUI::is_recievable_registered(char* key)
{
    return get_recievable(key) != nullptr;
}

void GUI::serialize_sndb_to_buffer() 
{
    size_t buffer_size = sizeof(u_int8_t) * 65 * sendable_count;

    if (sizeof(GUI::sendable_packet_buffer) < buffer_size)
    {
        delete[] GUI::sendable_packet_buffer;
        GUI::sendable_packet_buffer = nullptr;
    }
    if (GUI::sendable_packet_buffer == nullptr)
    {
        GUI::sendable_packet_buffer = new u_int8_t[buffer_size];
    }
    memset(GUI::sendable_packet_buffer, 0, buffer_size);

    // Copy sendables packets onto buffer
    for (size_t i = 0; i < sendable_count; ++i)
    {
        BaseSendable* sendable = sendables[i].sendable;
        sendable->refresh();
        auto data_ptr = sendable->get_serialized_data();
        memcpy(GUI::sendable_packet_buffer + (i * 65), data_ptr, 65); // 65 bytes per packet (forgive the hardcoding 💀)
    }
}

void GUI::serialize_rcvb_to_json() 
{
    size_t json_buffer_size = GUI::json_buffer_size;
    bool first = true;

    if (GUI::recievables_json_buffer == nullptr || json_buffer_size > sizeof(GUI::recievables_json_buffer))
    {
        if (GUI::recievables_json_buffer != nullptr)
        {
            delete[] GUI::recievables_json_buffer;
        }
        GUI::recievables_json_buffer = new char[json_buffer_size];
    }
    memset(GUI::recievables_json_buffer, '\0', json_buffer_size);

    if (GUI::recievable_count == 0)
    {
        strcpy(GUI::recievables_json_buffer, "{}");
        return;
    } else
    {
        strcat(GUI::recievables_json_buffer, "{");
    }

    char key_json[35];
    for (size_t i = 0; i < recievable_count; i++)
    {
        memset(key_json, '\0', 35);

        char* key = recievables[i].key;
        printf("Serializing recievable, key: %s\n", key);
        BaseRecievable* value = recievables[i].recievable;
        if (!first)
        {
            strcat(GUI::recievables_json_buffer, ",");
        }

        first = false;

        snprintf(key_json, 35, "\"%s\":", key);
        strcat(GUI::recievables_json_buffer, key_json);
        value->cat_json(GUI::recievables_json_buffer, json_buffer_size);
    }
    strcat(GUI::recievables_json_buffer, "}");

    printf("Serialized recievables json: %s\n", GUI::recievables_json_buffer);    
}

GUI *GUI::Get()
{
    if (instancePtr == nullptr && GUI::mutex)
    {
        if (xSemaphoreTake(GUI::mutex, (TickType_t)10) == pdTRUE)
        {
            instancePtr = new GUI();
            xSemaphoreGive(GUI::mutex);
        }
        else
        {
            ESP_LOGW(TAG, "Mutex couldn't be obtained");
        }
    }
    return instancePtr;
}

GUI::~GUI()
{
    stop_soft_ap();
    stop_webserver();
    ESP_LOGI(TAG, "GUI Deinitialized");
}
