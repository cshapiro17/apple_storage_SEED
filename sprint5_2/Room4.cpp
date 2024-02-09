#include "Room4.h"
#include <Arduino.h>
#include "DFRobot_OxygenSensor.h"
using namespace std;


// Default constructor
Room4::Room4() : roomNum(0), appleType() {
}


// Constructor
Room4::Room4(int roomNum, Apple appleType) {
  setRoomNum(roomNum);
	setAppleType(appleType);
  
  boolean active = false;
} 

/* Function which sets the room to active
 * Takes in nothing
 * Returns nothing
 */
void Room4::activate(){
    active = true;
}

/* Function which sets the room to not active
 * Takes in nothing
 * Returns nothing
 */
void Room4::deactivate(){
    active = false;
}

/* Evaluate is used to measure the value of the sensor readings and determine if a
 * solenoid should be changed or not
 * Takes in the oxygen and carbon dioxide readings
 * Returns nothing
 */
int Room4::evaluateRoom(float oxygenValue, float carbonValue){

  // Variables to hold the state of the room
  int carbonLevel;
  int oxygenLevel;

  // Set values to compare to the sensor values
  float highMargin = appleType.getOxygenHighMargin();
  float lowMargin = appleType.getOxygenLowMargin();

  // Evaluate room conditions for carbon dioxide levels
  //0 = neutral, 1 = too low, 2 = too high, 3 = almost good?
  if(carbonValue > appleType.getOxygenHighLevel()){
    carbonLevel = 2;
  }
  else if(carbonValue < appleType.getOxygenLowLevel()){
	carbonLevel = 1;
  }
  else if(carbonValue < appleType.getOxygenHighMargin() && carbonValue > appleType.getOxygenLowMargin()){
	carbonLevel = 0;
  }
  else{
	// do nothing
  }
  
  // Evaluate room conditions for oxygen levels
  //0 = neutral, 1 = too low, 2 = too high, 3 = almost good?
  if(oxygenValue > appleType.getOxygenHighLevel()){
    oxygenLevel = 2;
  }
  else if(oxygenValue< appleType.getOxygenLowLevel()){
	  oxygenLevel = 1;
  }
  else if(oxygenValue < appleType.getOxygenHighMargin() && oxygenValue > appleType.getOxygenLowMargin()){
	  oxygenLevel = 0;
  }
  else{
	// do nothing
  }

  // Decide what changes to make to the room (0 = closed, 1 = open)
  // Homeostasis ***************************************************
  if(oxygenLevel == 0 && carbonLevel == 0){
	  setN2solState(0);
    setO2solState(0);
  }
  //O2 good, CO2 too low ******************************************
  else if(oxygenLevel == 0 && carbonLevel == 1){
	  //flag that CO2 is too low? what can we do ???
    setN2solState(1);
    setO2solState(0);
  }
  //O2 good, CO2 too high *****************************************
  else if(oxygenLevel == 0 && carbonLevel == 2){
	  //turn on nitrogen solenoid
    setN2solState(1);
    setO2solState(0);
  }
  //O2 too low, CO2 good ******************************************
  else if(oxygenLevel == 1 && carbonLevel == 0){
    //turn on atmosphere solenoid
    setN2solState(0);
    setO2solState(1);
  }
   //O2 too low, CO2 too low ***************************************
  else if(oxygenLevel == 1 && carbonLevel == 1){
	  //turn on atmosphere solenoid ???
    setN2solState(0);
    setO2solState(1);
  } 
  //O2 too low, CO2 too high ***************************************
  else if(oxygenLevel == 1 && carbonLevel == 2){
	  //turn on atmosphere solenoid
    setN2solState(0);
    setO2solState(1);
  }
   //O2 too high, CO2 good *****************************************
  else if(oxygenLevel == 2 && carbonLevel == 0){
	  //turn on nitrogen solenoid
    setN2solState(1);
    setO2solState(0);
  } 
  //O2 too high, CO2 too low ****************************************
  else if(oxygenLevel == 2 && carbonLevel == 1){
	  //turn on both solenoids ????
    setN2solState(1);
    setO2solState(1);
  }
  //O2 too high, CO2 too high ***************************************
  else if(oxygenLevel == 2 && carbonLevel == 2){
	  //turn on both solenoids ????
    setN2solState(1);
    setO2solState(1);
  }
}

/* Function which controls the state of the nitrogen solenoid
 * Takes in nothing
 * Returns nothing
*/
void Room4::nitrogenSolenoidChange(){

  // Get the state of the nitrogen solenoid for the room
  int solState = getN2solState();

  // Make change to solenoid according to solenoid state
  digitalWrite(N2sol.N2pin, solState);
  
  delay(500);
}

/* Function which controls the state of the oxygen solenoid/fan
 * Takes in nothing
 * Returns nothing
 */
void Room4::oxygenSolenoidChange() {
  
  // Get the state of the oxygen solenoid for the room
  int solState = getO2solState();

  // Make change to solenoid according to solenoid state
  digitalWrite(O2sol.O2pin, solState);
  
  delay(500);
}

/*isActive returns whether the room is active or not
* Inputs: N/A
* Outputs: boolean true/false
*/
boolean Room4::isActive(){
    return active;
}

/* Setter for room number
 * Takes in a number which represents the room number 
 * Returns nothing
 */
void Room4::setRoomNum(int RoomNum){
	roomNum = RoomNum;
}

/* Setter for the oxygen input solenoid pin
 * Takes in a pin which connects to the oxygen solenoid/fan for the room
 * Returns nothing
 */
void Room4::setOxygenSolenoidPin(int oxygenSolenoidPin){
	O2sol.O2pin = oxygenSolenoidPin;
}

/*  Setter for the nitrogen input solenoid pin
 *  Takes in a pin which connects to the nitrogen solenoid for the room
 *  Returns nothing
 */
void Room4::setNitrogenSolenoidPin(int nitrogenSolenoidPin){
	N2sol.N2pin = nitrogenSolenoidPin;
}

/* Setter for nitrogen solenoid states
 * Takes in the desired state of the solenoid
 * Returns nothing
 */
void Room4::setN2solState(int desiredN2solState) {
  N2sol.N2state = desiredN2solState;
  digitalWrite(N2sol.N2pin,N2sol.N2state);
}

/* Setter for oxygen solenoid states
 * Takes in the desired state of the solenoid
 * Returns nothing
 */
void Room4::setO2solState(int desiredO2solState) {
  O2sol.O2state = desiredO2solState;
  digitalWrite(O2sol.O2pin,O2sol.O2state);
}

/* Sets the apple type for the room
 * Takes in an apple object representing the apple which is in the room
 * Returns nothing
 */
void Room4::setAppleType(Apple apple) {
  appleType = apple;
}

/*
 * Setter for solenoid pin
 * Takes in an integer containing pin which controls solenoid
 * Returns nothing
 */
void Room4::setSensingSolenoidPin(int solPin) {
  senseSol.sensePin = solPin;
}

/* Setter for sensing solenoid state
 * Takes in an integer containing the state of the solenoid (0 = closed, 1 = open)
 * Return nothing
 */
void Room4::setSenseSolState(int senseState) {
  senseSol.senseState = senseState;
}

/* Getter for the nitrogen input solenoid pin
 * Takes in nothing
 * Returns the pin number from the struct as an int
 */
int Room4::getNitrogenSolenoidPin() {
  return N2sol.N2pin;
}

/* Getter for the nitrogen solenoid state
 * Takes in nothing
 * Returns the state of the solenoid as an int (0 is closed, 1 is open)
 */
int Room4::getN2solState() {
  return N2sol.N2state;
}

/* Getter for the oxygen solenoid state
 * Takes in nothing
 * Returns the state of the solenoid as an int (0 is closed, 1 is open)
 */
int Room4::getO2solState() {
  return O2sol.O2state;
}

/* Getter for the type of apple in the room
 *  Takes in nothing
 *  Returns the type of apple as an apple object
 */
Apple Room4::getAppleType() {
  return appleType;
}

/* Getter for the oxygen solenoid pin
 * Takes in nothing
 * Returns the pin which connects to the oxygen solenoid of the room
 */
int Room4::getOxygenSolenoidPin() {
  return O2sol.O2pin;
}

/* Getter for room number
 *  Takes in nothing
 *  Returns the number of the room
 */
int Room4::getRoomNum() {
  return roomNum;
}

/*
 * Getter for the pin which controls the solenoid at the pump
 * Takes in nothing 
 * Returns the pin as an integer
 */
int Room4::getSensingSolenoidPin() {
  return senseSol.sensePin;
}

/* Getter for the state of the sensing solenoid
 * Takes in nothing
 * Returns the state as an integer
 */
int Room4::getSenseSolState() {
  return senseSol.senseState;
}
