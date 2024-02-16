#include <Arduino.h>
#include "System.h"
using namespace std;

// Default constructor
System::System() : systemName("Apple Storage Control"), systemOn(false) {
  rooms.clear();
  apples.clear();
}

// Constructor
System::System(String systemName, int pumpPin, int pumpState, int numRooms) {
  setSystemName(systemName);
  setPumpPin(pumpPin);
  setPumpState(pumpState);
  setNumRooms(numRooms);

  rooms.clear();
  apples.clear();
}

/*
 * Setter for system name
 * Takes in a String which represented the desired system name
 * Returns nothing
 */
void System::setSystemName(String nameInput) {
  systemName = nameInput;
}

/*
 * Setter for pump pin
 * Takes in an integer containing pin which controls pump
 * Returns nothing
 */
void System::setPumpPin(int pumpPin) {
  sensingPump.pin = pumpPin;
}

/* Setter for the pump state
 * Takes in an integer representing the state of the pump (0 = off, 1 = on)
 * Returns nothing
 */
void System::setPumpState(int pumpState) {
  sensingPump.pumpState = pumpState;
}

/* Setter for the number of rooms in the system
 * Takes in an integer representing the number of rooms
 * Returns nothing
 */
 void System::setNumRooms(int numRoomsInput) {
  numRooms = numRoomsInput;
 }

/*
 * Getter for system name
 * Takes in nothing
 * Returns the name of the system
 */
String System::getSystemName() {
  return systemName;
}


/*
 * Getter for the pin which controls the pump
 * Takes in nothing
 * Returns the pin as an integer
 */
int System::getPumpPin() {
  return sensingPump.pin;
}

/* Getter for the state of the sensing pump
 * Takes in nothing
 * Returns the state as an integer (0 = off, 1 = on)
 */
int System::getPumpState() {
  return sensingPump.pumpState;
 }

/* Getter for the number of rooms in the system
* Takes in nothing
* Returns the number of rooms as an integer
*/
int System::getNumRooms() {
  return numRooms;
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
void System::addRoom(Room4 newRoom) {
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

/* Function which adds an apple object to the system
 * Takes in an apple objects and adds to the apples vector
 * Returns nothing
 */
void System::addApple(Apple newApple) {
  apples.push_back(newApple);
}

/* Function which removes an apple from the system given the name of the apple
 * Takes in the name of the apple
 * Returns a boolean indicating whether it has been removed
 */
bool System::removeApple(String appleName) {
  for (int i = 0; i < apples.size(); i++) {
    if (appleName == apples[i].getAppleName()) {
      apples.erase(apples.begin() + i);
      return true;
    }
  }
  return false;
}

/* pumpOn changes the pump that brings air in from that room to the sensing unit
*  Inputs: boolean pumpSwitch, whether the pump should be turned on or off
*  Outputs: rights to serial monitor whether the pump is on or off when called
*/
void System::pumpOn(boolean pumpSwitch, int roomNum){
  if(pumpSwitch){
    
    // Set solenoid to open and turn pump on
    digitalWrite(rooms[roomNum].senseSol.sensePin, HIGH);
    digitalWrite(sensingPump.pin, HIGH);

    sensingPump.pumpState = 1;
    
    delay(10);
  }
  else{
    
    // Set solenoid to closed and turn pump off
    digitalWrite(rooms[roomNum].senseSol.sensePin, LOW);
    digitalWrite(sensingPump.pin, LOW);

    // Set pump s
    sensingPump.pumpState = 0;
    
    delay(10);
  }
}

// Function which is used to sense oxygen using the DFRobot_OxygenSensor
float System::senseOxygen(DFRobot_OxygenSensor oxygen, int collectData){
    return oxygen.getOxygenData(collectData);
}
