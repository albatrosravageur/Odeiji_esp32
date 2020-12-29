
#ifndef Arduino
#include <Arduino.h>
#define Arduino
#endif

#ifndef Utilitaires_h
#include <Utilitaires.h>
#define Utilitaires_h
#endif

#ifndef Wifi_h
#include <WiFi.h>
#define Wifi_h
#endif

#include <led.h>

#include <HTTPClient.h>

#define Firebase_h

void connect2wifi();
void get_time();
void fire_loop(void *pvParameters);
void fire_get_time();
void wifi_research_loop(void *pvParameters);

class Fire_attributes
{
public:
  String meeting_ID = "";
  String wifi_ssid = "Livebox-F2CB";   // input your home or public wifi name
  String wifi_password = "bonjour123"; //password of wifi ssid
  String server_address = "http://us-central1-odeji-6a294.cloudfunctions.net/"; // the project name address from firebase id
};

Fire_attributes fire;
TaskHandle_t fire_task = NULL;
TaskHandle_t wifi_task = NULL;

void set_wifi_ssid(String ssid)
{
  fire.wifi_ssid = ssid;
}

String get_wifi_ssid()
{
  return fire.wifi_ssid;
}

void set_wifi_password(String password)
{
  fire.wifi_password = password;
}

String get_wifi_password()
{
  return fire.wifi_password;
}

void set_meeting_ID(String ID)
{
  fire.meeting_ID = ID;
}

String get_meeting_ID()
{
  return fire.meeting_ID;
}

void stop_firebase()
{
  if (fire_task != NULL)
  {
    vTaskDelete(fire_task);
    fire_task = NULL;
  }
}

bool fire_setup()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    return false;
  }
  else
  {
    stop_firebase();
    xTaskCreatePinnedToCore(fire_loop, "Firebase Task", 12000, NULL, 3, &fire_task, 0);
    return true;
  }
}

void fire_loop(void *pvParameters)
{
  while (1)
  {
    fire_get_time();
    delay(500);
  }
}
void stop_looking_for_wifi()
{
  if (wifi_task != NULL)
  {
    vTaskDelete(wifi_task);
    wifi_task = NULL;
  }
  else
  {
    Serial.println("Wifi task is null");
  }
}

void connect2wifi()
{
  stop_looking_for_wifi();
  xTaskCreatePinnedToCore(wifi_research_loop, "Bluetooth Task", 10000, NULL, 5, &wifi_task, 0);
}

void wifi_research_loop(void *pvParameters)
{
  char ssid[fire.wifi_ssid.length() + 1];
  char password[fire.wifi_password.length() + 1];
  fire.wifi_ssid.toCharArray(ssid, fire.wifi_ssid.length() + 1);
  fire.wifi_password.toCharArray(password, fire.wifi_password.length() + 1);

  WiFi.begin(ssid, password); //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(fire.wifi_ssid);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(fire.wifi_ssid);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP()); //print local IP address

  stop_looking_for_wifi();
}

void fire_get_time()
{
  HTTPClient http;
  String serverPath = fire.server_address + "GetTime?dest=" + fire.meeting_ID;
  Serial.println(serverPath);
  http.begin(serverPath.c_str());
  int status = http.GET();
  if (status > 0)
  {
    String message = http.getString();
    int time = message.toInt();
    if ((time < -MAX_TIME) || (time > MAX_TIME))
    {
      led_demo();
    }
    else
    {
      led_show_time(time);
    }
    Serial.println("Read time is " + String(time));
  }
  http.end();
}

void fire_play_pause()
{
  HTTPClient http;
  String serverPath = fire.server_address + "PlayPause?dest=" + fire.meeting_ID;
  http.begin(serverPath.c_str());
  long int useless = http.GET();
  http.end();
}

void fire_go_next()
{
  HTTPClient http;
  String serverPath = fire.server_address + "Next?dest=" + fire.meeting_ID;
  http.begin(serverPath.c_str());
  long int useless = http.GET();
  http.end();
}