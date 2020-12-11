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
#include <firebase.h>

#define Led_h

//CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;

class Led
{
public:
    int point_duration = 0;      // length of the point in absolu
    int point_left = NUM_LEDS;   // length of the point left to show (i.e 8 minutes over a 15 minutes duration points)
    uint8_t brightness = 127;
    time_t last_reset = now();   //last time we changed the point
    time_t last_passage = now(); //last time we were on the loop
};

Led led;
#include <FastLED.h>

// How many leds in your strip?

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 18
#define CLOCK_PIN 5

// Define the array of leds
CRGB leds[NUM_LEDS];

void led_setup() { 
    pinMode(DATA_PIN,OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
      // Uncomment/edit one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  	  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
      
      // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, RGB>(leds, NUM_LEDS);

      // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
       FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}

void led_demo() { 
    // Turn the LED on, then pause
  for(int i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB::Red;
  }
  FastLED.show();

  delay(500);
  for(int i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB::Green;
  }
  FastLED.show();
  delay(500);
  for(int i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB::Blue;
  }
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  for(int i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  while(1)
  {}
  delay(500);
}
/*
void led_setup()
{
    delay(1000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LEDS_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}*/
/*
void led_loop(void *pvParameters)
{

    while (1)
    {
        uint8_t firebase_input = state_to_display();

        if (firebase_input == NO_NEW_POINT)
            ; // nothing happens, no point has been added or changed

        if (firebase_input == END_OF_AGENDA)
        { // end of agenda, show demo leds
            led.point_duration = 0;
            Serial.println("End of agenda read in led, demo time !");
        }

        // if this is a new point
        if ((firebase_input != NO_NEW_POINT) && (firebase_input != END_OF_AGENDA) && (firebase_input != AGENDA_ON_PAUSE))
        {
            Serial.println("New point has been read in leds, length is :" + String(firebase_input));
            led.point_duration = firebase_input;
            led.last_reset = now();
        }

        // Compute points to display
        if (firebase_input != AGENDA_ON_PAUSE){  // if no pause, compute points to display with the difference of time
            led.point_left = led.point_duration - minute(now()-led.last_reset);
        }
        else{
            led.last_reset += now() - led.last_passage; // if pause, offset the difference of time to equilibrate the time 
        }
        led.last_passage = now();

        // display the whole thing
        if (led.point_duration) // if there is something to display
        {
            if (led.point_left >= 0)
            {
                linear_display(led.point_left, CRGB::Green);
            }
            else
            {
                linear_display(-led.point_left, CRGB::Orange);
            }
        }
        else // otherwise just show cool default display
        {
            ChangePalettePeriodically();

            static uint8_t startIndex = 0;
            startIndex = startIndex + 1; // motion speed

            FillLEDsFromPaletteColors(startIndex);
        }

        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);

        //Serial.println("I was in the led loop");
    }
}*/

void linear_display(int nb_leds, uint32_t color)
{

    for (int i = 0; i < NUM_LEDS + 1; i++)
    {
        if (i<nb_leds) leds[i] = ColorFromPalette(currentPalette, color, led.brightness, currentBlending);
        else           leds[i] = ColorFromPalette(currentPalette, color, 0, currentBlending);
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