#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;
int state = 0;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  matrix.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(state == 12){
    state = 0;
  }
  stateDisplay(state);
  matrix.renderBitmap(frame, 8, 12);
  delay(1000);
  state++;
}

void stateDisplay(int state){
  for (int i = 0; i < 12; i++) {
    frame[0][i] = 0;
  }
  frame[0][state-1] = 1;
}

void roomDisplay(int room, int LEDA, int LEDB){
  for (int i = 0; i < 12; i++) {
    frame[9-room][i] = 0;
  }
  frame[9-room][0] = LEDA;
  frame[9-room][0] = LEDB;
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
    roomDisplay(room,0,0);
    solenoid = false;
  }
  else{
    return previousSolenoid;
  }
  return solenoid;
}