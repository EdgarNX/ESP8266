#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.

#define WIFI_SSID "DIGI_a2e383"
#define WIFI_PASSWORD "1c622160"
#define FIREBASE_HOST "smartsolutions-d090c-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "83eft0BVRK8BNeH2RaHBxhQRNLoxsLdrQiHBDImK"

#define LED 2

const int ledPin =  5;       // the number of the LED pin

void setup() {
    
    pinMode(LED,OUTPUT);
    
    pinMode(ledPin, OUTPUT);
    
    digitalWrite(LED,0);
    
    Serial.begin(9600);
    
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("connecting");
    
    while (WiFi.status() != WL_CONNECTED) {
    
    Serial.print(".");
    
    delay(500);
    
    }
    
    Serial.println();
    
    Serial.print("connected: ");
    
    Serial.println(WiFi.localIP());
    
    Firebase.begin(FIREBASE_HOST);
    
    //Firebase.setInt("LEDStatus",0);

}

void loop() {

    //Serial.print(Firebase.getInt("LEDStatus"));
  
    
    if(Firebase.getInt("LEDStatus")){
      //digitalWrite(LED,HIGH);
      digitalWrite(ledPin,HIGH);
      Serial.println("ledPIN high high high high high high");
    }else{
     //digitalWrite(LED,LOW);
      digitalWrite(ledPin,LOW);
      Serial.println("ledPIN low low low low low low low");
    }
    
    if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());
      
      return;
    }
    
    delay(1000);
}
