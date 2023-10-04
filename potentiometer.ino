//This code is for testing the potentiometer and logging into to the console. Two LEDs are used, turning on for different potentiometer
//input values. Pins and values can be changed as this is preliminary code


#include "Arduino.h"
//#include "millisDelay.h"

static unsigned int state;
static unsigned long time;
int LED1 = 5;                 //LED1 digital output pin
int LED2 = 6;                 //LED2 digital output pin
int potent = A3;              //input analog pin for potentiometer
int solenoidPin = 4;            //solenoid input pin
int potValue;             

int highLevel;                //high level for sensing
int lowLevel;                 //low level for sensing
boolean solenoid;
boolean newData = false;
void setup() {
  // pin classifications
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  Serial.begin(9600); //Initialize serial communication with a baud rate of 9600
  state = 1;
}

void loop() {
  // Initial stuff
  switch(state){
    //Restart State
    case 1:
      //Serial.println(state);
      
//      Serial.println("High Margin: ");
//      while (Serial.available() == 0) {
//      }
//      highLevel = Serial.parseInt();
//      delay(1000);
//      
//      Serial.println("Low Margin: ");
//      while (Serial.available() == 0 && newData == false) {
//        Serial.println("1");
//        newData = true;
//      }
//      lowLevel = Serial.parseInt();
      highLevel = 600;
      lowLevel = 300;
      delay(1000);
      state = 2;
  
      break;
    
    //Sense State
    case 2:
      delay(500);
      Serial.println(state);
      potValue = analogRead(potent);
      //Serial.println(potValue);
      state = 3;

      break;
    
    //Log value State
    case 3:
      Serial.println(potValue);
      //delay(1000);
      state = 4;


      break;
    
    //Evaluate Value State
    case 4:
      Serial.println(state);
      if(potValue>highLevel){
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        solenoid = true;
      }
      else if(potValue<lowLevel){
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        solenoid = true;
      }
      else{
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,HIGH);
        solenoid = false;
      }
      
      state = 5;
      break;
    
    //Solenoid mimicking state (LEDs for now)
    case 5:
      //open or close solenoid depending on state of potentiometer
      if(solenoid == true){
        digitalWrite(solenoidPin,HIGH);
        Serial.println("Solenoid Open");
      }
      else{
        digitalWrite(solenoidPin,LOW);
        Serial.println("Solenoid Closed");
      }
      delay(500);
      state = 2;

      break;
  }
}

