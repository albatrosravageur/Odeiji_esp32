#include <bat.h>
#include <Utilitaires.h>

#define RESOLUTION 4096
#define OPERATING_VOLTAGE 3.3

// FULL_CHARGE = RESOLUTION*(4.2V/2)/OPERATING_VOLTAGE
#define FULL_CHARGE 2606
// MIN_CHARGE = RESOLUTION*(3.6V/2)/OPERATING_VOLTAGE
#define MIN_CHARGE 2234

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

void set_charge()
{
    pinMode(LED_CHARGED_PIN, INPUT_PULLUP);
    pinMode(LED_CHARGE_PIN, INPUT_PULLDOWN);
}

void set_charged()
{
    pinMode(LED_CHARGE_PIN, INPUT_PULLUP);
    pinMode(LED_CHARGED_PIN, INPUT_PULLDOWN);
}

void turn_off()
{
    pinMode(LED_CHARGE_PIN, INPUT_PULLUP);
    pinMode(LED_CHARGED_PIN, INPUT_PULLUP);
}

int bat_get_level()
{
    return bat.level;
}

void bat_loop(void *pvParameters)
{
    while (1)
    {
        bat.level = round(100 * (analogRead(BAT_LEVEL_READER) - MIN_CHARGE) / (FULL_CHARGE - MIN_CHARGE));
        if (bat.level > 90)
        {
            set_charged();
        }
        else
        {
            set_charge();
        }
        delay(1000);
    }
}