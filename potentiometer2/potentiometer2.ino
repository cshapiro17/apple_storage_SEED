//This is an extension of potentiometer.ino This senses for X amount of time 
//before sensing again

//#include "Arduino.h"
#include "MillisTimerLib.h"

unsigned long previousMillis = 0;
int interval = 5000;

static unsigned int state;
static unsigned long time;
int LED1 = 5;                 //LED1 digital output pin
int LED2 = 6;                 //LED2 digital output pin
int LED3 = 7;                 //LED3 digital output pin
int LED4 = 8;                 //LED4 digital output pin
int potent1 = A0;             //input analog pin for potentiometer 1
int potent2 = A1;             //input analog pin for potentiometer 2
int solenoidPin1 = 4;         //solenoid 1 input pin
int solenoidPin2 = 3;         //solenoid 1 input pin
int room1;
int room2;
int highLevel;                //high level for sensing
int lowLevel;                 //low level for sensing
boolean solenoid1;
boolean solenoid2;
int potValue1;                 //analog value for potentiometer 1
int potValue2;                 //analog value for potentiometer 2

void setup() {
  // pin classifications
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(solenoidPin1,OUTPUT);
  pinMode(solenoidPin2,OUTPUT);
  Serial.begin(9600); //Initialize serial communication with a baud rate of 9600
  delay(500);
  state = 1;
}

void loop() {
  unsigned long currentMillis = millis();

  // Initial stuff
  switch(state){
    //Restart State
    case 1:
      highLevel = 700;
      lowLevel = 300;
      room1 = 1;
      room2 = 2;
      delay(1000);
      state = 2;
  
      break;
    
    //Sense State
    case 2:
      //Serial.println("Reading Potentiometer Value:");
      if ((unsigned long)(currentMillis - previousMillis) >= interval) {
      previousMillis = currentMillis;
      potValue1 = analogRead(potent1);
      potValue2 = analogRead(potent2);
      //Serial.println(potValue);
      state = 3;
      }
      break;
    
    //Log value State
    case 3:
      //Serial.println(potValue1);
      //delay(1000);
      logValue(room1,potValue1);
      logValue(room2,potValue2);
      state = 4;
      break;
    
    //Evaluate Value State
    case 4:
      solenoid1 = evaluateSensor(potValue1,highLevel,lowLevel,solenoid1,LED1,LED2);
      solenoid2 = evaluateSensor(potValue2,highLevel,lowLevel,solenoid2,LED3,LED4);      
      state = 5;
      break;
    
    //Solenoid mimicking state
    case 5:      
      solenoidChange(solenoid1,room1,solenoidPin1);
      solenoidChange(solenoid2,room2,solenoidPin2);
      state = 2;
      break;
  }
}


void logValue(int room, int potValue){
  //Serial.print(now.hour());
  //Serial.print(":");
  //Serial.print(now.minute());
  //Serial.print("\t");
  Serial.print(room);
  Serial.print(",");
  Serial.println(potValue);
}


//this function takes in a boolean to see if the solenoid should be open or closed, 
//which room to mimick, and which pin that solenoid changes.
void solenoidChange(boolean solenoid,int room, int solenoidPin){
  if(solenoid == true){
    if(!digitalRead(solenoidPin)){
    Serial.print("Room ");
    Serial.print(room);
    Serial.println("open");
    delay(10);
    }
    digitalWrite(solenoidPin,HIGH);
    
  }
  else{
    if(digitalRead(solenoidPin)){
      Serial.print("Room ");
      Serial.print(room);
      Serial.println("closed");
      delay(10);
    }
    digitalWrite(solenoidPin,LOW);

  }
  delay(500);
}

/*evaluateSensor is used to measure the value of the potentiometer and determine if a
 * solenoid should be changed or not.
 * Inputs: potentiometer value, high level threshold, low level threshold, 2 LEDs to mimick
 * Output: Boolean true or false to mimick solenoid 
 */
boolean evaluateSensor(int potValue, int highLevel, int lowLevel, boolean previousSolenoid, int LEDA, int LEDB){
  boolean solenoid = previousSolenoid;
  int highMargin = 550;       //high level it has to come back to in order to close valve
  int lowMargin = 450;        //low level it has to come back to in order to close valve

  //Serial.println("Evaluating Potentiometer Value");
  if(potValue>highLevel){
    digitalWrite(LEDA,HIGH);
    digitalWrite(LEDB,LOW);
    solenoid = true;
  }
  else if(potValue<lowLevel){
    digitalWrite(LEDA,LOW);
    digitalWrite(LEDB,HIGH);
    solenoid = true;
  }
  else if(potValue<highMargin && potValue>lowMargin){
    digitalWrite(LEDA,HIGH);
    digitalWrite(LEDB,HIGH);
    solenoid = false;
  }
  else{
    return previousSolenoid;
  }
  return solenoid;
}
