
#include <Utilitaires.h>

#include <TimeLib.h>

#include <FastLED.h>

#include <colorutils.h>
#include <led.h>
#include <TimeLib.h>
#include <firebase.h>

#define Led_h

//CRGB leds[NUM_LEDS];

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
    int8_t brightness = 50;
};

Led led;
#include <FastLED.h>

#define DATA_PIN 18
#define CLOCK_PIN 5

// Define the array of leds
CRGB leds[NUM_LEDS];

void led_setup()
{
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
}

void led_demo()
{
    // Turn the LED on, then pause
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Orange;
    }
    FastLED.show();

    delay(500);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Yellow;
    }
    FastLED.show();
    delay(500);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Purple;
    }
    FastLED.show();

    delay(500);
    // Now turn the LED off, then pause
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
    delay(500);
}

void led_show_time(int nb_leds)
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

void linear_display(int nb_leds, uint32_t color)
{

    for (int i = 0; i < NUM_LEDS + 1; i++)
    {
        if (i < nb_leds)
            leds[i] = ColorFromPalette(currentPalette, color, led.brightness, currentBlending);
        else
            leds[i] = ColorFromPalette(currentPalette, color, 0, currentBlending);
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

/*void led_demo()
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
}*/

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