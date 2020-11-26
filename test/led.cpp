#ifndef NeoPixel
#include <Adafruit_NeoPixel.h>
#define NeoPixel
#endif

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

#ifndef HTTP_h
#include <HTTPClient.h>
#define HTTP_h
#endif
#include <TimeLib.h>

#ifndef BT
#include "BluetoothSerial.h"
#define BT
#endif

#define Led_h

class Led
{
private:
  int delay_val = 1000; //en ms
  Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

public:
  int counter = 0;
  int go = 0;
  int point_len[NB_POINT] = {0};
  uint32_t last_hit = second(now());
  int get_delay()
  {
    return delay_val;
  }
  void add_point(int length)
  {
    int i = 0;
    while (point_len[i] && (i++ < NB_POINT + 1))
    {
    }
    if (i == (NB_POINT + 1))
      SerialBT.print("Unable to write new point as the agenda is full (>32 points) \n ");
    else
    {
      point_len[i] = length;
      SerialBT.print("Point added. Length of agenda is now ");
      SerialBT.println(i + 1);
    }
  }

  void next()
  {
    if (point_len[0])
    {
      delay_val = point_len[0] * 60 * 1000 / NUMPIXELS;
      //decalage
      for (int i = 0; i < NB_POINT - 1; i++)
      {
        point_len[i] = point_len[i + 1];
      }
      point_len[-1] = 0;
      if (delay_val < 300)
        Serial.print("WARNING THE DELAY VALUE IS UNDER 300 MS\n");
      go = 1;
    }
  }

  void setup()
  {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

    led.pixels.begin();
  }

  void clear()
  {
    led.pixels.clear();
    led.pixels.show();
    counter = 0;
    //vTaskDelete(Task2);
    //xTaskCreatePinnedToCore(led_loop, "Task2", 10000, NULL, 5, &Task2, 0);
    Serial.print("Cleared led.pixels\n");
    go = 0;
  }

  void led_loop(void *pvParameters)
  {
    while (1)
    {
      if (go)
      {
        led.pixels.clear();
        for (int i = 0; i < led.counter; i++)
        {/*
          if (led.counter < NUMled.pixels)
            led.pixels.setPixelColor(led.counter, pixels.Color(0, 60, 0));
          if (led.counter >= (2 * NUMPIXELS))
            pixels.setPixelColor(led.counter % NUMPIXELS, pixels.Color(60, 0, 0));
          if ((led.counter >= NUMPIXELS) && (led.counter < 2 * NUMPIXELS))
            pixels.setPixelColor(led.counter % NUMPIXELS, pixels.Color(40, 30, 0));*/
          if (i < NUMPIXELS)
            led.pixels.setPixelColor(i, led.pixels.Color(0, 90, 0));
          if (i >= (2 * NUMPIXELS))
            led.pixels.setPixelColor(i % NUMPIXELS, led.pixels.Color(90, 0, 0));
          if ((i >= NUMPIXELS) && (i < 2 * NUMPIXELS))
            led.pixels.setPixelColor(i % NUMPIXELS, led.pixels.Color(60, 30, 0));
        }

        /* if ((led.counter <= 3 * NUMled.pixels) && ((second(now())-led.last_hit) > led.get_delay()))
        {
          led.counter++;
          led.last_hit += led.get_delay()/1000;
          Serial.println("A new pixel has been turned on");
        }*/
        led.counter++;
        noInterrupts();
        led.pixels.show();
        interrupts();
      }
      delay(led.get_delay());
    }
  }
};