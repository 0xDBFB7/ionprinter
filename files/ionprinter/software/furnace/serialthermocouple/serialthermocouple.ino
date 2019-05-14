/*************************************************** 
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include "Adafruit_MAX31855.h"

// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   3
#define MAXCS   4
#define MAXCLK  5

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

// Example creating a thermocouple instance with hardware SPI
// on a given CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS);

int integral = 0;
float target = 300;
void setup() {
  while (!Serial); // wait for Serial on Leonardo/Zero, etc
  
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  // wait for MAX chip to stabilize
  delay(500);
}



void loop() {
   double c = thermocouple.readCelsius();
   if(c < target){
      integral++;
   }
   else{
      integral--;
   }
   integral = constrain(integral,0,255);
   analogWrite(13,integral);
//  digitalWrite(13,HIGH);
   Serial.println(String(c) + "," + String(integral));
   delay(100);
   
}
