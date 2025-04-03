idf_component_register(SRCS "StringRecievable.cpp" "FloatRecievable.cpp" "IntRecievable.cpp" "Recievable.cpp" "Option.cpp" "GUI.cpp"
                  REQUIRES esp_http_server esp_wifi nvs_flash
                  INCLUDE_DIRS ".")