//void led_loop(void *pvParameters);
void led_setup();
void led_demo();

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.


void SetupPurpleAndGreenPalette();
void ChangePalettePeriodically();
void SetupTotallyRandomPalette();
void SetupBlackAndWhiteStripedPalette();
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void linear_display(int nb_leds, uint32_t color);
