#ifndef ESP32APA102Driver_H
#define ESP32APA102Driver_H

#include "apa102LEDStrip.h"
#include "colourObject.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/spi_master.h"
#include "driver/adc.h"
#include "driver/dac.h"
#include "esp_adc_cal.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "time.h"
#include "sys/time.h"

#define PIN_NUM_MOSI 5
#define PIN_NUM_CLK  18

#define totalPixels 60
#define bytesPerPixel 4
#define maxValuePerColour 64
#define maxSPIFrameInBytes 8000
#define maxSPIFrequency 10000000

//Main functions
int setupSPI();
void renderLEDs();
//apa102LEDStrip functions
void initLEDs(struct apa102LEDStrip *ledObject, short int numLEDs, unsigned char bytesPerLED, unsigned char globalBrightness);
void setPixel(struct apa102LEDStrip *ledObject, short int pixelIndex, unsigned char *pixelColour, int pixel);
void getPixel(struct apa102LEDStrip *ledObject, short int pixelIndex, unsigned char *pixelColour);
//colourObject functions
void initSimpleColourObject(struct colourObject *thisObject, unsigned char maxValue);
void initComplexColourObject(struct colourObject *thisObject, unsigned char maxValue, unsigned char colourBlockCount, unsigned char *rgbColourArray);
void gradientGenerator(struct colourObject *thisObject, unsigned short int colourIndex, unsigned short int bandwidth);
void getColour(struct colourObject *thisObject, short int colourIndex, unsigned char *colourBlock);

#endif