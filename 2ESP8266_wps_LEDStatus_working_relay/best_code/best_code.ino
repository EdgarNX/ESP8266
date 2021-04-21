// Test for ESP8266 WPS connection.

#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "smartsolutions-d090c-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "83eft0BVRK8BNeH2RaHBxhQRNLoxsLdrQiHBDImK"

const int ledPin =  5;       // the number of the LED pin

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  // WPS works in STA (Station mode) only.
  WiFi.mode(WIFI_STA);
  delay(1000);
  // Called to check if SSID and password has already been stored by previous WPS call.
  // The SSID and password are stored in flash memory and will survive a full power cycle.
  // Calling ("",""), i.e. with blank string parameters, appears to use these stored values.
  WiFi.begin("","");
  // Long delay required especially soon after power on.
  delay(4000);
  // Check if WiFi is already connected and if not, begin the WPS process.
  if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nAttempting connection ...");
      WiFi.beginWPSConfig();
      // Another long delay required.
      delay(3000);
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected!");
        Serial.println(WiFi.localIP());
        Serial.println(WiFi.SSID());
        Serial.println(WiFi.macAddress());
      }
      else {
        Serial.println("Connection failed!");
      }
  }
  else {
    Serial.println("\nConnection already established.");
  }
  Firebase.begin(FIREBASE_HOST);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
  
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
  } else
  {
          Serial.println("fuck fuck fuck fuck fuck");
  }
    
    delay(1000);
}
