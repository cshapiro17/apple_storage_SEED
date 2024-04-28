//this code is to try using the MQ135 for sensing. It reads values from the sensor and displays them in
//the serial monitor.
#include "MQ135.h";

int LED1 = 5;        //LED 1 pin
int sense1 = A3;     //analog pin for MQ135 sensor

void setup() {
    pinMode(LED1,OUTPUT);
    pinMode(sense1,INPUT);
}

void loop() {

}