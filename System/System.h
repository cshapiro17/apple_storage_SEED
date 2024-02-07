// Header file for the system class

#ifndef System_h
#define System_h

#include <Arduino.h>
#include "Room3.h"
#include "Apple.h"
#include "DFRobot_OxygenSensor.h"
#include <string>
#include <vector>
using std::vector;
using std::string;

// Struct which holds the information for the pump used to pull air into the measuring environemnt
struct Pump {
  int pin;
  int solenoidPin;
  int pumpState;
};

class System {
  public:

    // Default constructor
    System();

    // Getters
    string getSystemName();
    int getPumpSolenoidPin();
    int getPumpPin();
    int getNumRooms();

    // Setters
    void setSystemName(string systemName);
    void setPumpSolenoidPin(int);
    void setPumpPin(int);
    int setNumRooms(int);

    // Function which takes in the button input and calls the appropriate function
    void handleButtonInput(int buttonInput);

    // Function to turn on and off the system
    void toggleSystemOn();

    // Function to add a room
    void addRoom(Room3 newRoom);

    // Function to remove a room by room number
    bool removeRoom(int roomNum);

    // Function to turn on and off the measurement pump
    void pumpOn(boolean);

    // Measure oxygen
    float senseOxygen(DFRobot_OxygenSensor, int);

    // Measure CO2
    // TODO: ADD CO2 MEASUREMENT CODE

    // Vector of the room objects
    vector<Room3> rooms;

    // Vector of apple objects
    vector<Apple> apples;

  private:

    // Fields
    string systemName;
    struct Pump boxPump;
    int numRooms;
    boolean systemOn;
};
#endif
