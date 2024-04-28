#include <Arduino.h>
#include "System.h"
using namespace std;

// Default values
System::System() : systemName("Apple Storage Control"), systemOn(false) {
  rooms.clear();
}

/*
 * Setter for system name
 * Takes in a string which represented the desired system name
 * Returns nothing
 */
void System::setSystemName(string nameInput) {
  systemName = nameInput;
}

/*
 * Setter for solenoid pin
 * Takes in an integer containing pin which controls solenoid
 * Returns nothing
 */
void System::setPumpSolenoidPin(int solPin) {
  boxPump.solenoidPin = solPin;
}

/*
 * Setter for pump pin
 * Takes in an integer containing pin which controls pump
 * Returns nothing
 */
void System::setPumpPin(int pumpPin) {
  boxPump.pin = pumpPin;
}

/*
 * Getter for system name
 * Takes in nothing
 * Returns the name of the system
 */
string System::getSystemName() {
  return systemName;
}

/*
 * Getter for the pin which controls the solenoid at the pump
 * Takes in nothing 
 * Returns the pin as an integer
 */
int System::getPumpSolenoidPin() {
  return boxPump.solenoidPin;
}

/*
 * Getter for the pin which controls the pump
 * Takes in nothing
 * Returns the pin as an integer
 */
int System::getPumpPin() {
  return boxPump.pin;
}

/*
 * Function which handles the button input from the dashboard
 * Takes in the button input as an integer
 * Calls appropriate function
 */
void System::handleButtonInput(int buttonInput) {
  // TODO: Fill this function
}

void System::toggleSystemOn() {
  // TODO: Fill this function
}

/*
 * Function which adds a room object to the system
 * Takes in a room object and adds to rooms vector
 * Returns nothing
 */
void System::addRoom(Room3 newRoom) {
  rooms.push_back(newRoom);
}

/*
 * Function which removes a room from the system given the room number
 * Takes in the number of the room (i.e. 1, 2, 3, 4,...)
 * Returns a boolean indicating whether it has been removed
 */
bool System::removeRoom(int roomNum) {
  if (roomNum < 0 || roomNum >= rooms.size()) {
    return false;
  }
  rooms.erase(rooms.begin() + roomNum + 1);
  return true;
}

/*pumpChange changes the pump that brings air in from that room to the sensing unit
*Inputs: boolean pumpSwitch, whether the pump should be turned on or off
*Outputs: rights to serial monitor whether the pump is on or off when called
*/
void System::pumpOn(boolean pumpSwitch){
  if(pumpSwitch){
    // Set solenoid to open and turn pump on
    digitalWrite(boxPump.solenoidPin, HIGH);
    digitalWrite(boxPump.pin, HIGH);

    boxPump.pumpState = 1;

    /*
    Serial.print("Pump ");
    Serial.print(_roomNum);
    Serial.println(" on for 10 Seconds.");
    */
    
    delay(10);
  }
  else{
    // Set solenoid to closed and turn pump off
    digitalWrite(boxPump.solenoidPin, LOW);
    digitalWrite(boxPump.pin, LOW);

    boxPump.pumpState = 0;

    /*
    Serial.print("Pump ");
    Serial.print(_roomNum);
    Serial.println(" off");
    */
    
    delay(10);
  }
}

// Function which is used to sense oxygen using the DFRobot_OxygenSensor
float System::senseOxygen(DFRobot_OxygenSensor oxygen, int collectData){
    return oxygen.getOxygenData(collectData);
}
