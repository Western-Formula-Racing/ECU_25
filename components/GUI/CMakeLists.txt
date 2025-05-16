idf_component_register(SRCS "ButtonRecievable.cpp" "StringRecievable.cpp" "FloatRecievable.cpp" "IntRecievable.cpp" "GUI.cpp"
                  REQUIRES esp_http_server esp_wifi nvs_flash spiffs fatfs driver vfs
                  INCLUDE_DIRS ".")