// M7
// Cpp for Room class

/*
  Code for the control of the room objects. Has getters, setters, and other functions which 
  define the variables for room objects.
*/

#include "Room.h"
#include <Arduino.h>
using namespace std;


// Default constructor
Room::Room() : roomNum(0), appleType() {
}


// Constructor
Room::Room(String roomName, int roomNum, int volume, Apple appleType, int o2FanPin, int n2SolenoidPin, int senseSolenoidPin, int o2FanState, int n2SolenoidState, int senseSolenoidState) {
  setRoomName(roomName);
  setRoomNum(roomNum);
  setRoomVolume(volume);
	setAppleType(appleType, "Default");
  setOxygenFanPin(o2FanPin);
  setNitrogenSolenoidPin(n2SolenoidPin);
  setSensingSolenoidPin(senseSolenoidPin);
  setO2fanState(o2FanState);
  setN2solState(n2SolenoidState);
  setSenseSolState(senseSolenoidState);
  
  bool active = false;
  bool underTest = false;
} 

/* Function which sets the room to active
 * Takes in nothing
 * Returns nothing
 */
void Room::activate(){
    active = true;
}

/* Function which sets the room to not active
 * Takes in nothing
 * Returns nothing
 */
void Room::deactivate(){
    active = false;

    // Set all solenoids and fans to be off since the room is not active
    setO2fanState(0);
    setN2solState(0);
    setSenseSolState(0);
}

/* Function which sets the room to be under test
 * Takes in nothing
 * Returns nothing
 */
void Room::setUnderTest(bool testingState) {
  underTest = testingState;
}

/* Function which returns whether or not the room is under test
 * Takes in nothing
 * Returns the testing state of the room
 */
bool Room::isUnderTest() {
  return underTest;
}

/* Evaluate is used to measure the value of the sensor readings and determine if a
 * solenoid should be changed or not
 * Takes in the oxygen and carbon dioxide readings
 * Returns nothing
 */
int Room::evaluateRoom(float oxygenValue, float carbonValue){

  // Variables to hold the state of the room
  int oxygenLevel;

  // Set values to compare to the sensor values
  float highMargin = appleType.getOxygenHighMargin();
  float lowMargin = appleType.getOxygenLowMargin();
  
  // Evaluate room conditions for oxygen levels
  //0 = neutral, 1 = too low, 2 = too high, 3 = almost good?
  if(oxygenValue > appleType.getOxygenHighLevel()){
    oxygenLevel = 2;
  }
  else if(oxygenValue < appleType.getOxygenLowLevel()){
	  oxygenLevel = 1;
  }
  else if(oxygenValue <= appleType.getOxygenHighMargin() && oxygenValue >= appleType.getOxygenLowMargin()){
	  oxygenLevel = 0;
  }
  else{
	  // Do nothing
  }

  // Decide what changes to make to the room (0 = closed, 1 = open)
  // Homeostasis ***************************************************
  if(oxygenLevel == 0){
	  setN2solState(0);
    setO2fanState(0);
  }
  //O2 too low, CO2 good ******************************************
  else if(oxygenLevel == 1){
    //turn on atmosphere solenoid
    setN2solState(0);
    setO2fanState(1);
  }
   //O2 too high, CO2 good *****************************************
  else if(oxygenLevel == 2){
	  //turn on nitrogen solenoid
    setN2solState(1);
    setO2fanState(0);
  } 
}

/*isActive returns whether the room is active or not
* Inputs: N/A
* Outputs: boolean true/false
*/
boolean Room::isActive(){
    return active;
}

/* Setter for room number
 * Takes in a number which represents the room number 
 * Returns nothing
 */
void Room::setRoomNum(int RoomNum){
	roomNum = RoomNum;
}

/* Setter for room volume
 * Takes in an integer representing the volume of the room in cubic feet
 * Returns nothing
 */
void Room::setRoomVolume(int volume) {
  roomVolume = volume;
}

/* Setter for the oxygen input solenoid pin
 * Takes in a pin which connects to the oxygen solenoid/fan for the room
 * Returns nothing
 */
void Room::setOxygenFanPin(int OxygenFanPin){
	O2fan.O2pin = OxygenFanPin;
}

/*  Setter for the nitrogen input solenoid pin
 *  Takes in a pin which connects to the nitrogen solenoid for the room
 *  Returns nothing
 */
void Room::setNitrogenSolenoidPin(int nitrogenSolenoidPin){
	N2sol.N2pin = nitrogenSolenoidPin;
}

/* Setter for nitrogen solenoid states
 * Takes in the desired state of the solenoid
 * Returns nothing
 */
void Room::setN2solState(int desiredN2solState) {
  N2sol.N2state = desiredN2solState;

  // Drive the solenoid to the desired state
  digitalWrite(N2sol.N2pin,N2sol.N2state);
}

/* Setter for oxygen solenoid states
 * Takes in the desired state of the solenoid
 * Returns nothing
 */
void Room::setO2fanState(int desiredO2fanState) {
  O2fan.O2state = desiredO2fanState;

  // Drive the solenoid to the desired state
  digitalWrite(O2fan.O2pin,O2fan.O2state);
}

/* Sets the apple type for the room
 * Takes in an apple object representing the apple which is in the room
 * Returns nothing
 */
void Room::setAppleType(Apple apple, String apple_str) {
  appleType = apple;
  currentApple = apple_str;
}

/*
 * Setter for solenoid pin
 * Takes in an integer containing pin which controls solenoid
 * Returns nothing
 */
void Room::setSensingSolenoidPin(int solPin) {
  senseSol.sensePin = solPin;
}

/* Setter for sensing solenoid state
 * Takes in an integer containing the state of the solenoid (0 = closed, 1 = open)
 * Return nothing
 */
void Room::setSenseSolState(int senseState) {
  senseSol.senseState = senseState;

  // Turn on or off the solenoid accordingly
  digitalWrite(senseSol.sensePin, senseSol.senseState);
}

/* Setter for the name of the room
 * Takes in a string of the name of the room 
 * Returns nothing
 */
void Room::setRoomName(String roomNameInput) {
  roomName = roomNameInput;
}

/* Getter for the nitrogen input solenoid pin
 * Takes in nothing
 * Returns the pin number from the struct as an int
 */
int Room::getNitrogenSolenoidPin() {
  return N2sol.N2pin;
}

/* Getter for the nitrogen solenoid state
 * Takes in nothing
 * Returns the state of the solenoid as an int (0 is closed, 1 is open)
 */
int Room::getN2solState() {
  return N2sol.N2state;
}

/* Getter for the oxygen solenoid state
 * Takes in nothing
 * Returns the state of the solenoid as an int (0 is closed, 1 is open)
 */
int Room::getO2fanState() {
  return O2fan.O2state;
}

/* Getter for the type of apple in the room
 *  Takes in nothing
 *  Returns the type of apple as an apple object
 */
Apple Room::getAppleType() {
  return appleType;
}

/* Getter for the oxygen solenoid pin
 * Takes in nothing
 * Returns the pin which connects to the oxygen solenoid of the room
 */
int Room::getOxygenFanPin() {
  return O2fan.O2pin;
}

/* Getter for room number
 *  Takes in nothing
 *  Returns the number of the room
 */
int Room::getRoomNum() {
  return roomNum;
}

/* Getter for room volume
 * Takes in nothing
  * Returns the volume of the room
  */
int Room::getRoomVolume() {
  return roomVolume;
}

/*
 * Getter for the pin which controls the solenoid at the pump
 * Takes in nothing 
 * Returns the pin as an integer
 */
int Room::getSensingSolenoidPin() {
  return senseSol.sensePin;
}

/* Getter for the state of the sensing solenoid
 * Takes in nothing
 * Returns the state as an integer
 */
int Room::getSenseSolState() {
  return senseSol.senseState;
}

/* Getter for the name of the room 
 * Takes in nothing
 * Returns the name of the room
 */
String Room::getRoomName() {
  return roomName;
}

/* Function which gets the name of the apple being stored
 * Takes in nothing
 * Returns the name of the apple
 */
String Room::getCurrentApple() {
  return currentApple;
}