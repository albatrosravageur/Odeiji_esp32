
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

#include <bt.h>

#include <led.h>

#include <HTTPClient.h>

#define Firebase_h

void fire_loop(void *pvParameters);
void fire_get_time();

class Fire_attributes
{
public:
  String meeting_ID = "    ";
  String server_address = "http://us-central1-odeji-6a294.cloudfunctions.net/"; // the project name address from firebase id
};

Fire_attributes fire; // Stored into the RTC fast memory, so it won't be deleted by the deep sleep mode

TaskHandle_t fire_task = NULL;

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
    try
    {
      vTaskDelete(fire_task);
      fire_task = NULL;
    }
    catch (const std::overflow_error &e)
    {
    }
  }
}

bool fire_setup()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    return 0;
  }
  else
  {
    HTTPClient http;
    String serverPath = fire.server_address + "GetTime?dest=" + fire.meeting_ID;
    http.begin(serverPath.c_str());
    int status = http.GET();
    http.end();
    if (status == 200)
    {
      Serial.println("status is 200, I connect myself to the server");
      stop_firebase();
      xTaskCreatePinnedToCore(fire_loop, "Firebase Task", 12000, NULL, FIRE_PRIORITY, &fire_task, FIRE_CORE);
      return true;
    }
    if (status == 404)
      return 0;
    if (status == 500)
      return 0;
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

void fire_get_time()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    String serverPath = fire.server_address + "GetTime?dest=" + fire.meeting_ID;
    http.begin(serverPath.c_str());
    int status = http.GET();
    if (status > 0)
    {
      String message = http.getString();
      int time = message.toInt();
      if ((time < -POINT_MAX_TIME) || (time > POINT_MAX_TIME))
      {
        led_demo();
      }
      else
      {
        led_show_time(time);
      }
    }
    http.end();
  }
}

void fire_play_pause()
{
  if ((WiFi.status() == WL_CONNECTED) && (fire_task != NULL))
  {
    HTTPClient http;
    String serverPath = fire.server_address + "PlayPause?dest=" + fire.meeting_ID;
    http.begin(serverPath.c_str());
    long int useless = http.GET();
    http.end();
  }
}

void fire_go_next()
{
  if ((WiFi.status() == WL_CONNECTED)&& (fire_task != NULL))
  {
    HTTPClient http;
    String serverPath = fire.server_address + "Next?dest=" + fire.meeting_ID;
    http.begin(serverPath.c_str());
    long int useless = http.GET();
    http.end();
  }
}

String get_firebase_state()
{
  if (fire_task == NULL)
  {
    return "Not connected to meeting";
  }
  else if (WiFi.status() != WL_CONNECTED)
  {
    return "Unable to connect because no WiFi";
  }
  else
  {
    HTTPClient http;
    String serverPath = fire.server_address + "GetTime?dest=" + fire.meeting_ID;
    http.begin(serverPath.c_str());
    int status = http.GET();
    http.end();
    if (status == 200)
      return "Connected to meeting";
    if (status == 404)
      return "error 404 in server";
    if (status == 500)
      return "error 500 in server";
  }
  return "";
}