
#ifndef Firebase_h
#include <IOXhop_FirebaseESP32.h> // firebase library
#define Firebase_h
#endif

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

#include <firebase.h>

#define Firebase_h

#define FIREBASE_HOST "odeji-6a294.firebaseio.com"               // the project name address from firebase id
#define FIREBASE_AUTH "Vh2ojPFvUNp7PItyQMwJ32erPMI3DR1gPzjJnnSf" // the secret key generated from firebase
#define WIFI_SSID "oue-oue"                                      // input your home or public wifi name
#define WIFI_PASSWORD "cbonnard"                                 //password of wifi ssid

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
  char* wifi_ssid = "oue-oue";
  char* wifi_password = "cbonnard";
};

Fire_attributes fire;
TaskHandle_t fire_task;

void set_wifi_ssid(String ssid){
    ssid.toCharArray(fire.wifi_ssid,ssid.length()) ;
}

String get_wifi_ssid(){
  return String(fire.wifi_ssid);
}

void set_wifi_password(String password){
    password.toCharArray(fire.wifi_password,password.length()) ;
}

String get_wifi_password(){
  return String(fire.wifi_password);
}

void set_meeting_ID(String ID){
  fire.meeting_ID = ID;
}

String get_meeting_ID(){
  return fire.meeting_ID;
}

uint8_t state_to_display()
{
  if (fire.new_point_available)
  {
    fire.new_point_available = 0;
    return fire.current_duration;
  }
  if (fire.on_pause)
  {
    return AGENDA_ON_PAUSE; //as long as this is true, it keeps being on pause
  }
  else
  {
    return NO_NEW_POINT;
  }
}

void fire_go_to_next_point(){
  fire.current_point = 1 + Firebase.getInt(PATHTOMETADATA + String("current point"));
  Firebase.setInt(PATHTOMETADATA + String("current point"),fire.current_point);
}


void fire_setup()
{
  WiFi.begin(fire.wifi_ssid,fire.wifi_password); //try to connect with wifi
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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // connect to firebase

  xTaskCreatePinnedToCore(fire_loop, "Firebase Task", 12000, NULL, 3, &fire_task, 0);
  Serial.println("Firebase is set \n");
}

void fire_loop(void *pvParameters)
{
  while (1)
  {
    Serial.println("Meeting ID is "+fire.meeting_ID);
    delay(500);
  }
}

// OLD FIREBASE STUFF
/*   int read_current_point = Firebase.getInt(PATHTOMETADATA + String("current point"));
    if ((read_current_point != fire.current_point)||(fire.it_s_the_beginning)) // check database state and display next point
    {
      fire.current_point = read_current_point;
      Serial.println("I've read in the database that point number became "+String(read_current_point));
      fire.it_s_the_beginning = false;
      int agenda_changed = Firebase.getInt(PATHTOMETADATA + String("agenda changed"));
      if (agenda_changed)
      {
        // Refresh completely the database
        fire.nb_points = Firebase.getInt(PATHTOMETADATA + String("nb points"));
        for (int i = 0; i < MAX_AGENDA_SIZE; i++)
        {
          if (i < fire.nb_points)
          {
            fire.agenda[i] = Firebase.getInt(PATHTOPOINTS + String(i) + String("/duration"));
          }
          else
          {
            fire.agenda[i] = 0;
          }
        }
        Firebase.setInt(PATHTOMETADATA + String("agenda changed"), 0);
      }
      else{
        fire.current_point++;
      }
      if (fire.current_point < fire.nb_points)
      {
        fire.current_duration = fire.agenda[fire.current_point];
        fire.new_point_available = 1;
      }
    }
    fire.on_pause = Firebase.getInt(PATHTOMETADATA + String("on pause"));*/