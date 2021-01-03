#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/touch_pad.h"
#include "soc/rtc_periph.h"

#include <touch.h>
#include <Utilitaires.h>
#include <firebase.h>
#include <led.h>
#include <wifi.h>
#include <bt.h>
#include <bat.h>

class Touch{
    public:
    bool s_pad_activated;
    uint16_t threshold;
};

Touch touch;

TaskHandle_t touch_task;

void touch_set_thresholds()
{
    uint16_t touch_value;
    //read filtered value
    touch_pad_read_filtered(TOUCH_NUM, &touch_value);
    touch.threshold = touch_value * TOUCH_THRESH_PERCENT / 100;
    ESP_LOGI(TAG, "test init: touch pad val is " + String(touch_value) + "\n");
    //set interrupt threshold.
    ESP_ERROR_CHECK(touch_pad_set_thresh(TOUCH_NUM, touch.threshold));
}

void callback()
{
    //just a placeholder
}

void touch_turn_off_device()
{
    touchAttachInterrupt(TOUCH_NUM, callback, touch.threshold * TOUCH_THRESH_PERCENT / 100); //Smaller threshold to wake up to make it sleep tight
    Serial.println("Going to sleep now");
    Serial.flush();
    led_clear_display();
    Serial.end();
    fire_end();
    wifi_stop_search();
    bt_stop();
    bat_stop();
    led_red_blink();
    esp_deep_sleep_start();
}

void touch_loop(void *pvParameter)
{
    bool flagPP = 0;
    bool flagNext = 0;
    bool flagOff = 0;
    int count = 0;
    bool joker = 0;

    while (1)
    {
        //interrupt mode, enable touch interrupt
        touch_pad_intr_enable();
        if (touch.s_pad_activated || joker)
        {
            // if one step misses it's ok
            if (!touch.s_pad_activated)
            {
                joker = 0;
            }
            else
            {
                joker = 1;
            }
            count++;
            // Set flag corresponding to how long the count has been made
            if (count >= TIME_OFF_MS / TOUCH_PERIOD)
            {
                if (!flagOff)
                    Serial.println("FlagOff is set on");
                flagOff = 1;
            }

            else if (count >= TIME_NEXT_MS / TOUCH_PERIOD)
            {
                if (!flagNext)
                {
                    Serial.println("FlagNext is set on");
                    flagNext = 1;
                }
                led_red_loading(int((count * TOUCH_PERIOD - TIME_NEXT_MS) * (NUM_LEDS + 4) / 1000));
            }

            else if (count >= TIME_PP_MS / TOUCH_PERIOD)
            {
                if (!flagPP)
                {
                    Serial.println("FlagPlayPause is set on");
                    flagPP = 1;
                }
                led_green_loading(int((count * TOUCH_PERIOD - TIME_PP_MS) * (NUM_LEDS + 1) / 1000));
            }
            else
            {
                led_yellow_loading(int((count * TOUCH_PERIOD * (NUM_LEDS + 1)) / 1000));
            }

            ESP_LOGI(TAG, "T7 activated! \n");
        }
        else
        {
            if (flagOff)
            {
                touch_turn_off_device();
            }
            else if (flagNext)
            {
                fire_go_next();
                led_green_blink();
            }
            else if (flagPP)
            {
                fire_play_pause();
                led_yellow_blink();
            }
            else if (count)
                led_clear_display();

            flagPP = 0;
            flagNext = 0;
            flagOff = 0;
            count = 0;
        }
        // Clear information on pad activation
        touch.s_pad_activated = false;
        // Wait a while for the pad being released
        delay(TOUCH_PERIOD);
    }
}

/*
  Handle an interrupt triggered when a pad is touched.
  Recognize what pad has been touched and save it in a table.
 */
void touch_rtc_intr(void *arg)
{
    uint32_t pad_intr = touch_pad_get_status();
    //clear interrupt
    touch_pad_clear_status();
    if ((pad_intr >> TOUCH_NUM) & 0x01)
    {
        touch.s_pad_activated = true;
    }
}

void touch_setup()
{
    // Setup pin
    pinMode(TOUCH_PIN, INPUT);

    // Initialize touch pad peripheral, it will start a timer to run a filter
    ESP_LOGI(TAG, "Initializing touch pad");
    touch_pad_init();
    // If use interrupt trigger mode, should set touch sensor FSM mode at 'TOUCH_FSM_MODE_TIMER'.
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    // Set reference voltage for charging/discharging
    // the high reference voltage will be 2.7V - 1V = 1.7V, The low reference voltage will be 0.5V.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    // Init touch pad IO
    touch_pad_config(TOUCH_NUM, TOUCH_THRESH_NO_USE);
    // Initialize and start a software filter to detect slight change of capacitance.
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
    // Set thresh hold
    touch_set_thresholds();
    // Register touch interrupt ISR
    touch_pad_isr_register(touch_rtc_intr, NULL);

    //Deep sleep mode part
    esp_sleep_enable_touchpad_wakeup();
    //Don't waste the data such as wifi password and meeting ID
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_ON);

    //Start task
    xTaskCreatePinnedToCore(touch_loop, "Touch task", 2048, NULL, TOUCH_PRIORITY, &touch_task, TOUCH_CORE);
}
