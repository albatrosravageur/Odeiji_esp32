/* Touch Pad Read Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include "esp_log.h"
#include "Utilitaires.h"
#include "touch.h"
#include "Arduino.h"

#define TOUCH_PAD_NO_CHANGE   (-1)
#define TOUCH_THRESH_NO_USE   0
#define TOUCH_FILTER_MODE_EN  (1)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

/*
  Read values sensed at all available touch pads.
 Print out values in a loop on a serial monitor.
 */
uint16_t touch_value;
TaskHandle_t touch_task;


static void tp_example_read_task(void *pvParameter)
{
    while (1) {
        touch_pad_read(TOUCH_PIN, &touch_value);
        Serial.print("T7 :"+String(touch_value));
    }
    Serial.println("\n");
    vTaskDelay(100);
}

static void tp_example_touch_pad_init(void)
{
    touch_pad_read(TOUCH_PIN, &touch_value);
    touch_pad_config(TOUCH_PIN, touch_value*1.5);
    touch_pad_set_trigger_mode(TOUCH_TRIGGER_BELOW);
    touch_pad_intr_enable();
}

void touch_setup(void)
{
    // Initialize touch pad peripheral.
    // The default fsm mode is software trigger mode.
    touch_pad_init();
    // Set reference voltage for charging/discharging
    // In this case, the high reference voltage will be 2.7V - 1V = 1.7V
    // The low reference voltage will be 0.5
    // The larger the range, the larger the pulse count value.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    tp_example_touch_pad_init();

    // Start task to read values sensed by pads
    xTaskCreatePinnedToCore(tp_example_read_task, "touch_pad_read_task", 2048, NULL,0, &touch_task,0);
}



