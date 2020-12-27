
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

#define WIFI_SSID "oue-oue"                                                 // input your home or public wifi name
#define WIFI_PASSWORD "cbonnard"                                            //password of wifi ssid

void connect2wifi();
void get_time();
void fire_loop(void *pvParameters);
void fire_get_time();


class Fire_attributes
{
public:
  int current_duration = END_OF_AGENDA;
  String meeting_ID;
  int current_point = 0;
  int nb_points = 0;
  int agenda[MAX_AGENDA_SIZE];
  bool on_pause = false;
  bool new_point_available = 1;
  bool it_s_the_beginning = true;
  char *wifi_ssid = "oue-oue";
  char *wifi_password = "cbonnard";
  String server_address = "http://us-central1-odeji-6a294.cloudfunctions.net/"; // the project name address from firebase id
};

Fire_attributes fire;
TaskHandle_t fire_task;

void set_wifi_ssid(String ssid)
{
  ssid.toCharArray(fire.wifi_ssid, ssid.length());
}

String get_wifi_ssid()
{
  return String(fire.wifi_ssid);
}

void set_wifi_password(String password)
{
  password.toCharArray(fire.wifi_password, password.length());
}

String get_wifi_password()
{
  return String(fire.wifi_password);
}

void set_meeting_ID(String ID)
{
  fire.meeting_ID = ID;
}

String get_meeting_ID()
{
  return fire.meeting_ID;
}

void fire_setup()
{
  connect2wifi();
  xTaskCreatePinnedToCore(fire_loop, "Firebase Task", 12000, NULL, 3, &fire_task, 0);
}

void fire_loop(void *pvParameters)
{
  while (1)
  {
    fire_get_time();
    delay(500);
  }
}

void connect2wifi()
{
  WiFi.begin(fire.wifi_ssid, fire.wifi_password); //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

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
}

void fire_get_time()
{
  HTTPClient http;
  String serverPath = fire.server_address + "GetTime?dest="+ fire.meeting_ID;

  http.begin(serverPath.c_str());
  long int time = http.GET();
  http.end();

  if ((time < -MAX_TIME) || (time > MAX_TIME))
  {
    led_demo();
  }
  else
  {
    led_show_time(time);
  }
}

void fire_play_pause()
{
  HTTPClient http;
  String serverPath = fire.server_address + "PlayPause?dest="+ fire.meeting_ID;
  http.begin(serverPath.c_str());
  long int useless = http.GET();
  http.end();
 }

 void fire_go_next()
{
  HTTPClient http;
  String serverPath = fire.server_address + "Next?dest="+ fire.meeting_ID;
  http.begin(serverPath.c_str());
  long int useless = http.GET();
  http.end();
 }