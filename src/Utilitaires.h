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
    SET_WIFI_SSID = '0',
    GET_WIFI_SSID = '1',
    SET_WIFI_PASSWORD = '2',
    GET_WIFI_PASSWORD='3',
    SET_MEETING_ID='4',
    GET_MEETING_ID='5',
    GO_CONNECT_FIREBASE='6',
    UNABLE_TO_CONNECT_FIREBASE='7',
    GO_CONNECT_WIFI='8',
    STOP_LOOKING_FOR_WIFI='9',
    STOP_FIREBASE = 'B',
    DO_DEMO_LEDS = 'A',
    PLAYPAUSE = 'C',
    NEXT = 'D'
};
