#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define FIREBASE_HOST "smartsolutions-d090c-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "83eft0BVRK8BNeH2RaHBxhQRNLoxsLdrQiHBDImK"

//led
#define PIN 2
#define NUMPIXELS 16

int ledStripFlag = 0;

int ledStripBrighness = 255;
int ledStripBrighnessFromFirebase;

int ledStripColorRed = 255;
int ledStripColorGreen = 255;
int ledStripColorBlue = 255;
int ledStripColorRedFromFirebase;
int ledStripColorGreenFromFirebase;
int ledStripColorBlueFromFirebase;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

//relay
int lightBulbPin =  5;       // the number of the LED pin

//servo motor
int servoPin = 0;

Servo Servo1;

void setup() {
  pinMode(lightBulbPin, OUTPUT);

  Servo1.attach(servoPin);

  pixels.begin();

  Serial.begin(115200);
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
    
    //relay for light bulb
    if(Firebase.getInt("LightBulbStatus")){
      digitalWrite(lightBulbPin,HIGH);  
      Serial.println("LightBulbStatus high high high high high high");
    }else{
      digitalWrite(lightBulbPin,LOW);      
      Serial.println("LightBulbStatus low low low low low low low");
    }

    //servo motor for latch
    if(Firebase.getInt("LatchStatus")){
      Servo1.write(75); 
      Serial.println("LatchStatus high high high high high high");
    }else{
      Servo1.write(0); 
      Serial.println("LatchStatus low low low low low low low");
    }

    //led strip
    if(Firebase.getInt("LedStatus")){
      if(ledStripFlag != 1){
        for(int i = 0; i < NUMPIXELS; i++){
          pixels.setPixelColor(i, pixels.Color(ledStripColorRed,ledStripColorGreen,ledStripColorBlue));
          pixels.setBrightness(ledStripBrighness);
          pixels.show();
        }
        ledStripFlag = 1;
      }
      
      Serial.println("LedStatus high high high high high high");
    } else {
      pixels.fill();
      pixels.show();
      ledStripFlag = 0;
      
      Serial.println("LedStatus low low low low low low low");
    }
    
    //led strip brightness
    if(Firebase.getInt("LedStatus")){
 
      Serial.println(Firebase.getInt("LedStripBrightness"));
      Serial.println(ledStripBrighnessFromFirebase);
      Serial.println(ledStripBrighness);
      
      ledStripBrighnessFromFirebase = map(Firebase.getInt("LedStripBrightness"), 0, 100, 0, 255);
      if(ledStripBrighness != ledStripBrighnessFromFirebase){
        
        ledStripBrighness = ledStripBrighnessFromFirebase;
        for(int i = 0; i<NUMPIXELS; i++){
          pixels.setPixelColor(i, pixels.Color(ledStripColorRed,ledStripColorGreen,ledStripColorBlue));
          pixels.setBrightness(ledStripBrighness);
          pixels.show();
        }
        Serial.println("LedStripBrightness changed changed changed changed changed");    
      }else{
        Serial.println("LedStripBrightness not changed not changed not changed");
      }
    }
    
    //led strip color
    if(Firebase.getInt("LedStatus")){
      
      Serial.println(Firebase.getInt("LedStripColor/Red"));
      Serial.println(Firebase.getInt("LedStripColor/Green"));
      Serial.println(Firebase.getInt("LedStripColor/Blue"));
      Serial.println(ledStripColorRed);
      Serial.println(ledStripColorGreen);
      Serial.println(ledStripColorBlue);

      ledStripColorRedFromFirebase = Firebase.getInt("LedStripColor/Red");
      ledStripColorGreenFromFirebase = Firebase.getInt("LedStripColor/Green");
      ledStripColorBlueFromFirebase = Firebase.getInt("LedStripColor/Blue");
      
      if(ledStripColorRed != ledStripColorRedFromFirebase || ledStripColorGreen != ledStripColorGreenFromFirebase || ledStripColorBlue != ledStripColorBlueFromFirebase){

        ledStripColorRed = ledStripColorRedFromFirebase;
        ledStripColorGreen = ledStripColorGreenFromFirebase;
        ledStripColorBlue = ledStripColorBlueFromFirebase;
        
        for(int i = 0; i<NUMPIXELS; i++){
          pixels.setPixelColor(i, pixels.Color(ledStripColorRed,ledStripColorGreen,ledStripColorBlue));
          pixels.setBrightness(ledStripBrighness);
          pixels.show();
        }
        
        Serial.println("LedStripColor changed changed changed changed changed");
      }else{
        Serial.println("LedStripColor not changed not changed not changed");
      }
    }

    if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());
      
      return;
    }
  } else
  {
     Serial.println("There is no internet connection!");
  }
    
    delay(1000);
}
