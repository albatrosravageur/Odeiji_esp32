#ifndef Arduino
#include <Arduino.h>
#define Arduino
#endif

#ifndef Utilitaires_h
#include <Utilitaires.h>
#define Utilitaires_h
#endif

#ifndef Wifi_h
#include <WiFi.h>
#define Wifi_h
#endif

#include <TimeLib.h>

#ifndef BT
#include "BluetoothSerial.h"
#define BT
#endif

#ifndef Firebase_h
#include <firebase.h>
#define Firebase_h
#endif

#ifndef Led_h
#include <led.h>
#define Led_h
#endif

#ifndef bt_h
#include <bt.h>
#define bt_h
#endif

#include <touch.h>

#include <task.h>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Pin attribution
void setup_pin(){
  pinMode(PUSH_BUTTON, INPUT); //set pushbutton pin as a reader
  pinMode(FB_LED_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
}

TaskHandle_t fire_task;
TaskHandle_t led_task;
TaskHandle_t bt_task;

void setup()
{
  // Begin serial
  Serial.begin(9600);
  delay(500);

  // Setup pins
  setup_pin();
  // Setup time
  //setTime(0);

  xTaskCreatePinnedToCore(fire_loop, "Firebase Task",  12000 ,  NULL  ,5, &fire_task, 0);
  xTaskCreatePinnedToCore(led_loop, "LED Task",  12000 ,  NULL ,3, &led_task, 0);
  xTaskCreatePinnedToCore(bt_loop, "Bluetooth Task",  12000 ,  NULL  ,5, &bt_task, 0);

  fire_setup();
  touch_setup();
  led_setup();
  bt_setup();
}

void loop()
{
    vTaskDelay(10);
}
