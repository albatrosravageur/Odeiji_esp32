#include <Utilitaires.h>

#include <TimeLib.h>

#include <FastLED.h>

#include <colorutils.h>
#include <led.h>
#include <TimeLib.h>
#include <firebase.h>

#define Led_h

CRGBPalette16 currentPalette;
TBlendType currentBlending;
void linear_display(int nb_leds, uint32_t color);
void SetupBlackAndWhiteStripedPalette();
void SetupPurpleAndGreenPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupTotallyRandomPalette();

extern CRGBPalette16 myRedWhiteBluePalette;

class Led
{
public:
    int8_t brightness = BRIGHTNESS;
    bool flag = true;
};

Led led;

// Define the array of leds
CRGB leds[NUM_LEDS];

void led_setup()
{
    pinMode(LED_DATA_PIN, OUTPUT);
    pinMode(LED_CLOCK_PIN, OUTPUT);
    FastLED.addLeds<APA102, LED_DATA_PIN, LED_CLOCK_PIN, BGR>(leds, NUM_LEDS);
}

void led_demo()
{
    led.flag = false;
    // Turn the LED on, then pause
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Yellow;
    }
    FastLED.show();

    delay(500);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Green;
    }
    FastLED.show();
    delay(500);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Red;
    }
    FastLED.show();

    delay(500);
    // Now turn the LED off, then pause
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
    led.flag = true;
    delay(500);
}

void led_show_time(int nb_leds)
{
    if (led.flag)
    {
        if (nb_leds < 0)
        {
            linear_display(-nb_leds, CRGB::Orange);
        }
        if (nb_leds >= 0)
        {
            linear_display(nb_leds + 1, CRGB::Green);
        }
    }
}
void led_blue_loading(int time)
{
    linear_display(time, CRGB::Blue);
}

void led_red_loading(int time)
{
    linear_display(time, CRGB::Red);
}

void led_green_loading(int time)
{
    linear_display(time, CRGB::Green);
}

void led_yellow_loading(int time)
{
    linear_display(time, CRGB::Yellow);
}

void led_purple_loading(int time)
{
    linear_display(time, CRGB::Purple);
}

void led_red_blink()
{
    led.flag = false;

    for (int i = 0; i < 3; i++)
    {
        linear_display(NUM_LEDS, CRGB::Red);
        delay(LED_BLINK_DELAY);
        led_clear_display();
        if (i < 2)
            delay(LED_BLINK_DELAY);
    }
    led.flag = true;
}

void led_blue_blink()
{
    led.flag = false;

    for (int i = 0; i < 3; i++)
    {
        linear_display(NUM_LEDS, CRGB::Blue);
        delay(LED_BLINK_DELAY);
        led_clear_display();
        if (i < 2)
            delay(LED_BLINK_DELAY);
    }
    led.flag = true;
}

void led_green_blink()
{
    led.flag = false;

    for (int i = 0; i < 3; i++)
    {
        linear_display(NUM_LEDS, CRGB::Green);
        delay(LED_BLINK_DELAY);
        led_clear_display();
        if (i < 2)
            delay(LED_BLINK_DELAY);
    }
    led.flag = true;
}

void led_yellow_blink()
{
    led.flag = false;

    for (int i = 0; i < 3; i++)
    {
        linear_display(NUM_LEDS, CRGB::Yellow);
        delay(LED_BLINK_DELAY);
        led_clear_display();
        if (i < 2)
            delay(LED_BLINK_DELAY);
    }
    led.flag = true;
}

void led_purple_blink()
{
    led.flag = false;

    for (int i = 0; i < 3; i++)
    {
        linear_display(NUM_LEDS, CRGB::Purple);
        delay(LED_BLINK_DELAY);
        led_clear_display();
        if (i < 2)
            delay(LED_BLINK_DELAY);
    }
    led.flag = true;
}

void led_clear_display()
{
    linear_display(NUM_LEDS, CRGB::Black);
}

void linear_display(int nb_leds, uint32_t color)
{
    for (int i = 0; i < NUM_LEDS + 1; i++)
    {
        if (i >= (NUM_LEDS - nb_leds))
            leds[i] = color;
        else
            leds[i] = CRGB::Black;
    }
    FastLED.show();
}
