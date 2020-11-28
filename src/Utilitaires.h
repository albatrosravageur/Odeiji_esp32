#include <Arduino.h>


#define Utilitaires_h

#define DEVICE_NAME "Odeji"

#define FB_LED_PIN A4
#define PATHTOPOINTS "/playground/meetings/M5Q1AFT33/agenda/points/"

#define LEDS_PIN 21
#define BRIGHTNESS  64
#define NUM_LEDS    24
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 13
enum firebase_to_led_indicator{
    NO_NEW_POINT =  255,
    END_OF_AGENDA = 0
};
#define TOUCH_PIN TOUCH_PAD_NUM7

