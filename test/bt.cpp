void bt_setup()
{
  SerialBT.begin("Odeiji"); //Bluetooth device name
  Serial.print("The device started, now you can pair it with bluetooth!\n");
}

void bt_loop(void *pvParameters)
{
  while (1)
  {
    String chaine = "";
    while (SerialBT.available())
        chaine = chaine + char(SerialBT.read());
    int value = chaine.toDouble();
    if ((value>0)&&(value<60))
        led.add_point(int(value));
    if (value == 9999)
    {
        SerialBT.println("Passed to next point");
        led.clear();
        led.next();
    }
    delay(100);
  }
}
