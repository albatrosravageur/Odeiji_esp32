/* Touch Pad Interrupt Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "ESP32APA102Driver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
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
#include "esp_pthread.h"
#include "brightness.h"
#include "ESP32APA102Driver.h"
#if CONFIG_IDF_TARGET_ESP32
#include "esp_adc_cal.h"
#endif

#define ESP_INTR_FLAG_DEFAULT 0

#define NO_OF_SAMPLES   64          //Multisampling
#define REFRESH_PERIOD_MS 1000      //sensor value is read every second

#define TAG_PP = "PLAY/PAUSE BUTTON"
#define TAG_BT = "BT BUTTON"

static xQueueHandle gpio_evt_queue = NULL;

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;

static const adc_atten_t atten = ADC_ATTEN_DB_0;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}


uint32_t get_bright_cont(){
     uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            adc_reading += adc1_get_raw((adc_channel_t)channel);
        }
        adc_reading /= NO_OF_SAMPLES;
        uint32_t cont = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
    return cont; // voltage
}

void bright_main(void)
{
    gpio_num_t adc_gpio_num;

    //Configure ADC
    adc1_config_width(width);
    adc1_config_channel_atten(channel, atten);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
}