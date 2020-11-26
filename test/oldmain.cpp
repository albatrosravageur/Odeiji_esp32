#include <Arduino.h>

#ifndef Utilitaires_h
#include <Utilitaires.h>
#endif
#ifndef Wifi_h
#include <WiFi.h>
#define Wifi_h
#endif
#ifndef HTTP_h
#include <HTTPClient.h>
#define HTTP_h
#endif
#include <TimeLib.h>
#include <ezButton.h>

#ifdef __AVR__
#include <avr/power.h>
#endif
#ifndef BT
#include "BluetoothSerial.h"
#define BT
#endif
#include <Pushbutton.h>
//#include <firebase.cpp>
#include <led.cpp>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Pin attribution

TaskHandle_t Task3;
TaskHandle_t Task2;
TaskHandle_t Task1;
Pushbutton button(PUSH_BUTTON);

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


void led_loop(void *pvParameters);

Led led;

int updates_finder();

void updates_finder_loop(void *pvParameters)
{
  while (1)
  {
    int time_val = updates_finder();
    if (time_val > 0)
    {
      Serial.print("Passing to the next point !\n");
      led.add_point(time_val);
    }
    delay(500);
  }
}

void setup()
{
  pinMode(PUSH_BUTTON, INPUT); //set pushbutton pin as a reader
  pinMode(BLINKER, OUTPUT);
  led.setup();
  bt_setup();
  Serial.begin(115200);
  //xTaskCreatePinnedToCore(updates_finder_loop, "Task1", 10000, NULL, 1, &Task1, 1); //includes wifi finder
  xTaskCreatePinnedToCore(bt_loop, "Task3", 10000, NULL, 4, &Task3, 1);
  xTaskCreatePinnedToCore(led_loop, "Task2", 10000, NULL, 5, &Task2, 0);
  setTime(0);
}

void loop()
{
  if (button.isPressed())
  {
    led.clear();
    led.next();
    delay(1000);
  }
  yield();
}
