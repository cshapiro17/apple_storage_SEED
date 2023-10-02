//This code is for testing the potentiometer and logging into to the console. Two LEDs are used, turning on for different potentiometer
//input values. Pins and values can be changed as this is preliminary code


#include "Arduino.h"

#define DELAY 30000 //milliseconds

static unsigned int state;
static unsigned long time;
int LED1 = 5;
int LED2 = 6;
int potent = 1; //placeholder
int potValue;
boolean solenoid;

void setup() {
  // pin classifications
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(potent,INPUT);

  Serial.begin(9600); //Initialize serial communication with a baud rate of 9600
  time = 0;
  state = 1;
}

void loop() {
  // Initial stuff
  switch state
  {
    //Restart State
    case 1:
      //time = millis();
      state = 2;
  
      break;
    
    //Sense State
    case 2:
      Delay(1000);
      potValue = analogRead(potent);
      state = 3;

      break;
    
    //Log value State
    case 3:
      Serial.println("Potentiometer Value: " + potValue);
      state = 4;


      break;
    
    //Evaluate Value State
    case 4:
      if(potValue>1000){
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        solenoid = true;
      }
      else if(potValue<100){
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        solenoid = true;
      }
      else{
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        solenoid = false;
      }
      state = 5;

      break;
    
    //Solenoid mimicking state (LEDs for now)
    case 5:
      if(solenoid == true){
        //solenoid opens
        
      }
      else if(solenoid == false){
        //solenoid closes
        
      }
      state = 2;

      break;
    
}

