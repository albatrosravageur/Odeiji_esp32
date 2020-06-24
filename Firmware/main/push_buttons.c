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
#include "push_buttons.h"
#include "ESP32APA102Driver.h"

#define GPIO_INPUT_IO_0 25
#define GPIO_INPUT_IO_1 5
#define GPIO_INPUT_PIN_SEL ((1ULL << GPIO_INPUT_IO_0) | (1ULL << GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0

#define TAG_PP = "PLAY/PAUSE BUTTON"
#define TAG_BT = "BT BUTTON"

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

int cont;

int getcont(){
    return cont;
}


void tx_task1(void *arg)
{
    while (1)
    {
        cont = gpio_get_level(GPIO_INPUT_IO_0);
        if(cont==1){
            printf("%d\n", cont);
        }
        
        /*char **ch = "lvl\n"; int i;
        send_by_BT(ch);*/

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void push_main(void)
{
    xTaskCreate(tx_task1, "tx_task1", CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    gpio_config_t io_conf;

    gpio_num_t adc_gpio_num;

    vTaskDelay(2 * portTICK_PERIOD_MS);

    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_down_en = 1;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);

    //create a queue to handle gpio event from isr
    xTaskCreate(tx_task1, "tx_task1", CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 3, NULL);
}