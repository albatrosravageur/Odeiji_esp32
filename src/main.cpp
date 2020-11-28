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
//#include <bt.h>
#define bt_h
#endif

#include <touch.h>

#include <task.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Pin attribution
void setup_pin()
{
  pinMode(FB_LED_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
}

// Create task handles
TaskHandle_t fire_task;
TaskHandle_t led_task;
TaskHandle_t bt_task;
TaskHandle_t touch_task;

void setup()
{
  // Begin serial
  Serial.begin(115200);
  delay(500);

  // Setup pins
  setup_pin();

  // Setup time
  setTime(0);

  //  Setup bluetooth
  /*bt_setup();
  xTaskCreatePinnedToCore(bt_loop, "Bluetooth Task",10000, NULL, 5, &bt_task, 0);
  Serial.println("Bluetooth is set \n");
*/
  Serial.println("Start setups");
  // Setup Firebase
  fire_setup();
  xTaskCreatePinnedToCore(fire_loop, "Firebase Task", 12000, NULL, 5, &fire_task, 0);
  Serial.println("Firebase is set \n");

  // Setup touch button
  touch_setup();
  xTaskCreatePinnedToCore(touch_loop, "Touch task", 2048, NULL, 1, &touch_task, 1);
  Serial.println("Touch is set \n");

  // Setup leds control
  led_setup();
  xTaskCreatePinnedToCore(led_loop, "LED Task", 5000, NULL, 3, &led_task, 0);//(void *)&get_point_duration, 3, &led_task, 0);
  Serial.println("Led is set \n");
}

void loop()
{
  vTaskDelay(10);
}
