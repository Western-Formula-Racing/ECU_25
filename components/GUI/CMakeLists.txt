idf_component_register(SRCS "Option.cpp" "ValueRecievable.cpp" "Sendable.cpp" "ButtonRecievable.cpp" "GUI.cpp"
                  REQUIRES esp_http_server esp_wifi nvs_flash spiffs fatfs driver vfs
                  INCLUDE_DIRS ".")
