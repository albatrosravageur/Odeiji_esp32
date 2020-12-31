#include <Utilitaires.h>
#include <WiFi.h>
#include <bt.h>
#include <rtc_wake_stub_wifi.h> // Stored into the RTC fast memory, so it won't be deleted by the deep sleep mode
#include <led.h>

class WiFi_attributes
{
public:
  String ssid = "Livebox-F2CB";   // input your home or public wifi name
  String password = "bonjour123"; //password of wifi ssid
};

WiFi_attributes wifi;

TaskHandle_t wifi_task = NULL;

void RTC_IRAM_ATTR set_wifi_ssid(String ssid)
{
  wifi.ssid = ssid;
}

String RTC_IRAM_ATTR get_wifi_ssid()
{
  return wifi.ssid;
}

void RTC_IRAM_ATTR set_wifi_password(String password)
{
  wifi.password = password;
}

String RTC_IRAM_ATTR get_wifi_password()
{
  return wifi.password;
}

void stop_looking_for_wifi()
{
  if (wifi_task != NULL)
  {
    vTaskDelete(wifi_task);
    wifi_task = NULL;
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
  bt_send(String("Connecting to " + wifi.ssid));
  int i = 0;
  int j = 0;
  bool go_up = true;
  // Show progression with leds
  while ((WiFi.status() != WL_CONNECTED) && (j++ < 10*MAX_WIFI_RESEARCH_TIME))
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
    bt_send(String("Failed to connect to " + wifi.ssid));
    led_red_blink();
  }
  else
  {
    bt_send(String("Connected to " + wifi.ssid));
    led_purple_blink();
  }
  stop_looking_for_wifi();
}
void RTC_IRAM_ATTR connect2wifi()
{
  stop_looking_for_wifi();
  xTaskCreatePinnedToCore(wifi_research_loop, "Wifi Task", 10000, NULL, WIFI_PRIORITY, &wifi_task, WIFI_CORE);
}