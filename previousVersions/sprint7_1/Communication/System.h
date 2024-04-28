// M7
// Header for the System class

#ifndef System_h
#define System_h

#include <Arduino.h>
#include "Room.h"
#include "Apple.h"
#include "DFRobot_OxygenSensor.h"
#include <string>
#include <vector>
using std::vector;
using std::string;

// Struct which holds the information for the pump used to pull air into the measuring environemnt
struct Pump {
  int pin;
  int pumpState;
};

struct atmosphericSolenoid {
  int atmSolPin;
  int atmSolState;
};

class System {
  public:

    // Default constructor
    System();

    // Constructor
    System(String systemName, int pumpPin, int pumpState, int atmPin, int atmState, int numRooms);

    // Activates a room
    void activateRoom(int);

    // Deactivates a room
    void deactivateRoom(int);

    // Getters
    String getSystemName();
    int getPumpPin();
    int getPumpState();
    int getNumRooms();
    bool getSystemState();
    int getAtmSolPin();
    int getAtmSolState();

    // Setters
    void setSystemName(String systemName);
    void setPumpPin(int);
    void setPumpState(int);
    void setNumRooms(int);
    void setSystemState(bool);
    void setAtmSolPin(int);
    void setAtmSolState(int);

    // Function to add a room to the system
    void addRoom(Room newRoom);

    // Function to remove a room by room number
    bool removeRoom(int roomNum);

    // Function to turn on and off the measurement pump
    void pumpOn(boolean, int);

    // Function to add an apple to the system
    void addApple(Apple newApple);

    // Function to remove an apple from the system
    bool removeApple(String appleName);

    // Public fields
    vector<Room> rooms;            // Vector which holds the system's rooms
    vector<Apple> apples;           // Vector which holds the system's apple types

  private:

    // Private fields
    String systemName;
    struct Pump sensingPump;
    struct atmosphericSolenoid atmSol;
    int numRooms;
    bool systemOn;
};
#endif