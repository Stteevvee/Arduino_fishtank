/*
* Author:   Steve Jame
            Wouter Van Vlaenderen
            
* Todo:     Timing pump (1s ~ 1ml) -> Test
*/
#define DEBUG 1
// Setup pump(s)
// Pump 1
int pump1 = 8;            // Pump pin number
int duration1 = 5;        // Time in seconds
int day1[] = {0};         // Specific day(s), 0 = daily
String time1 = "23:00";   // Use this time format

// Pump 2
int pump2 = 9;            // Pump pin number
int duration2 = 2;        // Time in seconds
int day2[] = {3,5,6};     // Specific day(s), 0 = daily
String time2 = "23:00";   // Use this time format

// Setup RTC
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  // Set time, only use this function once to set time.
  // DS3231 seconds, minutes, hours, day, date, month, year
  // Sunday = 1, Monday = 2, Tuesday = 3, Wednesday = 4, Thursday = 5, Friday = 6, Saturday = 7
  // setDS3231time(00,34,22,3,28,6,16);
  
  // Pump output
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  digitalWrite(pump1, LOW);
  digitalWrite(pump2, LOW);
}

void loop() {
  // Read RTC time
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  
  #ifdef DEBUG
  #endif
  
  delay(1000);
  
  dosePump(pump1, duration1, day1, time1, (sizeof(day1)/sizeof(int)));
  dosePump(pump2, duration2, day2, time2, (sizeof(day2)/sizeof(int)));
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year) {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val) {
  return((val/16*10) + (val%16));
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val) {
  return( (val/10*16) + (val%10) );
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second));     // set seconds
  Wire.write(decToBcd(minute));     // set minutes
  Wire.write(decToBcd(hour));       // set hours
  Wire.write(decToBcd(dayOfWeek));  // set day of week
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month));      // set month
  Wire.write(decToBcd(year));       // set year (0 to 99)
  Wire.endTransmission();
}

void dosePump(int pumpID, int duration, int day[], String time, int arrSize){
  for(int j = 0; j <= arrSize - 1; j++){
    if(dayOfWeek == day[j] || day[j] == 0){
      if(time.substring(0,2).toInt() == hour && time.substring(3,5).toInt() == minute){
        if(second < duration){
          digitalWrite(pumpID, HIGH);
        }else{
          digitalWrite(pumpID, LOW);
        }
      }else{
        // Extra security LOW
        digitalWrite(pumpID, LOW);
      }
    }
  }
}
