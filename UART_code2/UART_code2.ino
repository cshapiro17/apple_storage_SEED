#include  <Arduino.h>
#include "CO2Sensor.h"


int count = 0;
CO2Sensor myCO2Sensor;

int ppm;
float percent;
void setup() {
  Serial.begin(9600);                 //Serial Monitor 
  Serial1.begin(9600);                //CO2 sensor Serial RX0/TX0
  delay(2000);
  int count = 0;
  delay(1000);
}

void loop() {
  Serial.println(" ");
  if(count == 0){
    myCO2Sensor.initialize();
  }
  if(count == 10){
    myCO2Sensor.calibrate();
  }
  delay(2000);
  ppm = myCO2Sensor.getPPM();
  percent = myCO2Sensor.getPercent();
  
  Serial.print("CO2 Level: ");
  Serial.print(ppm);
  Serial.print("   ");
  Serial.println(percent);

  count ++;
}
