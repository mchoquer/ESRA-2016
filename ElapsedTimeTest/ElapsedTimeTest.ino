// RTC Elapsed Time and Arduino I2C Test -- Master Code
#include <Wire.h>
#include "RTClib.h"
// ESRA 2016 Copy
long currentTime = 0;
long prevTime = 0;
const int ledPin = 13;
bool lightState = 0;
const int switchPin = 12;
bool switchState = 0;
bool prevState = 0;
long time = 0;
long debounce = 200;

RTC_DS1307 rtc;

 void showDate (const char* txt, const DateTime& date) { 
  Serial.print(txt);
  Serial.print(' ');
  Serial.print(date.year(), DEC);
  Serial.print("/");
  Serial.print(date.month(), DEC);
  Serial.print("/");
  Serial.print(date.day(), DEC);
  Serial.print(' ');
  Serial.print(date.hour(), DEC);
  Serial.print(":");
  Serial.print(date.minute(), DEC);
  Serial.print(":");
  Serial.print(date.second(), DEC);
}

void showTimeSpan(const char* txt, const TimeSpan& ts) {
    Serial.print(txt);
    Serial.print(" ");
    Serial.print(ts.days(), DEC);
    Serial.print(" days ");
    Serial.print(ts.hours(), DEC);
    Serial.print(" hours ");
    Serial.print(ts.minutes(), DEC);
    Serial.print(" minutes ");
    Serial.print(ts.seconds(), DEC);
    Serial.print(" seconds (");
    Serial.print(ts.totalseconds(), DEC);
    Serial.print(" total seconds)");
    Serial.println();
}   

void setup() {
  Wire.begin();
  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while(1);
  }
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  rtc.adjust(DateTime(__DATE__, __TIME__));    // Current time  
}

void loop() {
  DateTime now = rtc.now();
  switchState = digitalRead(switchPin);
  if (switchState == LOW && millis() - time > debounce) {

  currentTime = millis();  // Will set to countdown, turned on by switch with
    if (currentTime-prevTime >= 1000 && currentTime <= 9999) {   // if loop
      Serial.print("Current Time: ");
      Serial.print(currentTime/1000);
      Serial.print("s, Previous Time: ");
      Serial.print(prevTime/1000);
      Serial.print("s ");
      prevTime = currentTime;   // start new loop condition
      currentTime = millis();   // have to re-initialize time b/c of loop
      lightState != lightState;
      Serial.print(" Light: ");
      Serial.println(lightState);
      digitalWrite(ledPin, lightState);
      showDate("", now);
      Serial.print(" Current time: ");
      Serial.println(currentTime/1000);
    /* Wire.requestFrom(8, 6);     // Place for data
      while (Wire.available()) {// slave may send less than requested
        char c = Wire.read();   // receive a byte as character
        Serial.print(c);        // print the character */
    delay(1000);      // Scheme to keep time interval 1000ms  
  }
   
   if (currentTime >= 10000 && currentTime < 10999) {
    Serial.print("Blastoff! Current date and time: ");
    showDate("", now); 
    delay(10999-millis()); 
   }
   
   if (currentTime-prevTime >= 1000 && currentTime >= 11000) {   // Another scheme
      currentTime = millis();
      prevTime = currentTime; 
      /* Serial.print(' ');
      Serial.print(currentTime); */  // Used to test accuracy of time interval        
      Serial.print("Current time: " +
                    now.hour() );
      /*Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.print(' '); */
      long millisSecond = (millis()-10000L)/1000L;
      lightState != lightState;
      digitalWrite(ledPin, lightState);
      DateTime launchTime = now - TimeSpan(millisSecond);
      TimeSpan elapsedTime = now - launchTime;
      Serial.print("Millis :");
      Serial.print(millis());
      Serial.print("Elapsed Time: ");
      showTimeSpan("", elapsedTime);
      /* Wire.requestFrom(8, 6);
       while (Wire.available()) { // slave may send less than requested
        char c = Wire.read();
        Serial.print(c);
    } */            

  }

 }
}
