/*
* Author:   Steve Jame
            Wouter Van Vlaenderen
*/

// Setup RTC
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  // Setup pump

}

void loop() {
  // Read RTC time
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

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

// Time bcd to decimal values
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
