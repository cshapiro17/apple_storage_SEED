//Code for sprint 2 goal -- mimicking air through a pump into a enclosed environment to test it

#include "Arduino_LED_Matrix.h"
#include "MillisTimerLib.h"
ArduinoLEDMatrix matrix;

uint8_t frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

//**************************************
//INPUTS
//**************************************
int potent1 = A0;             //input analog pin for potentiometer 1

//**************************************
//OUTPUTS
//**************************************
//LEDS
int LED1 = 5;                 //LED1 digital output pin
int LED2 = 6;                 //LED2 digital output pin
int LED3 = 7;                 //LED3 digital output pin
int LED4 = 8;                 //LED4 digital output pin
//solenoids
int solenoidPin1 = 4;         //solenoid 1 output pin
int solenoidPin2 = 3;         //solenoid 1 output pin
//pumps
int pump1 = 2;                //pump 1 output pin

//**************************************
//Global Variables
//**************************************
unsigned long previousMillis = 0;
int interval = 10000;          //interval for state 2 delay
int interval2 = 10000;         //interval for state 3 delay
static unsigned int state;    //state variable
int potValue1;                //analog potentiometer value
boolean solenoid1;
boolean solenoid2;
int room1 = 1;                //room number
int highLevel = 600;          //high level threshold
int lowLevel = 400;           //low level threshold
boolean pumpOnOff;

void setup() {
  // pin classifications
  // pinMode(LED1,OUTPUT);
  // pinMode(LED2,OUTPUT);
  // pinMode(LED3,OUTPUT);
  // pinMode(LED4,OUTPUT);
  pinMode(solenoidPin1,OUTPUT);
  pinMode(solenoidPin2,OUTPUT);
  pinMode(pump1,OUTPUT);
  pinMode(potent1,INPUT);
  Serial.begin(115200);
  matrix.begin();
  delay(500);
  state = 1;
}


void loop(){
  unsigned long currentMillis = millis();
  stateDisplay(state);
  matrix.renderBitmap(frame, 8, 12);

  switch(state){
    //reset state
    case 1:
      delay(1000);
      state = 2;
    break;
    
    //pump in air state (open solenoid, turn on pump for X amount of time)
    case 2:
      if ((unsigned long)(currentMillis - previousMillis) >= interval){
        pumpOnOff = true;
        solenoidChange(pumpOnOff,1,solenoidPin1);
        pumpChange(pumpOnOff,room1,pump1);
        previousMillis = currentMillis;
        state = 3;
      }
    break;

    //wait for pump to fill room
    case 3:
      if ((unsigned long)(currentMillis - previousMillis) >= interval2) {
        state = 4;
        previousMillis = currentMillis;
      }
    break;


    //turn off pump and solenoid
    case 4:
      pumpChange(false,room1,pump1);
      solenoidChange(false,1,solenoidPin1);
      state = 5;
    break;


    //Sense State of room pumped in
    case 5:
      potValue1 = analogRead(potent1);
      state = 6;
    break;


    //log value State of room
    case 6:
      Serial.println(potValue1);
      state = 7;
    break;

    //Evaluate state of room 
    case 7:
        solenoid2 = evaluateSensor(room1,potValue1,highLevel,lowLevel,solenoid2,LED1,LED2);

        //if value still outside of spec, keep room open or open room
        if(solenoid2){
          state = 8;
        }
        //if value within spec/back to middle, close room
        else{
          state = 8;
        }

    //(OPTION 1) mimick room state
    case 8:
      solenoidChange(solenoid2,room1,solenoidPin2);
      previousMillis = currentMillis;
      state = 2;
    break;

    //(OPTION 2) close room state
  }
}


//this function takes in a boolean to see if the solenoid should be open or closed, 
//which room to mimick, and which pin that solenoid changes.
void solenoidChange(boolean solenoid,int room, int solenoidPin){
  if(solenoid == true){
    if(!digitalRead(solenoidPin)){
    Serial.print("Room ");
    Serial.print(room);
    Serial.println(" open");
    delay(10);
    }
    digitalWrite(solenoidPin,HIGH);
    
  }
  else{
    if(digitalRead(solenoidPin)){
      Serial.print("Room ");
      Serial.print(room);
      Serial.println(" closed");
      delay(10);
    }
    digitalWrite(solenoidPin,LOW);
  }
  delay(500);
}


void pumpChange(boolean pumpSwitch, int room, int pumpPin){
  if(pumpSwitch){
    digitalWrite(pumpPin,HIGH);
    Serial.print("pump ");
    Serial.print(room);
    Serial.println(" on");
    delay(10);
  }
  else{
    digitalWrite(pumpPin,LOW);
    Serial.print("pump ");
    Serial.print(room);
    Serial.println(" off");
    delay(10);
  }
}

/*These functions are for displaying different variables on the onboard LED matrix for the
* Arduino R4 Wifi
*/
void roomDisplay(int room, int LEDA, int LEDB){
  for (int i = 0; i < 12; i++) {
    frame[8-room][i] = 0;
  }
  frame[8-room][0] = LEDA;
  frame[8-room][1] = LEDB;
}

void stateDisplay(int state){
  for (int i = 0; i < 12; i++) {
    frame[0][i] = 0;
  }
  frame[0][state-1] = 1;
}

/*evaluateSensor is used to measure the value of the potentiometer and determine if a
 * solenoid should be changed or not.
 * Inputs: potentiometer value, high level threshold, low level threshold, 2 LEDs to mimick
 * Output: Boolean true or false to mimick solenoid 
 */
boolean evaluateSensor(int room, int potValue, int highLevel, int lowLevel, boolean previousSolenoid, int LEDA, int LEDB){
  boolean solenoid = previousSolenoid;
  int highMargin = 550;       //high level it has to come back to in order to close valve
  int lowMargin = 450;        //low level it has to come back to in order to close valve

  //Serial.println("Evaluating Potentiometer Value");
  if(potValue>highLevel){
    roomDisplay(room,1,0);
    solenoid = true;
  }
  else if(potValue<lowLevel){
    roomDisplay(room,0,1);
    solenoid = true;
  }
  else if(potValue<highMargin && potValue>lowMargin){
    roomDisplay(room,1,1);
    solenoid = false;
  }
  else{
    return previousSolenoid;
  }
  return solenoid;
}


