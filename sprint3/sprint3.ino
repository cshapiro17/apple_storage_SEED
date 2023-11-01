/*MAIN Code for sprint 3 goal -- Sensing room able to pump in air from 2 rooms, along with sense room flushing capabilities
* to make sure sensor is reading values properly. Also able to read both O2 and CO2, making changes to both
*/
#include "Arduino_LED_Matrix.h"
#include "MillisTimerLib.h"
#include "Room.h"
#include "DFRobot_OxygenSensor.h"

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
int solenoidPin2 = 3;         //solenoid 2 output pin
int solenoidPin3 = 9;         //solenoid 3 output pin
int solenoidPin4 = 10;         //solenoid 4 output pin
//pumps
int pumpPin1 = 2;                //pump 1 output pin
int pumpPin2 = 11;                //pump 2 output pin

//**************************************
//Global Variables
//**************************************
unsigned long previousMillis = 0;
int interval = 10000;          //interval for state 2 delay
int interval2 = 10000;         //interval for state 3 delay
static unsigned int state;    //state variable
int potValue1;                //analog potentiometer value
//boolean solenoid1;
boolean roomSolenoid;         //boolean whether or not the rooms valve is open/closed
int roomNum1 = 1;                //room number
int roomNum2 = 2;                //room number
static int MAXROOMS = 1;
int highLevel = 600;          //high level threshold
int lowLevel = 400;           //low level threshold
float oxygenValue;
boolean pumpOnOff;
Room room[2]={Room(roomNum1,4,2,solenoidPin1,solenoidPin2,pumpPin1),Room(roomNum2,4,2,solenoidPin3,solenoidPin4,pumpPin2)};


int x = 0;                        //x is the RUT (Room under Test)

DFRobot_OxygenSensor oxygen;
#define collectNumber 10
#define Oxygen_IIC_Address ADDRESS_3

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
  while(!oxygen.begin(Oxygen_IIC_Address)){
    Serial.println("I2C device number error !");
    delay(1000);
  }
  Serial.println("I2C connect success");

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
      room[0].activate();
      room[1].activate();
      delay(1000);
      state = 2;
    break;
    
    //pump in air state (open solenoid, turn on pump for X amount of time)
    case 2:
      if ((unsigned long)(currentMillis - previousMillis) >= interval){
        if(room[x].isActive()){
          room[x].pumpChange(true);
          previousMillis = currentMillis;
          state = 3;
        }
        else if (x == MAXROOMS){
          x = 0;
        }
        else{
          x ++;
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
      room[x].pumpChange(false);
      state = 5;
    break;


    //Sense State of room pumped in
    case 5:
      potValue1 = analogRead(potent1);
      oxygenValue = oxygen.getOxygenData(collectNumber);
      state = 6;
    break;


    //log value State of room
    case 6:
      Serial.println(oxygenValue);
      Serial.println(potValue1);
      state = 7;
    break;

    //Evaluate state of room 
    case 7:
        roomSolenoid = room[x].evaluateRoom(potValue1,oxygenValue,frame);

    //mimick room state
    case 8:
      room[x].solenoidChange(roomSolenoid);
      if(x == MAXROOMS){
        x = 0;
      }
      else{
        x ++;
      }

      previousMillis = currentMillis;
      state = 2;
    break;

    //Pump in fresh air state
    case 9:
      //turn fresh air pump on


    break;
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
