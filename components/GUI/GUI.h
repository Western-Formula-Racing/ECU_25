#ifndef _GUI_
#define _GUI_
#include <esp_log.h>
#include <esp_http_server.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <esp_spiffs.h>
#include <stdio.h>
#include <cstring>
#include <unordered_map>
#include <Recievable.h>
#include <FloatRecievable.h>
#include <IntRecievable.h>
#include <StringRecievable.h>
#include <Option.h>

#define WIFI_SSID "WFR-ECU-AP"
#define WIFI_PASS "123456789" 
#define SERVER_PORT 3000
#define ENTRY_BUFFER_SIZE 50
#define HTML_SIZE 4815

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
    static esp_err_t handle_update(httpd_req_t *req);
    static esp_err_t handle_recievable(httpd_req_t *req);

    // Serialize sendables to json
    static char* serialize_sndb_to_json(void);

    // Serialize sendables to json
    static char* serialize_rcvb_to_json(void);

    // Serialize 

    // Maps
    static std::unordered_map<char*, int(*)()> int_callables;
    static std::unordered_map<char*, float(*)()> float_callables;
    static std::unordered_map<char*, char*(*)()> string_callables;
    static std::unordered_map<char*, bool(*)()> bool_callables;

    static std::unordered_map<char*, Recievable<int>*> int_recievables;
    static std::unordered_map<char*, Recievable<float>*> float_recievables;
    static std::unordered_map<char*, Recievable<char*>*> string_recievables;

public:
    // Register int sendables
    void register_int_sendable(char* key, int(*)());

    // Register float sendables
    void register_float_sendable(char* key, float(*)());

    // Register string sendables
    void register_string_sendable(char* key, char*(*)());

    // Register bool sendables
    void register_bool_sendable(char* key, bool(*)());

    // Register int recievable
    void register_int_recievable(char* key, Recievable<int>* int_recievable);

    // Register float recievable
    void register_float_recievable(char* key, Recievable<float>* float_recievable);

    // Register string recievable
    void register_string_recievable(char* key, Recievable<char*>* string_recievable);

    // Get int recievable
    Recievable<int>* get_int_recievable(char* key);

    // Get float recievable
    Recievable<float>* get_float_recievable(char* key);

    // Get string recievable
    Recievable<char*>* get_string_recievable(char* key);

    // Update int sendable
    void update_int_sendable(char* key, int(*callable)());

    // Update float sendable
    void update_float_sendable(char* key, float(*callable)());

    // Update string sendable
    void update_string_sendable(char* key, char*(*callable)());

    // Update bool sendable
    void update_bool_sendable(char* key, bool(*callable)());

    // Removes sendable
    void remove_sendable(char* key);

    // Checks sendable is registered from key
    bool is_sendable_registered(char* key);

    // Checks if recievable exists
    bool is_recievable_registered(char* key);

    // Deleting the copy constructor and copy reference constructor to prevent copies
    GUI(const GUI &) = delete;
    GUI &operator=(const GUI &) = delete;
    GUI(GUI &&) = delete;
    GUI &operator=(GUI &&) = delete;
    static GUI *Get();
};

#endif