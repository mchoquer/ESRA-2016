int switchState = 0;
const int switchPin = 12;
#include <Wire.h>
#include "RTClib.h"

long time = 0;
long debounce = 200;

String makeString(int);

RTC_DS1307 rtc;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while(1);
  }
  pinMode(switchPin, OUTPUT);
  rtc.adjust(DateTime(__DATE__, __TIME__));    // Adjust RTC to current time  

/* ------ Set up Sensors ---------------------------------- */

//Magnetometer
Wire.beginTransmission(0x1E);   // Address of magnetometer 0x1E
Wire.write(0x02);               // Select mode register
Wire.write(0x00);               // Continuous measurement mode
Wire.endTransmission();

}

void loop() {
  //DateTime now = rtc.now();
  long currentTime = 0;
  long prevTime = 0;
  long interval = 1000;
  String intro = "\"HH\",\"MM\",\"SS\",\"millis()\",\"Magnetometer\",\"x\",\"y\",\"z\",";
  intro += '\n';
  String master = "";
  
  switchState = digitalRead(switchPin);
  if( switchState == HIGH && millis() - time > debounce) 
  { 
    Serial.print(intro); // Print column names of csv
    
    while (1) 
    {
      currentTime = millis();
      if ( currentTime - prevTime >= interval) {
        prevTime = currentTime;
        currentTime = millis();
        for( int i = 1; i < 2; i++) { // Change '2' to number of sensors later
          master += makeString(i);
        } 
        Serial.print(master); 
        master = "";      // Clear line of data
        
      }
    }
  }
} 

String makeString(int n) {
  DateTime now = rtc.now();
  String sensorString = "";
  sensorString = sensorString + "\"" + now.hour()   // Timestamp
  +"\",\"" + now.minute() + "\",\"" + now.second() + "\",\""
  + millis() + "\",";
  
  switch (n) {
    case 1:   // Magnetometer
      int x, y, z;
      Wire.beginTransmission(0x1E);
      Wire.write(0x03);
      Wire.endTransmission();
      Wire.requestFrom(0x1E, 6);
      if(6 <= Wire.available()) {
        x = Wire.read()<<8; // x msb
        x |= Wire.read();   // x lsb
        z = Wire.read()<<8;
        z |= Wire.read();
        y = Wire.read()<<8;
        y |= Wire.read();
      }
      sensorString = sensorString + '\"' + x + '\"' + ',';  // x-coordinate
      sensorString = sensorString + '\"' + y + '\"' + ',';  // y-coordinate
      sensorString = sensorString + '\"' + z + '\"' + ',' + '\n'; // z-coordinate
      break;
    
    default: 
      return "Error";
      break; 
    }
    
    return sensorString;
}

