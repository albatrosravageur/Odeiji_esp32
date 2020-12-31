#include <Utilitaires.h>
#include <TimeLib.h>
#include <firebase.h>
#include <led.h>
#include <bat.h>
#include <bt.h>
#include <touch.h>

void setup()
{
  // Begin serial
  Serial.begin(115200);

  // Setup time
  setTime(0);

  // Setup leds control
  led_setup();

  // Setup touch button
  touch_setup();

  // Say hello to daddy
  led_demo();

  //  Setup bluetooth
  bt_setup();

  // Setup battery leds display
  bat_setup();
}

void loop()
{
  vTaskDelay(2000);
}