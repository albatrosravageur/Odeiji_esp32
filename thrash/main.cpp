#include <http_get.h>

#ifndef Wifi_h
  #include <WiFi.h>
  #define Wifi_h
#endif
#ifndef Arduino_h
  #include <Arduino.h>
#endif
#ifndef Utilitaires_h
//  #include <Utilitaires.h>
#endif


int WiFiCon() {
    // Check if we have a WiFi connection, if we don't, connect.
  int xCnt = 0;

  if (WiFi.status() != WL_CONNECTED){

        Serial.println();
        Serial.println();
        Serial.print("Connecting to ");
        Serial.println(ssid);

        WiFi.mode(WIFI_STA);

        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED  && xCnt < 50) {
          delay(500);
          Serial.print(".");
          xCnt ++;
        }

        if (WiFi.status() != WL_CONNECTED){
          Serial.println("WiFiCon=0");
          return 0; //never connected
        } else {
          Serial.println("WiFiCon=1");
          Serial.println("");
          Serial.println("WiFi connected");
          Serial.println("IP address: ");
          Serial.println(WiFi.localIP());
          return 1; //1 is initial connection
        }

  } else {
    Serial.println("WiFiCon=2");
    return 2; //2 is already connected
  
  }
}


void setup() {

  Serial.begin(115200);

  WiFiCon();

}

void loop() {

  if (WiFiCon() > 0) {
     void http_get_loop();
  }
  delay(1000);
}
