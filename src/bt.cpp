#include "BluetoothSerial.h"
#include <bt.h>
#include <firebase.h>
#include <touch.h>
#include <Utilitaires.h>
#include <led.h>
#include <wifi.h>
#include <bat.h>

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

void bt_send(int usecase)
{
    switch (usecase)
    {
    case WIFI_FAILURE:
        SerialBT.print(char(GO_CONNECT_WIFI));
        SerialBT.println(char(FAILURE));
        break;
    case WIFI_CONNECTED:
        SerialBT.print(char(GO_CONNECT_WIFI));
        SerialBT.println(char(SUCCESS));
        break;

    }
}

void bt_stop()
{
    SerialBT.end();
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
                wifi_set_ssid(message.substring(1));
                SerialBT.print(char(SET_WIFI_SSID));
                SerialBT.println("SSID set to " + message.substring(1));
                break;
            case GET_WIFI_SSID:
                SerialBT.print(char(GET_MEETING_ID));
                SerialBT.println(wifi_get_ssid());
                break;
            case SET_WIFI_PASSWORD:
                wifi_set_password(message.substring(1));
                SerialBT.print(char(SET_WIFI_PASSWORD));
                SerialBT.println("Password set to " + message.substring(1));
                break;
            case GET_WIFI_PASSWORD:
                SerialBT.print(char(GET_WIFI_PASSWORD));
                SerialBT.println(wifi_get_password());
                break;
            case SET_MEETING_ID:
                fire_getID(message.substring(1));
                SerialBT.print(char(SET_MEETING_ID));
                SerialBT.println("Meeting name set to " + message.substring(1));
                break;
            case GET_MEETING_ID:
                SerialBT.print(GET_MEETING_ID);
                SerialBT.println(fire_getID());
                break;
            case GO_CONNECT_FIREBASE:
                SerialBT.print(GO_CONNECT_FIREBASE);
                if (!fire_setup())
                    SerialBT.println(char(FAILURE));
                else
                    SerialBT.println(char(SUCCESS));
                // Feedback to app is asynchronous
                break;
            case STOP_CONNECT_MEETING:
                fire_base();
                SerialBT.println(char(STOP_CONNECT_MEETING));
                break;
            case GO_CONNECT_WIFI:
                wifi_connect();
                break;
            case STOP_LOOKING_FOR_WIFI:
                wifi_stop_search();
                led_clear_display();
                break;
            case DO_DEMO_LEDS:
                SerialBT.println("Led demo starts");
                led_demo();
                SerialBT.println("Led demo finishes");
                break;
            case DISCONNECT_FIREBASE:
                fire_base();
                SerialBT.println(char(DISCONNECT_FIREBASE));
                break;
            case PLAYPAUSE:
                fire_play_pause();
                SerialBT.println(char(PLAYPAUSE));
                led_yellow_blink();
                break;
            case NEXT:
                fire_go_next();
                SerialBT.println(char(NEXT));
                led_green_blink();
                break;
            case TURN_OFF:
                SerialBT.println(char(TURN_OFF));
                touch_turn_off_device();
                break;
            case ALL_PARAMETERS:
                SerialBT.print(char(ALL_PARAMETERS));
                SerialBT.print(String(bat_get_level())+"%");
                SerialBT.print(char('\n'));
                SerialBT.print(wifi_get_ssid());
                SerialBT.print(char('\n'));
                SerialBT.print(fire_getID());
                SerialBT.print(char('\n'));
                SerialBT.print(wifi_get_state());
                SerialBT.print(char('\n'));
                SerialBT.println(fire_get_state());
                break;
            case GET_BATTERY_LEVEL:
                SerialBT.print(char(GET_BATTERY_LEVEL));
                SerialBT.println(String(bat_get_level())+"%");
                break;
            }
        }
        delay(BT_PERIOD);
    }
}
