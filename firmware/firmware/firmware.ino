#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

uint16_t x_value = 0;
uint16_t y_value = 0;
uint16_t focus_value = 0;

float hv_set_voltage = 0;
float med_set_voltage = 0;

int med_switch_duty = 0;
int hv_switch_duty = 0;

void setup()  {
  Serial.begin(115200);
  
  pinMode(2,OUTPUT); //X
  pinMode(4,OUTPUT); //Y
  pinMode(7,OUTPUT); //FOC
  pinMode(3,OUTPUT);
  pinMode(11,OUTPUT);
  TCCR2B = (TCCR2B & 0b11111000) | 0x01;//pin 3
//    TCCR2B = (TCCR2B & 0b11111000) | 0x02;//pin 3

  analogWrite(11,hv_switch_duty);
  analogWrite(3,med_switch_duty);
  digitalWrite(2,HIGH);
  digitalWrite(4,LOW);
  digitalWrite(7,LOW);
  delay(100);
  dac.begin(0x61);
}

void calc_voltages(){
  if(int(analogRead(A0)*1.044) > int(med_set_voltage)){
    med_switch_duty--;
  }
  else{
    med_switch_duty++;
  }
  med_switch_duty = constrain(med_switch_duty,0,160);
  analogWrite(3,med_switch_duty);
  
  hv_switch_duty = constrain(hv_switch_duty,0,20);
  analogWrite(11,hv_switch_duty);
}
String input = "";
void loop() {
  if(Serial.available()){
    input = Serial.readStringUntil('\n');
    if(input[0] == 'E'){
      hv_switch_duty = 0;
      analogWrite(11,hv_switch_duty);
      med_switch_duty = 0;
      analogWrite(3,med_switch_duty);
      hv_set_voltage = 0;
      med_set_voltage = 0;
    }
    if(input[0] == 'H'){
      hv_switch_duty = input.substring(1).toInt();
    }
    if(input[0] == 'M'){
      med_set_voltage = input.substring(1).toInt();
    }
  }
//  digitalWrite(2,HIGH);
//  digitalWrite(4,LOW);
//  digitalWrite(7,LOW);
//  uint32_t counter;
//  for (counter = 0; counter < 4090; counter+=5)
//  {
//    dac.setVoltage(counter, false);
//    calc_voltages();
//  }
//  dac.setVoltage(2048, false);
//  digitalWrite(2,LOW);
//  digitalWrite(4,HIGH);
//  digitalWrite(7,LOW);
//   for (counter = 0; counter < 4090; counter+=5)
//  {
//    dac.setVoltage(counter, false);
//    calc_voltages();
//  }
//  dac.setVoltage(2048, false);
//  digitalWrite(2,LOW);
//  digitalWrite(4,LOW);
//  digitalWrite(7,HIGH);
//  for (counter = 0; counter < 4090; counter+=5)
//  {
//    dac.setVoltage(counter, false);
//    calc_voltages();
//  }
   calc_voltages();
   //Serial.println("Med.V: " + String(analogRead(A0)*1.044) + " Med. Duty: " + med_switch_duty);
   Serial.println("High.V: " + String(analogRead(A2)) + " High. Duty: " + hv_switch_duty);
   delay(100);
}
