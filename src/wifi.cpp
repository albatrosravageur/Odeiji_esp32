#include <Utilitaires.h>
#include <WiFi.h>
#include <bt.h>
#include <wifi.h> // Stored into the RTC fast memory, so it won't be deleted by the deep sleep mode
#include <led.h>

class WiFi_attributes
{
public:
  String ssid = "Livebox-F2CB";   // input your home or public wifi name
  String password = "bonjour123"; //password of wifi ssid
};

WiFi_attributes wifi;

TaskHandle_t wifi_task = NULL;

void set_wifi_ssid(String ssid)
{
  wifi.ssid = ssid;
}

String get_wifi_ssid()
{
  return wifi.ssid;
}

void RTC_IRAM_ATTR set_wifi_password(String password)
{
  wifi.password = password;
}

String get_wifi_password()
{
  return wifi.password;
}

void stop_looking_for_wifi()
{
  if (wifi_task != NULL)
  {
    try
    {
      vTaskDelete(wifi_task);
      wifi_task = NULL;
    }
    catch (const std::overflow_error &e)
    {
    }
  }
}

String get_wifi_state()
{
  if (wifi_task == NULL)
  {
    return "Not connected to Wifi";
  }
  else
  {
    if (WiFi.status() != WL_CONNECTED)
    {
    return "Looking for Wifi";
    }
    else
    {
      return "Connected to " + wifi.ssid;
    }
  }
}

void wifi_research_loop(void *pvParameters)
{
  Serial.println("in the loop");
  char ssid[wifi.ssid.length() + 1];
  char password[wifi.password.length() + 1];
  wifi.ssid.toCharArray(ssid, wifi.ssid.length() + 1);
  wifi.password.toCharArray(password, wifi.password.length() + 1);

  WiFi.begin(ssid, password);
  int i = 0;
  int j = 0;
  bool go_up = true;
  // Show progression with leds
  while ((WiFi.status() != WL_CONNECTED) && (j++ < 10 * MAX_WIFI_RESEARCH_TIME))
  {
    if (go_up)
      led_purple_loading(i++);
    else
      led_purple_loading(i--);
    if (i == 60)
      go_up = false;
    if (i == 0)
      go_up = true;
    delay(100);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    bt_send(WIFI_FAILURE);
    led_red_blink();
  }
  else
  {
    bt_send(WIFI_CONNECTED);
    led_purple_blink();
  }
  stop_looking_for_wifi();
}
void connect2wifi()
{
  stop_looking_for_wifi();
  xTaskCreatePinnedToCore(wifi_research_loop, "Wifi Task", 10000, NULL, WIFI_PRIORITY, &wifi_task, WIFI_CORE);
}
