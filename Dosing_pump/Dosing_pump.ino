/*
* Author:   Steve Jame
            Wouter Van Vlaenderen
            
* Todo:     Timing pump (1s ~ 1ml)
*/

// Setup RTC
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

// Pump pins
int pump1 = 8, pump2 = 9;


void setup() {
  Serial.begin(9600);
  Wire.begin();
  // Set time, only use this function once to set time.
  // DS3231 seconds, minutes, hours, day, date, month, year
  // Sunday = 1, Monday = 2, Tuesday = 3, Wednesday = 4, Thursday = 5, Friday = 6, Saturday = 7
  // setDS3231time(00,29,12,6,1,4,16);
  
  // Pump output
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
}

void loop() {
  // Read RTC time
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  
  // Debug monitor + pump
  Serial.print(hour);
  Serial.print(':');
  Serial.print(minute);
  Serial.print(':');
  Serial.println(second);
  delay(1000);
  
  // Fill in time
  if(hour = 0){
    digitalWrite(pump1, HIGH);
    digitalWrite(pump2, HIGH);
  }
  // End debug
  
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
  return( (val/16*10) + (val%16) );
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val) {
  return( (val/10*16) + (val%10) );
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
