#include <Wire.h>             // I2C
#include "RTClib.h"           // RTC
#include <SD.h>               // SD card
#include <util/delay.h>       // Temp/Press/Alt Sensor             
#include "Adafruit_BMP085.h"  // Temp/Press/Alt Sensor

int switchState = 0;
int count = 0;
long time = 0;
long debounce = 200;
long logInterval = 100; 

const int switchPin = 12;     // Switch to ignition charge
const int chipSelect = 10;    // CS line for SD card
const int greenLEDpin = 4;
const int redLEDpin = 5;

void error(char *);
String makeString(int);
File logfile;

RTC_DS1307 rtc;
Adafruit_BMP085 bmp;

void setup() {
  
  Serial.begin(9600); 
  pinMode(switchPin, INPUT);
  pinMode(chipSelect, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);
  pinMode(redLEDpin, OUTPUT);
  
  // See if card is present and can be initialized:
  if (!SD.begin(chipSelect)) 
    error("Card failed, or not present");
  Serial.println("card initialized.");

  // Create a new file
  char filename[] = "LOGGER00.CSV";
  for (long i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (!SD.exists(filename)) {
      // Only open a file that doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
      break;    // Leave loop
    }
  }
  if (! logfile) 
    error("Couldn't create file");
  Serial.print("Logging to: ");
  Serial.print(filename);
  
  Wire.begin();  
  
  // Connect first to RTC 
  if (! rtc.begin()) {
    error("Couldn't find RTC");
    logfile.println("RTC failed");
    while(1);
  }
  pinMode(switchPin, OUTPUT);
  rtc.adjust(DateTime(__DATE__, __TIME__));    // Adjust RTC to current time  

/* ------ Set up Sensors ---------------------------------- */

// Magnetometer
Wire.beginTransmission(0x1E);   // Address of magnetometer 0x1E
Wire.write(0x02);               // Select mode register
Wire.write(0x00);               // Continuous measurement mode
Wire.endTransmission();

// Press/Temp/Alt Sensor
bmp.begin();        // Initialize bmp

/* ------- End -------------------------------------------- */

// Print column names to csv file
logfile.println("HH, MM, SS, millis(), Magnetometer, x, y, z, "
   "Temp_C, Pres_Pa, Alt_Meter");
Serial.println("HH, MM, SS, millis(), Magnetometer, x, y, z, "
   "Temp_C, Pres_Pa, Alt_Meter");   // Echo to serial 

}

void loop() {
  DateTime now = rtc.now();
  int numSensors = 2;
  long currentTime = 0;
  long prevTime = 0;
  String master = "";
  
  switchState = digitalRead(switchPin);
  if( switchState == HIGH && millis() - time > debounce) 
  {     
    while (1) 
    {
      currentTime = millis();
      if ( currentTime - prevTime >= logInterval) {
        prevTime = currentTime;
        currentTime = millis();
        digitalWrite(greenLEDpin, HIGH);  // Starting recording
        master = master + now.hour()   // Timestamp
          + ", " + now.minute() + ", " + now.second() + ", "
          + millis() + ", ";
        for( int i = 0; i < numSensors; i++) { 
          master += makeString(i);
        } 
        logfile.println(master);  
        Serial.println(master);         // Echo to serial 
        digitalWrite(greenLEDpin, LOW); // End recording
        count++;                        // Increment count for flush time
        master = "";                    // Clear string of data
        
      }
      if (count >= 10) {    // Flush data to card
        digitalWrite(redLEDpin, HIGH);
        logfile.flush();
        digitalWrite(redLEDpin, LOW);
        count = 0;
      }
    }
  }
} 

void error(char *str) {
  Serial.print("Error: ");
  Serial.println(str);
  // red LED for error
  digitalWrite(redLEDpin, HIGH);
  while(1);
}

String makeString(int n) {
  String sensorString = "";
  switch (n) {
    case 0:   // Magnetometer
      int x, y, z;
      Wire.beginTransmission(0x1E);
      Wire.write(0x03);
      Wire.endTransmission();
      Wire.requestFrom(0x1E, 6);
      if(6 <= Wire.available()) {
        x = Wire.read()<<8; // x most significant bit
        x |= Wire.read();   // x least significant bit
        z = Wire.read()<<8; // z msb
        z |= Wire.read();   // z lsb
        y = Wire.read()<<8; // y msb
        y |= Wire.read();   // y lsb 
      }
      
      sensorString = sensorString + x + ", ";  // x-coordinate
      sensorString = sensorString + y + ", ";  // y-coordinate
      sensorString = sensorString + z + ", ";  // z-coordinate
      break;
      
    case 1:                           // BMP temperature, pressure, altitude
      sensorString = sensorString + bmp.readTemperature() + ", "; // Temp
      sensorString = sensorString + bmp.readPressure() + ", ";    // Pressure
      sensorString = sensorString + bmp.readAltitude() + ", ";    // Altitude
      break;

    default: 
      return "Error: Problem with recording data in makeString()";
      break; 
    }
 
    return sensorString;
}

