//This code is for testing the potentiometer and logging into to the console. Two LEDs are used, turning on for different potentiometer
//input values. Pins and values can be changed as this is preliminary code


#include "Arduino.h"
//#include "millisDelay.h"

static unsigned int state;
static unsigned long time;
int LED1 = 5;                 //LED1 digital output pin
int LED2 = 6;                 //LED2 digital output pin
int potent = A3;              //input analog pin for potentiometer
int potValue = 0;
boolean solenoid;

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
      Serial.println(state);
      state = 2;
  
      break;
    
    //Sense State
    case 2:
      delay(1000);
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
      if(potValue>700){
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        solenoid = true;
      }
      else if(potValue<400){
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
      Serial.println(state);
      if(solenoid == true){
        //solenoid opens
        
      }
      else if(solenoid == false){
        //solenoid closes
      }
      state = 2;

      break;
  }
}

