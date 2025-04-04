#include "GUI.h"
static const char *TAG = "GUI"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

GUI *GUI::instancePtr = nullptr;
SemaphoreHandle_t GUI::mutex = xSemaphoreCreateMutex();

std::unordered_map<char*, int(*)()> GUI::int_callables = {};
std::unordered_map<char*, float(*)()> GUI::float_callables = {};
std::unordered_map<char*, char*(*)()> GUI::string_callables = {};
std::unordered_map<char*, bool(*)()> GUI::bool_callables = {};
std::unordered_map<char*, Recievable<int>*> GUI::int_recievables = {};
std::unordered_map<char*, Recievable<float>*> GUI::float_recievables = {};
std::unordered_map<char*, Recievable<char*>*> GUI::string_recievables = {};

GUI::GUI()
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 1,
        .format_if_mount_failed = true
    };
    esp_vfs_spiffs_register(&conf);
    ESP_ERROR_CHECK(start_soft_ap());
    ESP_ERROR_CHECK(start_webserver());
    ESP_LOGI(TAG, "GUI Initialized");
}

esp_err_t GUI::handle_root(httpd_req_t *req)
{
    FILE* f_html = fopen("/spiffs/static.html", "r");

    if (f_html == NULL)
    {
        ESP_LOGE(TAG, "Could not find html file in SPIFFS");
        return ESP_FAIL;
    }

    char* buffer = new char[HTML_SIZE];
    memset(buffer, 0, HTML_SIZE);
    fread(buffer, 1, HTML_SIZE, f_html);
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, buffer, HTML_SIZE);
    fclose(f_html);
    return ESP_OK;
}

esp_err_t GUI::handle_update(httpd_req_t *req) 
{
    char* sndb_str = serialize_sndb_to_json();
    printf(sndb_str);
    //const char* rcvb_str = serialize_rcvb_to_json();
    size_t buff_size = strlen(sndb_str)+10;
    char* resp_str = new char[buff_size];
    strcpy(resp_str, "{");
    strcat(resp_str,  sndb_str);

    if (!strcmp("", sndb_str)) {
        strcat(resp_str, ",");
    }
    //strncat(resp_str, rcvb_str, buff_size-1);
    strcat(resp_str, "}");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_type(req, "text/json");
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}

esp_err_t GUI::handle_recievable(httpd_req_t *req)
{
    char* req_content = new char[RECV_BUFFER_SIZE];
    httpd_req_recv(req, req_content, RECV_BUFFER_SIZE);

    // Parse content
    char* buffer = new char[ENTRY_BUFFER_SIZE];
    strncpy(buffer, "", ENTRY_BUFFER_SIZE);

    int i = 0;
    int entry = 0;

    char* key = new char[ENTRY_BUFFER_SIZE];
    char* s_value = new char[ENTRY_BUFFER_SIZE];
    char* type = new char[ENTRY_BUFFER_SIZE];


    while (req_content[i] != '\0' && entry < 3)
    {
        strncat(buffer, "" + req_content[i], ENTRY_BUFFER_SIZE);
        
        if (req_content[i] == '\n') {
            if (entry == 0)
            {
                key = buffer;
            } else if (entry == 1)
            {
                s_value = buffer;
            } else if (entry == 2)
            {
                type = buffer;
            }
            
            
            buffer = new char[ENTRY_BUFFER_SIZE];
            strncpy(buffer, "", ENTRY_BUFFER_SIZE);
            entry++;
        }

        i++;
    }
    
    // Edit recievable value
    if (is_recievable_registered(key)) {
        if (strcmp(type, "int") || strcmp(type, "option<int>"))
        {
            int value = atoi(s_value);

            Recievable<int>* rcbl = int_recievables.at(key);
            rcbl->set_value(value);
        } else if (strcmp(type, "float") || strcmp(type, "option<float>"))
        {
            float value = atof(s_value);

            Recievable<float>* rcbl = float_recievables.at(key);
            rcbl->set_value(value);
        } else if (strcmp(type, "string") || strcmp(type, "option<string>"))
        {
            Recievable<char*>* rcbl = string_recievables.at(key);
            rcbl->set_value(s_value);
        }
    }

    // Send response
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_type(req, "text/html");
    const char* resp_str = "Update Successful";
    httpd_resp_send(req, resp_str, strlen(resp_str));

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

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t root_uri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = GUI::handle_root,
            .user_ctx = NULL};

        httpd_uri_t update_uri = {
            .uri = "/update",
            .method = HTTP_GET,
            .handler = GUI::handle_update,
            .user_ctx = NULL};

        //httpd_uri_t recievable_uri = {
        //    .uri = "/recievable",
        //    .method = HTTP_PUT,
        //    .handler = GUI::handle_recievable,
        //    .user_ctx = NULL};

        httpd_register_uri_handler(server, &root_uri);
        httpd_register_uri_handler(server, &update_uri);
        //httpd_register_uri_handler(server, &recievable_uri);

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

void GUI::register_int_sendable(char *key, int (*callback)())
{
    if (!is_sendable_registered(key)) 
    {
        int_callables.insert(std::pair<char*, int(*)()>{key, callback});
    } else 
    {
        ESP_LOGW(TAG, "Int sendable \"%s\" is already registered.", key);
    }
}

void GUI::register_float_sendable(char *key, float (*callback)()) 
{
    if (!is_sendable_registered(key)) 
    {
        float_callables.insert(std::pair<char*, float(*)()>{key, callback});
    } else 
    {
        ESP_LOGW(TAG, "Float sendable \"%s\" is already registered.", key);
    }
}

void GUI::register_string_sendable(char *key, char*(*callback)()) 
{
    if (!is_sendable_registered(key)) 
    {
        string_callables.insert(std::pair<char*, char*(*)()>{key, callback});
    } else 
    {
        ESP_LOGW(TAG, "String sendable \"%s\" is already registered.", key);
    }
}

void GUI::register_bool_sendable(char *key, bool(*callback)()) 
{
    if (!is_sendable_registered(key))
    {
        bool_callables.insert(std::pair<char*, bool(*)()>{key, callback});
    } else
    {
        ESP_LOGW(TAG, "Bool sendable \"%s\" is already registered.", key);
    }
}

void GUI::register_int_recievable(char *key, Recievable<int>* int_recievable)
{
    if (is_recievable_registered(key))
    {
        int_recievables.insert(std::pair<char*, Recievable<int>*>{key, int_recievable});
    } else
    {
        ESP_LOGW(TAG, "Int recievable \"%s\" is already registered", key);
    }
}

void GUI::register_float_recievable(char *key, Recievable<float>* float_recievable)
{
    if (is_recievable_registered(key))
    {
        float_recievables.insert(std::pair<char*, Recievable<float>*>{key, float_recievable});
    } else
    {
        ESP_LOGW(TAG, "Float recievable \"%s\" is already registered", key);
    }
}

void GUI::register_string_recievable(char *key, Recievable<char *>* string_recievable)
{
    if (is_recievable_registered(key))
    {
        string_recievables.insert(std::pair<char*, Recievable<char*>*>{key, string_recievable});
    } else
    {
        ESP_LOGW(TAG, "String recievable \"%s\" is already registered", key);
    }
}

Recievable<int>* GUI::get_int_recievable(char *key)
{    
    if (int_recievables.find(key) != int_recievables.end())
    {
        return int_recievables.at(key);
    } else
    {
        ESP_LOGW(TAG, "Int recievable \"%s\" is not registered", key);
        return nullptr;
    }
}

Recievable<float>* GUI::get_float_recievable(char *key)
{
    if (float_recievables.find(key) != float_recievables.end())
    {
        return float_recievables.at(key);
    } else
    {
        ESP_LOGW(TAG, "Float recievable \"%s\" is not registered", key);
        return nullptr;
    }
}

Recievable<char*>* GUI::get_string_recievable(char *key)
{
    if (string_recievables.find(key) != string_recievables.end())
    {
        return string_recievables.at(key);
    } else
    {
        ESP_LOGW(TAG, "Int recievable \"%s\" is not registered", key);
        return nullptr;
    }
}

void GUI::update_int_sendable(char *key, int (*callable)())
{
    if (int_callables.find(key) != int_callables.end())
    {
        remove_sendable(key);
        register_int_sendable(key, callable);
    } else
    {
        ESP_LOGW(TAG, "Int sendable %s is not registered", key);
    }
}

void GUI::update_float_sendable(char *key, float (*callable)())
{
    if (float_callables.find(key) != float_callables.end())
    {
        remove_sendable(key);
        register_float_sendable(key, callable);
    } else
    {
        ESP_LOGW(TAG, "Float sendable %s is not registered", key);
    }
}

void GUI::update_string_sendable(char *key, char *(*callable)())
{
    if (string_callables.find(key) != string_callables.end())
    {
        remove_sendable(key);
        register_string_sendable(key, callable);
    } else
    {
        ESP_LOGW(TAG, "String sendable %s is not registered", key);
    }
}

void GUI::update_bool_sendable(char *key, bool (*callable)())
{
    if (bool_callables.find(key) != bool_callables.end())
    {
        remove_sendable(key);
        register_bool_sendable(key, callable);
    } else
    {
        ESP_LOGW(TAG, "Bool sendable \"%s\" is not registered.", key);
    }
}

void GUI::remove_sendable(char *key) 
{
    if (int_callables.find(key) != int_callables.end())
    {
        int_callables.erase(key);
    } 
    else if (float_callables.find(key) != float_callables.end())
    {
        float_callables.erase(key);
    }
    else if (string_callables.find(key) != string_callables.end())
    {
        string_callables.erase(key);
    } 
    else if (bool_callables.find(key) != bool_callables.end())
    {
        bool_callables.erase(key);
    } else
    {
        ESP_LOGW(TAG, "Sendable \"%s\" is not registered.", key);
    }
}

bool GUI::is_sendable_registered(char *key)
{
    if (int_callables.find(key) != int_callables.end() || \
        float_callables.find(key) != float_callables.end() || \
        string_callables.find(key) != string_callables.end() || \
        bool_callables.find(key) != bool_callables.end())
    {
        return true;
    } else
    {
        return false;
    }
}

bool GUI::is_recievable_registered(char *key)
{
    if (int_recievables.find(key) != int_recievables.end() || \
        float_recievables.find(key) != float_recievables.end() || \
        string_recievables.find(key) != string_recievables.end())
    {
        return true;
    } else
    {
        return false;
    }
}

char* GUI::serialize_sndb_to_json() 
{
    size_t buffer_size = int_callables.size() * ENTRY_BUFFER_SIZE \
                         + float_callables.size() * ENTRY_BUFFER_SIZE \
                         + string_callables.size() * ENTRY_BUFFER_SIZE \
                         + bool_callables.size() * ENTRY_BUFFER_SIZE;

    bool first = true;

    if (buffer_size == 0)
    {
        return "";
    }

    size_t int_buf_size = int_callables.size() * ENTRY_BUFFER_SIZE;
    char* int_buf = new char[int_buf_size];
    for (std::pair<char*,int(*)()> int_entry: int_callables)
    {
        char* key = int_entry.first;
        int value = int_entry.second();
        char* int_val_buf = new char[16];
        snprintf(int_val_buf, (size_t)15, "%d", value);
        if (!first)
        {
            strcpy(int_buf, ",\"");
        } else
        {
            strcpy(int_buf, "\"");
            first = false;
        }

        strcat(int_buf, key);
        strcat(int_buf, "\":{");
        strcat(int_buf, "\"value\":");
        strcat(int_buf, int_val_buf);
        strcat(int_buf, ",\"type\":\"sendable>int\"}");
    }
    
    size_t float_buf_size = float_callables.size() * ENTRY_BUFFER_SIZE;
    char* float_buf = new char[float_buf_size];
    for (std::pair<char*, float(*)()> float_entry: float_callables)
    {
        char* key = float_entry.first;
        float value = float_entry.second();
        char* float_val_buf = new char[20];
        // only 5 decimal places
        snprintf(float_val_buf, 19, "%.5f", value);
        if (!first)
        {
            strcpy(float_buf, ",\"");
        } else
        {
            strcpy(float_buf, "\"");
            first = false;
        }

        strcat(float_buf, key);
        strcat(float_buf, ":\"{");
        strcat(float_buf, "\"value\":");
        strcat(float_buf, float_val_buf);
        strcat(float_buf, ",\"type\":\"sendable>float\"}");
    }
    
    size_t string_buf_size = string_callables.size() * ENTRY_BUFFER_SIZE;
    char* string_buf = new char[string_buf_size];
    for (std::pair<char*, char*(*)()> string_entry: string_callables)
    {
        char* key = string_entry.first;
        char* value = string_entry.second();
        
        if (!first)
        {
            strcpy(string_buf, ",\":");
        } else
        {
            strcpy(string_buf, "\":");
            first = false;
        }

        strcat(string_buf, key);
        strcat(string_buf, "\":{");
        strcat(string_buf, "\"value\":");
        strcat(string_buf, value);
        strcat(string_buf, ",\"type\":\"sendable>string\"}");
    }
    
    size_t bool_buf_size = bool_callables.size() * ENTRY_BUFFER_SIZE;
    char* bool_buf = new char[bool_buf_size];
    for (std::pair<char*, bool(*)()> bool_entry: bool_callables)
    {
        char* key = bool_entry.first;
        bool value = bool_entry.second();
        char* str_value;

        if (value) 
        {
            str_value = "true";    
        } else
        {
            str_value = "false";
        }
        
        
        if (!first)
        {
            strcpy(bool_buf, ",\":");
        } else
        {
            strcpy(bool_buf, "\":");
            first = false;
        }

        strcpy(bool_buf, key);
        strcat(bool_buf, "\":{");
        strcat(bool_buf, "\"value\":");
        strcat(bool_buf, str_value);
        strcat(bool_buf, ",\"type\":\"sendable>bool\"}");
    }

    // Buffer to store serialized data
    char* buffer = new char[strlen(int_buf) + strlen(float_buf) + strlen(string_buf) + strlen(bool_buf)];

    strncpy(buffer, int_buf, strlen(int_buf)-1);
    strncat(buffer, float_buf, strlen(float_buf)-1);
    strncat(buffer, string_buf, strlen(string_buf)-1);
    strncat(buffer, bool_buf, strlen(bool_buf)-1);
    return buffer;
}

char* GUI::serialize_rcvb_to_json() 
{
    size_t buffer_size = (RECV_BUFFER_SIZE * int_recievables.size()) \
                         + (RECV_BUFFER_SIZE * float_recievables.size()) \
                         + (RECV_BUFFER_SIZE * string_recievables.size());

    bool first = true;
    char* buffer = new char[buffer_size];
    

    if (buffer_size == 0)
    {
        return buffer;
    }

    size_t int_buffer_size = RECV_BUFFER_SIZE * int_recievables.size();
    char* int_buffer = new char[int_buffer_size];

    for (std::pair<char*, Recievable<int>*> recievable : int_recievables)
    {
        if (first)
        {
            strncpy(int_buffer, ",\"", buffer_size-1);
            first = false;
        } else {
            strncpy(int_buffer, "\"", buffer_size-1);
        }

        strncat(int_buffer, recievable.second->serialize_to_json(), buffer_size-1);
    }

    size_t float_buffer_size = RECV_BUFFER_SIZE * float_recievables.size();
    char* float_buffer = new char[float_buffer_size];
    memset(float_buffer, 0, float_buffer_size);
    for (std::pair<char*, Recievable<float>*> recievable : float_recievables)
    {
        if (first)
        {
            strncpy(float_buffer, ",\"", buffer_size-1);
            first = false;
        } else {
            strncpy(float_buffer, "\"", buffer_size-1);
        }

        strncat(float_buffer, recievable.second->serialize_to_json(), buffer_size-1);
    }

    size_t string_buffer_size = RECV_BUFFER_SIZE * string_recievables.size();
    char* string_buffer = new char[string_buffer_size];
    memset(string_buffer, 0, string_buffer_size);
    for (std::pair<char*, Recievable<char*>*> recievable : string_recievables)
    {
        if (first)
        {
            strncpy(string_buffer, ",\"", buffer_size-1);
            first = false;
        } else {
            strncpy(string_buffer, "\"", buffer_size-1);
        }

        strncat(string_buffer, recievable.second->serialize_to_json(), buffer_size+1);
    }

    strncat(buffer, int_buffer, buffer_size);
    strncat(buffer, float_buffer, buffer_size);
    strncat(buffer, string_buffer, buffer_size);

    ESP_LOGI(TAG, "Buffer: %s", buffer);

    return buffer;
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