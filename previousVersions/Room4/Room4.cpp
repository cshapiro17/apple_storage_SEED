#include "Room3.h"
#include <Arduino.h>
#include "DFRobot_OxygenSensor.h"
using namespace std;


// Default constructor
Room3::Room3() : roomNum(0), appleType() {
}


Room3::Room3(int roomNum, Apple appleType) {
  setRoomNum(roomNum);
	setAppleType(appleType);
  
  boolean active = false;
} 

//Activates the room so we know if in use
void Room3::activate(){
    active = true;
}

//Deactivates the room so we know if in use
void Room3::deactivate(){
    active = false;
}

/*evaluate is used to measure the value of the potentiometer and determine if a
 * solenoid should be changed or not.
 * Inputs: potentiometer value, matrix used for on board LEDs
 * Output: Boolean true or false to mimick solenoid 
 */
int Room3::evaluateRoom(float oxygenValue, float carbonValue){

  // Variables to hold the state of the room
  int carbonLevel;
  int oxygenLevel;

  // Set values to compare to the sensor values
  float highMargin = 3.5;
  float lowMargin = 2.5;

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
	//stay the same
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
	//stay the same
  }

  // Decide what changes to make to the room
  // 1 = ON & 0 = OFF
  
  // Homeostasis ***************************************************
  if(oxygenLevel == 0 && carbonLevel == 0){
	  setN2solState(0);
    setO2solState(0);
  }
  //O2 good, CO2 too low ******************************************
  else if(oxygenLevel == 0 && carbonLevel == 1){
	  //flag that CO2 is too low? what can we do ???
    setN2solState(0);
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

/*solenoidChange opens the solenoid to allow the pump to turn on into the sensing unit
* Inputs: boolean solenoid, whether the valve should be on or off
* Outputs: displays to serial monitor whether room solenoid is open or closed
*/
void Room3::nitrogenSolenoidChange(){

  // Get the state of the nitrogen solenoid for the room
  int solState = getN2solState();

  // Make change to solenoid according to solenoid state
  digitalWrite(N2sol.N2pin, solState);

  /*
  if(solState == 1){

    if(!digitalRead(_solenoidN2)){
      Serial.print("Room ");
      Serial.print(_roomNum);
      Serial.println("Nitrogen open.");
      delay(10);
    }
    
    digitalWrite(_nitrogenSolenoidPin,HIGH);
  }
  else if (solState == 0){

    if(digitalRead(_solenoidN2)){
      Serial.print("Room ");
      Serial.print(_roomNum);
      Serial.println("Nitrogen closed.");
      delay(10);
    }
    
    digitalWrite(_nitrogenSolenoidPin,LOW);
  }
  */
  
  delay(500);
}

void Room3::oxygenSolenoidChange() {
  
  // Get the state of the oxygen solenoid for the room
  int solState = getO2solState();

  // Make change to solenoid according to solenoid state
  digitalWrite(O2sol.O2pin, solState);

  /*
  if(solState == 1){

    if(!digitalRead(_solenoidO2)){
      Serial.print("Room ");
      Serial.print(_roomNum);
      Serial.println("Atmosphere open");
      delay(10);
    }
    
    digitalWrite(_solenoidO2,HIGH);
  }
  else{
    if(digitalRead(_solenoidO2)){
      Serial.print("Room ");
      Serial.print(_roomNum);
      Serial.println("Atmosphere closed");
      delay(10);
    }
    digitalWrite(_solenoidO2,LOW);
  }
  */
  
  delay(500);
}

/*isActive returns whether the room is active or not
* Inputs: N/A
* Outputs: boolean true/false
*/
boolean Room3::isActive(){
    return active;
}

// Setter for room number
void Room3::setRoomNum(int RoomNum){
	roomNum = RoomNum;
}

// Getter for room number
int Room3::getRoomNum() {
  return roomNum;
}

// Setter for the oxygen input solenoid pin
void Room3::setOxygenSolenoidPin(int oxygenSolenoidPin){
	oxygenSolenoidPin = oxygenSolenoidPin;
}

// Getter for the oxygen solenoid pin
int Room3::getOxygenSolenoidPin() {
  return O2sol.O2pin;
}

// Setter for the nitrogen input solenoid pin
void Room3::setNitrogenSolenoidPin(int nitrogenSolenoidPin){
	N2sol.N2pin = nitrogenSolenoidPin;
}

// Getter for the nitrogen input solenoid pin
int Room3::getNitrogenSolenoidPin() {
  return N2sol.N2pin;
}

// Setters for solenoid states
void Room3::setN2solState(int desiredN2solState) {
  N2sol.N2state = desiredN2solState;
}

void Room3::setO2solState(int desiredO2solState) {
  O2sol.O2state = desiredO2solState;
}

// Getters for solenoid states
int Room3::getN2solState() {
  return N2sol.N2state;
}

int Room3::getO2solState() {
  return O2sol.O2state;
}

void Room3::setAppleType(Apple apple) {
  appleType = apple;
}

Apple Room3::getAppleType() {
  return appleType;
}
