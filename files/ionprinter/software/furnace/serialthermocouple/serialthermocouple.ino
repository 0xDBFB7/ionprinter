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

#define P 1
#define I 1
#define D 4

double integral = 0;
double previous_error = 0;
float target = 1300;


void setup() {
  while (!Serial); // wait for Serial on Leonardo/Zero, etc
  
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  // wait for MAX chip to stabilize
  delay(500);
}



void loop() {
   double current_temp = 0;
   for(int i=0;i < 5;i++){
      current_temp += thermocouple.readCelsius();
      delay(100); 
   }
   current_temp /= 5;
   if(current_temp != current_temp){
      return; //continue
   }
   double error = target-current_temp;
   integral += error*0.5; //100 ms
   double derivative = error-previous_error;
   integral = constrain(integral,-300,300);
   
   double output = P * error + integral*I + D*derivative;
   output = constrain(output,0,255);
   previous_error = error;
   analogWrite(13,output);
   Serial.println(String(current_temp) + ", P: " + String(P*error) + " I: " + String(integral) 
                                  + " D: " +  String(derivative) + " O:" + String(output));
   
}
