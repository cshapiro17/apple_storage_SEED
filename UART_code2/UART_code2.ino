#include  <Arduino.h>
#include "CO2Sensor.h"


int count = 0;
CO2Sensor CO2Sensor;

int ppm;
float percent;
void setup() {
  Serial.begin(9600);                 //Serial Monitor 
  Serial1.begin(9600);                //CO2 sensor Serial RX0/TX0

  delay(1000);
}

void loop() {
  delay(2000);
  ppm = CO2Sensor.getPPM();
  percent = CO2Sensor.getPercent();
  
  Serial.print("CO2 Level: ");
  Serial.print(ppm);
  Serial.print("   ");
  Serial.println(percent);
}
