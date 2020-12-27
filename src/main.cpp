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

#include <bt.h>

#include <touch.h>

#include <task.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Create task handles
TaskHandle_t led_task;

TaskHandle_t touch_task;

void setup()
{
  // Begin serial
  Serial.begin(115200);

  // Setup time
  setTime(0);
  Serial.println("Start setups");
  //  Setup bluetooth
  bt_setup();
  Serial.println("Bluetooth is set \n");


  // Setup touch button
  pinMode(TOUCH_PIN, INPUT);
  /*touch_setup();
  xTaskCreatePinnedToCore(touch_loop, "Touch task", 2048, NULL, 4, &touch_task, 0);
  Serial.println("Touch is set \n");*/

  // Setup leds control
  led_setup();
  Serial.println("Led is set \n");

  // Setup firebase control
 // fire_setup();
 // Serial.println("Firebase is set \n");
}

void loop() {
  vTaskDelay(2000);
}