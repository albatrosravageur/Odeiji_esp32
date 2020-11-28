#include "BluetoothSerial.h"
#include <bt.h>
#include <Utilitaires.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define bt_h

BluetoothSerial SerialBT;

void bt_setup()
{
    delay(1000);
    //Serial.begin(115200);
    SerialBT.begin(DEVICE_NAME); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!\n");
}

void bt_loop(void *pvParameters)
{
    while (1)
    {
        if (SerialBT.available())
        {
            Serial.write(SerialBT.read());
        }
        Serial.println("I was in the bluetooth loop");
        delay(200);
    }
}
