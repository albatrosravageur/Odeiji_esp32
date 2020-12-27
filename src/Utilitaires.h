#include <Arduino.h>


#define Utilitaires_h

#define DEVICE_NAME "Odeji"

#define FB_LED_PIN A4
#define MAX_AGENDA_SIZE 250

#define BRIGHTNESS  64
#define NUM_LEDS    60
#define LED_TYPE    APA102
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 2
enum firebase_to_led_indicator{
    NO_NEW_POINT =  255,
    AGENDA_ON_PAUSE = 254,
    END_OF_AGENDA = 0,
    MAX_TIME = 100 //for a point
};
#define TOUCH_PIN TOUCH_PAD_NUM7_GPIO_NUM

enum bt_messages_type{
    SET_WIFI_SSID = 0,
    GET_WIFI_SSID,
    SET_WIFI_PASSWORD,
    GET_WIFI_PASSWORD,
    SET_MEETING_ID,
    GET_MEETING_ID,
    GO_CONNECT_FIREBASE,
    UNABLE_TO_CONNECT_FIREBASE,
    DO_DEMO_LEDS = 'A'
};
