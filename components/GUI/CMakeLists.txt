idf_component_register(SRCS "ButtonRecievable.cpp" "StringRecievable.cpp" "FloatRecievable.cpp" "IntRecievable.cpp" "Option.cpp" "GUI.cpp"
                  REQUIRES esp_http_server esp_wifi nvs_flash spiffs
                  INCLUDE_DIRS ".")