
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "driver/touch_pad.h"
#include "soc/rtc_periph.h"
//#include "soc/sens_periph.h"
#include "touch.h"
#include "Utilitaires.h"
#include "firebase.h"
#include "Arduino.h"

#define TOUCH_THRESH_NO_USE (0)
#define TOUCH_THRESH_PERCENT (80)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

bool s_pad_activated;
uint16_t threshold;

void tp_example_set_thresholds(void)
{
    uint16_t touch_value;
    //read filtered value
    touch_pad_read_filtered(TOUCH_PAD_NUM7, &touch_value);
    threshold = touch_value * 0.8;
    ESP_LOGI(TAG, "test init: touch pad val is " + String(touch_value) + "\n");
    //set interrupt threshold.
    ESP_ERROR_CHECK(touch_pad_set_thresh(TOUCH_PAD_NUM7, threshold));
}
void callback()
{
    //placeholder callback function
}
void touch_loop(void *pvParameter)
{
    bool flag1 = 0;
    bool flag2 = 0;
    bool flag3 = 0;
    while (1)
    {
        //interrupt mode, enable touch interrupt
        touch_pad_intr_enable();
        if (s_pad_activated)
        {
            if (flag2)
            {
                flag3 = 1;
            }
            else if (flag1)
            {
                flag2 = 1;
            }
            else
            {
                flag1 = 1;
            }
            ESP_LOGI(TAG, "T7 activated! \n");
            // Wait a while for the pad being released
            delay(1000);
            // Clear information on pad activation
            s_pad_activated = false;
            // Reset the counter triggering a message
            // that application is running
            fire_go_next();
        }
        else
        {
            if (flag3)
            {
                touchAttachInterrupt(T7, callback, threshold);
                Serial.println("Going to sleep now");
                esp_deep_sleep_start();
            }
            else if (flag2)
            {
                fire_go_next();
            }
            else if (flag1)
            {
                fire_play_pause();
            }
            flag1 = 0;
            flag2 = 0;
        }

        delay(50);
    }
}

/*
  Handle an interrupt triggered when a pad is touched.
  Recognize what pad has been touched and save it in a table.
 */
void tp_example_rtc_intr(void *arg)
{
    uint32_t pad_intr = touch_pad_get_status();
    //clear interrupt
    touch_pad_clear_status();
    if ((pad_intr >> TOUCH_PAD_NUM7) & 0x01)
    {
        s_pad_activated = true;
    }
}

/*
 * Before reading touch pad, we need to initialize the RTC IO.
 */
void tp_example_touch_pad_init(void)
{
    //init RTC IO and mode for touch pad.
    touch_pad_config(TOUCH_PAD_NUM7, TOUCH_THRESH_NO_USE);
}

void touch_setup(void)
{
    // Initialize touch pad peripheral, it will start a timer to run a filter
    ESP_LOGI(TAG, "Initializing touch pad");
    touch_pad_init();
    // If use interrupt trigger mode, should set touch sensor FSM mode at 'TOUCH_FSM_MODE_TIMER'.
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    // Set reference voltage for charging/discharging
    // For most usage scenarios, we recommend using the following combination:
    // the high reference voltage will be 2.7V - 1V = 1.7V, The low reference voltage will be 0.5V.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    // Init touch pad IO
    tp_example_touch_pad_init();
    // Initialize and start a software filter to detect slight change of capacitance.
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
    // Set thresh hold
    tp_example_set_thresholds();
    // Register touch interrupt ISR
    touch_pad_isr_register(tp_example_rtc_intr, NULL);

    //Deep sleep mode part
    esp_sleep_enable_touchpad_wakeup();
}
