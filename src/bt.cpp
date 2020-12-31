#include "BluetoothSerial.h"
#include <bt.h>
#include <firebase.h>
#include <Utilitaires.h>
#include <led.h>
#include <rtc_wake_stub_wifi.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define bt_h

BluetoothSerial SerialBT;
TaskHandle_t bt_task;



void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    if (event == ESP_SPP_SRV_OPEN_EVT)
    {
        Serial.println("Client Connected");
        led_blue_blink();
    }

    if (event == ESP_SPP_CLOSE_EVT)
    {
        Serial.println("Client disconnected");
    }
}

void bt_setup()
{
    SerialBT.begin(DEVICE_NAME); //Bluetooth device name
    SerialBT.register_callback(callback);

    xTaskCreatePinnedToCore(bt_loop, "Bluetooth Task", 10000, NULL, BT_PRIORITY, &bt_task, BT_CORE);
}

void bt_send(String message)
{
    SerialBT.println(message);
}

void bt_loop(void *pvParameters)
{

    while (1)
    {
        String message;
        if (SerialBT.available())
        {
            message = SerialBT.readString();
            message.trim();
            Serial.println(message);
            switch (message.charAt(0))
            {
            case SET_WIFI_SSID:
                set_wifi_ssid(message.substring(1));
                SerialBT.println("SSID set to " + message.substring(1));
                break;
            case GET_WIFI_SSID:
                SerialBT.println(get_wifi_ssid());
                break;
            case SET_WIFI_PASSWORD:
                set_wifi_password(message.substring(1));
                SerialBT.println("Password set to " + message.substring(1));
                break;
            case GET_WIFI_PASSWORD:
                SerialBT.println(get_wifi_password());
                break;
            case SET_MEETING_ID:
                set_meeting_ID(message.substring(1));
                SerialBT.println("Meeting name set to " + message.substring(1));
                break;
            case GET_MEETING_ID:
                SerialBT.println(get_meeting_ID());
                break;
            case GO_CONNECT_FIREBASE:
                if (fire_setup())
                {
                    SerialBT.println("Firebase is set \n");
                }
                else
                {
                    SerialBT.println(UNABLE_TO_CONNECT_FIREBASE);
                }
                break;
            case GO_CONNECT_WIFI:
                connect2wifi();
                break;
            case STOP_LOOKING_FOR_WIFI:
                stop_looking_for_wifi();
                break;
            case DO_DEMO_LEDS:
                SerialBT.println("Led demo starts");
                led_demo();
                SerialBT.println("Led demo finishes");
                break;
            case STOP_FIREBASE:
                stop_firebase();
                break;
            case PLAYPAUSE:
                fire_play_pause();
                break;
            case NEXT:
                fire_go_next();
                break;
            }
        }
        delay(200);
    }
}
