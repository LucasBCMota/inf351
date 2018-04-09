/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "f2fe0bd110904079b3e8c9be7805a19a";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Sbornia...";
char pass[] = "lauravadia";
int currentTime, previousTime = 0, interval = 1000;
int ledState = LOW , blinking = 1, count = -1;
int sensorPin = A0;
int sensorValue = 0;
bool control = false, night = false;

BLYNK_WRITE(V5)
{
  int pinValue = param.asInt();
  interval = pinValue;
}
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  interval = interval -100;
}
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  interval = interval + 100;
}
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt();
    blinking = pinValue;
    if(blinking == 0)
      digitalWrite(LED_BUILTIN, HIGH);
}
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt();
  count = pinValue;
}
BLYNK_WRITE(V7)
{
  int pinValue = param.asInt();
    if(pinValue == 1)
      control = true;
    else
      control = false;
}
BLYNK_WRITE(V6)
{
  int pinValue = param.asInt();
  if(pinValue == 1)
    night = true;
  else
    night = false;
}
void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Blynk.begin(auth, ssid, pass, IPAddress(139,59,206,133), 8080);
  Blynk.virtualWrite(V3, 1);
}

void loop()
{//
  Blynk.run();
  currentTime = millis();
  sensorValue = analogRead(sensorPin);
  Blynk.virtualWrite(V0, sensorValue);
  if(count == 0){
    count = -1;
    blinking = 0;
    digitalWrite(LED_BUILTIN, HIGH);
    Blynk.virtualWrite(V3, 0);
  }
  if(control)
    interval = sensorValue;
  if(night)
    if(sensorValue >600){
      blinking = 0;
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else{
      blinking = 1;
    }
   else
      blinking = 1;
  if((currentTime-previousTime)>=interval && blinking>0){
    if(count>0 && ledState == HIGH)
      count = count-1;
    previousTime = currentTime;
    if (ledState == LOW)
      ledState = HIGH;  // Note that this switches the LED *off*
    else
      ledState = LOW;   // Note that this switches the LED *on*
    digitalWrite(LED_BUILTIN, ledState);
  }
}

