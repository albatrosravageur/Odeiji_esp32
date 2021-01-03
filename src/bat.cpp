#include <Utilitaires.h>
#include <bat.h>

TaskHandle_t bat_task = NULL;

class Bat
{
public:
    int level = -1; // ratio from 0 to 100
};

Bat bat;

void bat_loop(void *pvParameters);

void bat_setup()
{
    pinMode(LED_CHARGE_PIN, INPUT_PULLUP);
    pinMode(LED_CHARGED_PIN, INPUT_PULLUP);
    pinMode(BAT_LEVEL_READER, INPUT);
    xTaskCreatePinnedToCore(bat_loop, "Battery LED Task", 12000, NULL, BAT_PRIORITY, &bat_task, BAT_CORE);
}

void bat_set_charge()
{
    pinMode(LED_CHARGED_PIN, INPUT_PULLUP);
    pinMode(LED_CHARGE_PIN, INPUT_PULLDOWN);
}

void bat_set_charged()
{
    pinMode(LED_CHARGE_PIN, INPUT_PULLUP);
    pinMode(LED_CHARGED_PIN, INPUT_PULLDOWN);
}

void bat_turn_off()
{
    pinMode(LED_CHARGE_PIN, INPUT_PULLUP);
    pinMode(LED_CHARGED_PIN, INPUT_PULLUP);
}

int bat_get_level()
{
    return min(max(1,bat.level),100); // Respect the [1;100]% interval for the battery level
}

void bat_stop()
{
    bat_turn_off();
    if (bat_task != NULL)
    {
        vTaskDelete(bat_task);
        bat_task = NULL;
    }
}

void bat_loop(void *pvParameters)
{
    while (1)
    {
        bat.level = round(100 * (analogRead(BAT_LEVEL_READER) - MIN_CHARGE) / (FULL_CHARGE - MIN_CHARGE));
        if (bat.level > 90)
        {
            bat_set_charged();
        }
        else
        {
            bat_set_charge();
        }
        delay(BAT_PERIOD);
    }
}