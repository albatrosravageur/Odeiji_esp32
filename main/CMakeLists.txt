idf_component_register(SRCS "main.c"
                            "touch_button.c"
                            "bluetooth.c"
                    INCLUDE_DIRS "~/esp/esp-idf/examples/bluetooth/esp_hid_device/main"
                                 "~/esp/esp-idf/components/bt/include"
                                 "/home/albatros/esp/esp-idf/components/bt/host/bluedroid/api/include/api")