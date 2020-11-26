
#ifndef Firebase_h
#include <IOXhop_FirebaseESP32.h>                                             // firebase library
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

#define FIREBASE_HOST "odeji-6a294.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "Vh2ojPFvUNp7PItyQMwJ32erPMI3DR1gPzjJnnSf"                    // the secret key generated from firebase
#define WIFI_SSID "oue-oue"                                          // input your home or public wifi name
#define WIFI_PASSWORD "cbonnard"                                    //password of wifi ssid

int point_duration;
                                               // led status received from firebase
void fire_setup() {

  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address

Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
Firebase.setString("LED_STATUS", "OFF");                                          //send initial string of led status
}

void fire_loop(void* pvParameters) {
  while(1){
    point_duration = Firebase.getInt("/playground/meetings/M5Q1AFT33/points/0/duration");
    Serial.println(point_duration);
    delay(500);
  }
}