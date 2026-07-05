#ifndef _GUI_
#define _GUI_
#include <esp_log.h>
#include <esp_http_server.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <esp_vfs.h>
#include <esp_spiffs.h>
#include <stdio.h>
#include <cstring>
#include <Sendable.h>
#include <Recievable.h>
#include <ValueRecievable.h>
#include <Option.h>
#include <ButtonRecievable.h>

#define WIFI_SSID "WFR-ECU-AP"
#define WIFI_PASS "123456789" 
#define SERVER_PORT 80
#define ENTRY_BUFFER_SIZE 200
#define BUTTON_BUFFER_SIZE 32
#define RECV_BUFFER_SIZE 200
#define HTML_SIZE 6500
#define JS_SIZE 5400
#define MAX_SENDABLES 32
#define MAX_RECIEVABLES 32

class GUI
{
private:
    // Singleton device class structure
    static GUI *instancePtr;
    static SemaphoreHandle_t mutex;
    GUI();
    
    // Destructor
    ~GUI();

    // Handles
    httpd_handle_t server = NULL;

    // WiFi
    esp_err_t start_soft_ap(void);
    esp_err_t stop_soft_ap(void);

    // Webserver
    esp_err_t start_webserver(void);
    esp_err_t stop_webserver(void);

    // Handlers
    static esp_err_t handle_root(httpd_req_t *req);
    static esp_err_t handle_js(httpd_req_t *req);
    static esp_err_t handle_update(httpd_req_t *req);
    static esp_err_t handle_recievable(httpd_req_t *req);
    static esp_err_t handle_fetch_recievables(httpd_req_t *req);
    static esp_err_t handle_triggered_command(httpd_req_t *req);

    // Serialize sendables to buffer
    static void serialize_sndb_to_buffer(void);

    // Serialize recievables to json
    static void serialize_rcvb_to_json(void);

    // For maps
    struct SendableEntry
    {
        char key[32];
        BaseSendable* sendable;
    };

    struct RecievableEntry
    {
        char key[32];
        BaseRecievable* recievable;
    };

    // Buffer to store recievables json
    static char* recievables_json_buffer;
    static size_t json_buffer_size;

    // Buffer to store sendable packet data before sending to client
    static u_int8_t* sendable_packet_buffer; // Adjust size as needed

    // Arrays to store sendables and recievables
    static SendableEntry sendables[MAX_SENDABLES];
    static size_t sendable_count;
    static RecievableEntry recievables[MAX_RECIEVABLES];
    static size_t recievable_count;

public:
    // Register sendables
    void register_sendable(char* key, BaseSendable* sendable);

    // Register recievables
    void register_recievable(char* key, BaseRecievable* recievable);

    // Get int recievable
    static BaseRecievable* get_recievable(char* key);

    // Get sendable
    static BaseSendable* get_sendable(char* key);

    // Removes sendable
    void remove_sendable(char* key);

    // Checks sendable is registered from key
    static bool is_sendable_registered(char* key);

    // Checks if recievable exists
    static bool is_recievable_registered(char* key);

    // Deleting the copy constructor and copy reference constructor to prevent copies
    GUI(const GUI &) = delete;
    GUI &operator=(const GUI &) = delete;
    GUI(GUI &&) = delete;
    GUI &operator=(GUI &&) = delete;
    static GUI *Get();
};

#endif