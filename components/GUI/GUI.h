#ifndef _GUI_
#define _GUI_
#include <esp_log.h>
#include <esp_http_server.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

#define WIFI_SSID "WFR-ECU-AP"
#define WIFI_PASS "123456789" 
#define SERVER_PORT 3000

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

public:
    // Deleting the copy constructor and copy reference constructor to prevent copies
    GUI(const GUI &) = delete;
    GUI &operator=(const GUI &) = delete;
    GUI(GUI &&) = delete;
    GUI &operator=(GUI &&) = delete;
    static GUI *Get();
};

#endif