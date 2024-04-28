#include "Room3.h"
#include <Arduino.h>
#include "DFRobot_OxygenSensor.h"
//#include <Vector.h>

//void Room::Room(void){
	
//}

Room3::Room3(int roomNum, int highLevel, int lowLevel, int oxygenSolenoidPin,int nitrogenSolenoidPin,int pumpSolenoidPin, int pumpPin, int nitrogenSolenoidState, int oxygenSolenoidState) {
  setRoomNum(roomNum);
	setHighLevel(highLevel);
  setLowLevel(lowLevel);
  setPumpSolenoidPin(pumpSolenoidPin);
	setOxygenSolenoidPin(oxygenSolenoidPin);
	setNitrogenSolenoidPin(nitrogenSolenoidPin);
  setPumpPin(pumpPin);
  boolean _previousSolenoid = false;
  long _highMargin = (_highLevel+_lowLevel)/2 + (_highLevel-(_highLevel+_lowLevel)/2)/3;
  long _lowMargin = (_highLevel+_lowLevel)/2 - (_highLevel-(_highLevel+_lowLevel)/2)/3;
  boolean _active = false;
} 

//Activates the room so we know if in use
void Room3::activate(){
    _active = true;
}

//Deactivates the room so we know if in use
void Room3::deactivate(){
    _active = false;
}

float Room3::senseOxygen(DFRobot_OxygenSensor oxygen, int collectData){
    return oxygen.getOxygenData(collectData);
}

/*evaluate is used to measure the value of the potentiometer and determine if a
 * solenoid should be changed or not.
 * Inputs: potentiometer value, matrix used for on board LEDs
 * Output: Boolean true or false to mimick solenoid 
 */
int Room3::evaluateRoom(float oxygenValue, float carbonValue, int solenoid[2]){

  // Variables to hold the state of the room
  int carbonLevel;
  int oxygenLevel;
  
  //boolean solenoid = _previousSolenoid;
       //low level it has to come back to in order to close valve

  // Set values to compare to the sensor values
  float highMargin = 3.5;
  float lowMargin = 2.5;

  // Evaluate room conditions for carbon dioxide levels
  //0 = neutral, 1 = too low, 2 = too high, 3 = almost good?
  if(carbonValue > _highLevel){
    carbonLevel = 2;
  }
  else if(carbonValue < _lowLevel){
	carbonLevel = 1;
  }
  else if(carbonValue < highMargin && carbonValue > lowMargin){
	carbonLevel = 0;
  }
  else{
	//stay the same
  }
  
  // Evaluate room conditions for oxygen levels
  //0 = neutral, 1 = too low, 2 = too high, 3 = almost good?
  if(oxygenValue > _highLevel){
    oxygenLevel = 2;
  }
  else if(oxygenValue<_lowLevel){
	  oxygenLevel = 1;
  }
  else if(oxygenValue < highMargin && oxygenValue > lowMargin){
	  oxygenLevel = 0;
  }
  else{
	//stay the same
  }

  // Decide what changes to make to the room
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

/*pumpChange changes the pump that brings air in from that room to the sensing unit
*Inputs: boolean pumpSwitch, whether the pump should be turned on or off
*Outputs: rights to serial monitor whether the pump is on or off when called
*/
void Room3::pumpOn(boolean pumpSwitch){
  if(pumpSwitch){
    digitalWrite(_pumpSolenoidPin,HIGH);
    digitalWrite(_pumpPin,HIGH);

    /*
    Serial.print("Pump ");
    Serial.print(_roomNum);
    Serial.println(" on for 10 Seconds.");
    */
    
    delay(10);
  }
  else{
    digitalWrite(_pumpPin,LOW);
    digitalWrite(_pumpSolenoidPin,LOW);

    /*
    Serial.print("Pump ");
    Serial.print(_roomNum);
    Serial.println(" off");
    */
    
    delay(10);
  }
}

/*solenoidChange opens the solenoid to allow the pump to turn on into the sensing unit
* Inputs: boolean solenoid, whether the valve should be on or off
* Outputs: displays to serial monitor whether room solenoid is open or closed
*/
void Room3::nitrogenSolenoidChange(){
  //Nitrogen solenoid change

  // Get the state of the nitrogen solenoid for the room
  int solState = getN2solState();

  // Make change to solenoid according to solenoid state
  digitalWrite(_nitrogenSolenoidPin, solState);

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
  
  // Atmosphere solenoid change

  // Get the state of the oxygen solenoid for the room
  int solState = getO2solState();

  // Make change to solenoid according to solenoid state
  digitalWrite(_oxygenSolenoidPin, solState);

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
    return _active;
}

// Setter for room number
void Room3::setRoomNum(int RoomNum){
	_roomNum = RoomNum;
}

// Getter for room number
int Room3::getRoomNum() {
  return _roomNum;
}

// Setter for high level
void Room3::setHighLevel(int highLevel){
	_highLevel = highLevel;
}

// Getter for high level
int Room3::getHighLevel() {
  return _highLevel;
}

// Setter for low level
void Room3::setLowLevel(int lowLevel){
	_lowLevel = lowLevel;
}

// Getter for low level
int Room3::getLowLevel() {
  return _lowLevel;
}

// Setter for the pumps solenoid pin
void Room3::setPumpSolenoidPin(int pumpSolenoidPin){
	_pumpSolenoidPin = pumpSolenoidPin;
}

// Getter for the pumps solenoid pin
int Room3::getPumpSolenoidPin() {
  return _pumpSolenoidPin;
}

// Setter for the oxygen input solenoid pin
void Room3::setOxygenSolenoidPin(int oxygenSolenoidPin){
	_oxygenSolenoidPin = oxygenSolenoidPin;
}

// Getter for the oxygen solenoid pin
int Room3::getOxygenSolenoidPin() {
  return _oxygenSolenoidPin;
}

// Setter for the nitrogen input solenoid pin
void Room3::setNitrogenSolenoidPin(int nitrogenSolenoidPin){
	_nitrogenSolenoidPin = nitrogenSolenoidPin;
}

// Getter for the nitrogen input solenoid pin
int Room3::getNitrogenSolenoidPin() {
  return _nitrogenSolenoidPin;
}

// Setter for the pin for the pump into the measuring environment
void Room3::setPumpPin(int pumpPin){
	_pumpPin = pumpPin;
}

// Getter for the pin for the pump into the measuring environment
int Room3::getPumpPin() {
  return _pumpPin;
}

// Setters for solenoid states
void Room3::setN2solState(int desiredN2State) {
  _nitrogenSolenoidState = desiredN2State;
}

void Room3::setO2solState(int desiredO2State) {
  _oxygenSolenoidState = desiredO2State;
}

// Getters for solenoid states
int Room3::getN2solState() {
  return _nitrogenSolenoidState;
}

int Room3::getO2solState() {
  return _oxygenSolenoidState;
}
