#include <Arduino.h>
#define Utilitaires_h

// Leds APA102
#define LED_DATA_PIN 18
#define LED_CLOCK_PIN 5
#define BRIGHTNESS 64
#define NUM_LEDS 60
#define LED_TYPE APA102
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 2
#define LED_BLINK_DELAY 250 //ms

// Touch
#define TOUCH_PIN TOUCH_PAD_NUM7_GPIO_NUM
#define TOUCH_NUM TOUCH_PAD_NUM7
#define TOUCHPAD_FILTER_TOUCH_PERIOD (50)
#define TOUCH_THRESH_NO_USE (0)
#define TOUCH_THRESH_PERCENT (80)
#define TIME_PP_MS 1000
#define TIME_NEXT_MS 2000
#define TIME_OFF_MS 3000

// Firebase
#define POINT_MAX_TIME 100
#define FIREBASE_ADDRESS "http://us-central1-odeji-6a294.cloudfunctions.net/"

// Wifi
#define MAX_WIFI_RESEARCH_TIME 60 //s

// Battery
#define LED_CHARGED_PIN A1
#define LED_CHARGE_PIN A0
#define BAT_LEVEL_READER A12
#define RESOLUTION 4096
#define OPERATING_VOLTAGE 3.3
// FULL_CHARGE = RESOLUTION*(4.2V/2)/OPERATING_VOLTAGE
#define FULL_CHARGE 2606
// MIN_CHARGE = RESOLUTION*(3.6V/2)/OPERATING_VOLTAGE
#define MIN_CHARGE 2234

// Bluetooth
#define DEVICE_NAME "Odeji-123456" // This is a unique ID

enum bt_messages_protocol
{
    SET_WIFI_SSID = 'G',
    GET_WIFI_SSID = '1',
    SET_WIFI_PASSWORD = '2',
    GET_WIFI_PASSWORD = '3',
    SET_MEETING_ID = '4',
    GET_MEETING_ID = '5',
    GO_CONNECT_FIREBASE = '6',
    DISCONNECT_FIREBASE = '7',
    GO_CONNECT_WIFI = '8',
    STOP_LOOKING_FOR_WIFI = '9',
    DO_DEMO_LEDS = 'A',
    STOP_FIREBASE = 'B',
    PLAYPAUSE = 'C',
    NEXT = 'D',
    TURN_OFF = 'E',
    ALL_PARAMETERS = 'F',
    GET_BATTERY_LEVEL = 'H'
};
enum bt_messages_state
{
    FAILURE = '0',
    SUCCESS = '1'
};
enum bt_messages_sent
{
    WIFI_FAILURE,
    WIFI_CONNECTED
};

// FREE-RTOS
// Core distribution of threads
#define BAT_CORE 0
#define TOUCH_CORE 0
#define BT_CORE 0
#define FIRE_CORE 0
#define WIFI_CORE 0

// Priorities among threads (1 is min, 5 is max)
#define BAT_PRIORITY 1
#define TOUCH_PRIORITY 3
#define BT_PRIORITY 2
#define FIRE_PRIORITY 2
#define WIFI_PRIORITY 2

// Tasks period (ms)
#define BAT_PERIOD 1000
#define TOUCH_PERIOD 50
#define BT_PERIOD 50
#define FIRE_PERIOD 500
#define WIFI_PERIOD 100