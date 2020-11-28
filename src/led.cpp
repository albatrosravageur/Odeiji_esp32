#include <Adafruit_NeoPixel.h>

#include <Arduino.h>

#include <Utilitaires.h>

#include <WiFi.h>

#include <HTTPClient.h>

#include <TimeLib.h>

#include <FastLED.h>

#include <TimeLib.h>

#include <task.h>
#include <colorutils.h>
#include <led.h>
#include <TimeLib.h>
#define Led_h

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;

time_t last_reset = now();
class Led
{
    public:
    int point_duration = 0;
    uint8_t brightness = 255;
};

Led led;

void led_setup()
{
    delay(1000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LEDS_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

void led_loop(void *pvParameters)
{
    int firebase_input = (uint32_t)pvParameters;

    if (firebase_input == NO_NEW_POINT) // nothing happens, no point has been added or changed
        if (firebase_input == END_OF_AGENDA)
        { // end of agenda, show demo leds
            led.point_duration = 0;
            Serial.println("End of agenda read in led, demo time !");
        }
    if ((firebase_input != NO_NEW_POINT) && (firebase_input != END_OF_AGENDA))
    {
        Serial.println("New point has been read in leds, length is :"+String(led.point_duration));
        led.point_duration = firebase_input;
        last_reset = now();
    }
    while (1)
    {
        if (!led.point_duration)
        {
            // cool default display
            ChangePalettePeriodically();

            static uint8_t startIndex = 0;
            startIndex = startIndex + 1; // motion speed

            FillLEDsFromPaletteColors(startIndex);
        }
        else
        {
            int time_left = minute(now() - last_reset) - led.point_duration;
            if (time_left >= 0)
            {
                linear_display(time_left, CRGB::Green);
            }
            else
            {
                linear_display(-time_left, CRGB::Orange);
            }
        }

        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);

        //Serial.println("I was in the led loop");
    }
}

void linear_display(int nb_leds, uint32_t color)
{

    for (int i = 0; i < nb_leds + 1; i++)
    {
        leds[i] = ColorFromPalette(currentPalette, color, led.brightness, currentBlending);
    }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, led.brightness, currentBlending);
        colorIndex += 3;
    }
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 50;
    static uint8_t lastSecond = 99;

    if (lastSecond != secondHand)
    {
        lastSecond = secondHand;
        if (secondHand == 0)
        {
            currentPalette = RainbowColors_p;
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 10)
        {
            currentPalette = RainbowStripeColors_p;
            currentBlending = NOBLEND;
        }
        if (secondHand == 15)
        {
            currentPalette = RainbowStripeColors_p;
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 20)
        {
            SetupPurpleAndGreenPalette();
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 25)
        {
            SetupTotallyRandomPalette();
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 30)
        {
            SetupBlackAndWhiteStripedPalette();
            currentBlending = NOBLEND;
        }
        if (secondHand == 35)
        {
            SetupBlackAndWhiteStripedPalette();
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 40)
        {
            currentPalette = CloudColors_p;
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 45)
        {
            currentPalette = PartyColors_p;
            currentBlending = LINEARBLEND;
        }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for (int i = 0; i < 16; i++)
    {
        currentPalette[i] = CHSV(random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid(currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green = CHSV(HUE_GREEN, 255, 255);
    CRGB black = CRGB::Black;

    currentPalette = CRGBPalette16(
        green, green, black, black,
        purple, purple, black, black,
        green, green, black, black,
        purple, purple, black, black);
}