#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

int buzzer = 13;//D7
int LED_FIRE = 14;//D5
int LED_NO_FIRE = 15;//D8
int flame_sensor = 16;//D0

const char* ssid="POCOPHONE";
const char* password = "pocophone";

HTTPClient http;
String LINK = 
  "http://us-central1-firealarm-b3fbd.cloudfunctions.net/sendMail/?dest=indranil12jana@gmail.com";

int flame_detected;
bool mailSent = false;

void setup()
{
  Serial.begin(9600);
  
  
  pinMode(buzzer, OUTPUT);
  pinMode(LED_FIRE, OUTPUT);
  pinMode(LED_NO_FIRE, OUTPUT);
  pinMode(flame_sensor, INPUT);

  connectToWifi();  
}

void loop()
{
  flame_detected = digitalRead(flame_sensor);
  if (flame_detected == 0)
  {
    Serial.println("Flame detected...! take action immediately.");
    digitalWrite(LED_NO_FIRE, LOW);
    digitalWrite(LED_FIRE, HIGH);
    digitalWrite(buzzer, HIGH);
//    blinkLED();
    sendMail();
  }
  else
  {
    Serial.println("No flame detected. stay cool");
    digitalWrite(LED_NO_FIRE, HIGH);
    digitalWrite(buzzer, LOW);
    digitalWrite(LED_FIRE, LOW);
    mailSent = false;
  }
  delay(1000);
}

void connectToWifi() {
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );
  
  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  Serial.println();

  Serial.println("Wifi Connected Success!");
}

//void blinkLED() {
//  digitalWrite(LED_FIRE, HIGH);
//  delay(200);
//  digitalWrite(LED_FIRE, LOW);
//  delay(200);
//}


void sendMail() {
  if(mailSent) return;

  http.begin(LINK); 

  int httpCode = http.GET();
  if (httpCode > 0) {
   String payload = http.getString();    //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload 
  } else {
    Serial.println("ERROR: Something went wrong..."); 
  }
  mailSent = true;
  http.end();
}


//MAGIC LINKS TO MAKE SENDING WORK

//Enable less secure apps - https://www.google.com/settings/security/lesssecureapps
//Disable Captcha temporarily so you can connect the new device/server - https://accounts.google.com/b/0/displayunlockcaptcha
