// M7
// Cpp for the System class

/*
  Code for the control of the system objects. Has getters, setters, and other functions which 
  define the variables for system objects.
*/

#include <Arduino.h>
#include "System.h"
using namespace std;

// Default constructor
System::System() : systemName("Apple Storage Control"), systemOn(false) {
  rooms.clear();
  apples.clear();
}

// Constructor
System::System(String systemName, int pumpPin, int pumpState, int atmPin, int atmState, int numRooms) {
  setSystemName(systemName);
  setPumpPin(pumpPin);
  setPumpState(pumpState);
  setAtmSolPin(atmPin);
  setAtmSolState(atmState);
  setNumRooms(numRooms);
  setSystemState(false);

  rooms.clear();
  apples.clear();
}

/* Function which activates a room in the system
 * Takes in an integer representing the room
 * Returns nothing
 */
void System::activateRoom(int room) {
  rooms[room].activate();
}

/* Function which deactivates a room in the system
 * Takes in an integer representing the room
 * Returns nothing
 */
void System::deactivateRoom(int room) {
  // Determine if the room is currently pumping air
  if (rooms[room].isUnderTest()) {
    // Turn pump off
    setPumpState(0);
  }
  // Deactivate the room
  rooms[room].deactivate();
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

  digitalWrite(sensingPump.pin, sensingPump.pumpState);
}

/* Setter for the number of rooms in the system
 * Takes in an integer representing the number of rooms
 * Returns nothing
 */
void System::setNumRooms(int numRoomsInput) {
  numRooms = numRoomsInput;
}

/* Setter for the state of the system
 * Takes in a boolean representing the state of the system (true = on)
 * Returns nothing
 */
void System::setSystemState(bool systemState) {
  systemOn = systemState;

  if (!systemOn) {

    // Deactivate each room
    for (int i = 0; i < rooms.size(); i++) {
      rooms[i].deactivate();
    }

    // Turn off atmospheric solenoid
    setAtmSolState(0);

    // Turn off pump
    setPumpState(0);

  }
}

/* Setter for the pin of the atmospheric solenoid which is used for calibration
 * Takes in an integer representing the pin of the solenoid
 * Returns nothing
 */
void System::setAtmSolPin(int atmSolPin) {
  atmSol.atmSolPin = atmSolPin;
}

/* Setter for the state of the atmospheric solenoid
 * Takes in an integer representing the state of the solenoid
 * Returns nothing
 */
void System::setAtmSolState(int atmSolState) {
  atmSol.atmSolState = atmSolState;

  // Turn on or off the solenoid accordingly
  digitalWrite(atmSol.atmSolPin, atmSol.atmSolState);
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
*  Takes in nothing
*  Returns the number of rooms as an integer
*/
int System::getNumRooms() {
  return numRooms;
}

/* Getter for the state of the system
 * Takes in nothing
 * Returns state of the system as a boolean
 */
bool System::getSystemState() {
  return systemOn;
}

/* Getter for the pin of the atmospheric solenoid
 * Takes in nothing
 * Returns the pin for the atmospheric solenoid as an int
 */
int System::getAtmSolPin() {
  return atmSol.atmSolPin;
}

/* Getter for the state of the atmospheric solenoid
 * Takes in nothing
 * Returns the state of the atmospheric solenoid as an int
 */
int System::getAtmSolState() {
  return atmSol.atmSolState;
}

/*
 * Function which adds a room object to the system
 * Takes in a room object and adds to rooms vector
 * Returns nothing
 */
void System::addRoom(Room newRoom) {
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
void System::pumpOn(bool pumpSwitch, int roomNum){

  if(pumpSwitch){

    // Check to see if atmospheric solenoid should be opened
    if (roomNum == 101) {
      // Open the atmospheric solenoid
      setAtmSolState(1);
    }
    else {
      // Open the solenoid to the room
      rooms[roomNum].setSenseSolState(1);
    }
    // Turn on pump
    setPumpState(1);
    
    delay(10);
  }
  else{

    // Check to see if atmospheric solenoid should be opened
    if (roomNum == 101) {
      // Close the atmospheric solenoid
      setAtmSolState(0);
    }
    else {
      // Close the solenoid to the room
      rooms[roomNum].setSenseSolState(0);
    }
    // Turn off pump
    setPumpState(0);
    
    delay(10);
  }
}