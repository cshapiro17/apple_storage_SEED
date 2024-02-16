// Header file for the system class

#ifndef System_h
#define System_h

#include <Arduino.h>
#include "Room4.h"
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

class System {
  public:

    // Default constructor
    System();

    // Getters
    String getSystemName();
    int getPumpPin();
    int getPumpState();
    int getNumRooms();

    // Setters
    void setSystemName(String systemName);
    void setPumpPin(int);
    void setPumpState(int);
    void setNumRooms(int);

    // Function which takes in the button input and calls the appropriate function
    void handleButtonInput(int buttonInput);

    // Function to turn on and off the system
    void toggleSystemOn();

    // Function to add a room to the system
    void addRoom(Room4 newRoom);

    // Function to remove a room by room number
    bool removeRoom(int roomNum);

    // Function to turn on and off the measurement pump
    void pumpOn(boolean, int);

    // Function to add an apple to the system
    void addApple(Apple newApple);

    // Function to remove an apple from the system
    bool removeApple(String appleName);

    // Measure oxygen
    float senseOxygen(DFRobot_OxygenSensor, int);

    // Measure CO2
    // TODO: ADD CO2 MEASUREMENT CODE

    // Public fields
    vector<Room4> rooms;            // Vector which holds the system's rooms
    vector<Apple> apples;           // Vector which holds the system's apple types

  private:

    // Private fields
    String systemName;
    struct Pump sensingPump;
    int numRooms;
    boolean systemOn;
};
#endif
