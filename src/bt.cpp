#include "BluetoothSerial.h"
#include <bt.h>
#include <firebase.h>
#include <Utilitaires.h>
#include <led.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define bt_h

BluetoothSerial SerialBT;
TaskHandle_t bt_task;

void bt_setup()
{
    SerialBT.begin(DEVICE_NAME); //Bluetooth device name
    xTaskCreatePinnedToCore(bt_loop, "Bluetooth Task",10000, NULL, 5, &bt_task, 0);
    Serial.println("Bluetooth is set \n");
}

void bt_loop(void *pvParameters)
{

    while (1)
    {
        String message;
        if (SerialBT.available())
        {
            message = SerialBT.readString();
            Serial.println(message);
            switch (message.charAt(0))
            {
            case SET_MEETING_ID:
                set_meeting_ID(message.substring(1, -1));
                Serial.println("Meeting name set to " + message.substring(1, -1));
                break;
            case GET_MEETING_ID:
                SerialBT.println(get_meeting_ID());
                break;
            case SET_WIFI_PASSWORD:
                set_meeting_ID(message.substring(1, -1));
                Serial.println("Password set to " + message.substring(1, -1));
                break;
            case GET_WIFI_PASSWORD:
                Serial.println(get_wifi_password());
            case SET_WIFI_SSID:
                set_wifi_ssid(message.substring(1, -1));
                Serial.println("SSID set to " + message.substring(1, -1));
                break;
            case GET_WIFI_SSID:
                SerialBT.println(get_wifi_ssid());
                break;
            case GO_CONNECT_FIREBASE:
                fire_setup();
                vTaskDelete(bt_task);
                break;
            case DO_DEMO_LEDS:
                led_demo();
                break;
            }
        }
        delay(200);
    }
}
