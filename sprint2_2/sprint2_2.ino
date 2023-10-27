//Code for sprint 2 goal -- mimicking air through a pump into a enclosed environment to test it

#include "Arduino_LED_Matrix.h"
#include "MillisTimerLib.h"
#include "Room.h"
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
int pumpPin1 = 2;                //pump 1 output pin

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
int roomNum1 = 1;                //room number
int highLevel = 600;          //high level threshold
int lowLevel = 400;           //low level threshold
boolean pumpOnOff;
Room room1(roomNum1,600,400,solenoidPin1,solenoidPin2,pumpPin1);

void setup() {
  // pin classifications
  // pinMode(LED1,OUTPUT);
  // pinMode(LED2,OUTPUT);
  // pinMode(LED3,OUTPUT);
  // pinMode(LED4,OUTPUT);
  pinMode(solenoidPin1,OUTPUT);
  pinMode(solenoidPin2,OUTPUT);
  pinMode(pumpPin1,OUTPUT);
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
      room1.activate();
      delay(1000);
      state = 2;
    break;
    
    //pump in air state (open solenoid, turn on pump for X amount of time)
    case 2:
      if ((unsigned long)(currentMillis - previousMillis) >= interval){
        if(room1.isActive()){
          room1.pumpChange(true);
          previousMillis = currentMillis;
          state = 3;
        }
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
      room1.pumpChange(false);
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
        solenoid2 = room1.evaluateRoom(potValue1,frame);

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
      room1.solenoidChange(solenoid2);
      previousMillis = currentMillis;
      state = 2;
    break;

    //(OPTION 2) close room state
  }
}

/*stateDisplay displays the current state of the FSM on first row of LED Matrix
*/
void stateDisplay(int state){
  for (int i = 0; i < 12; i++) {
    frame[0][i] = 0;
  }
  frame[0][state-1] = 1;
}
