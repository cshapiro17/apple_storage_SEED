// M7
// Header for the Room class

#ifndef Room_h
#define Room_h
#include <Arduino.h>
#include "Apple.h"

// Struct which holds the pin and state of the oxygen solenoid for the room
struct oxygenFan {
  int O2pin;
  int O2state;
};

// Struct which holds the pin and state of the nitrogen solenoid for the room
struct nitrogenSolenoid {
  int N2pin;
  int N2state;
};

// Struct which holds the pin and state for the solenoid which controls air into the measuring environement
struct sensingSolenoid {
  int sensePin;
  int senseState;
};

class Room{
    public:

      // Default Constructor
      Room();
  
      // Constructor
      Room(String roomName, int roomNum, Apple appleType, int o2FanPin, int n2SolenoidPin, int senseSolenoidPin, int o2FanState, int n2SolenoidState, int senseSolenoidState);

      // Activates the room
      void activate();

      // Deactivates the room
      void deactivate();

      // Sets the room to be under test
      void setUnderTest(bool);

      // Gets the testing state of the room
      bool isUnderTest();
  
      // Function which takes in an oxygen value, carbon value, and solenoid state array
      int evaluateRoom(float, float);

      // Modifys the state of the nitrogen solenoid
      void nitrogenSolenoidChange();

      // Modifys the state of the oxygen solenoid/fan
      void oxygenFanChange();

      // Returns if the room is active
      bool isActive();
  
      // Setter for room number
      void setRoomNum(int);
  
      // Getter for room number
      int getRoomNum();

      // Setter for apple type
      void setAppleType(Apple);

      // Getter for apple type
      Apple getAppleType();
  
      // Setters for device pins
      void setOxygenFanPin(int);
      void setNitrogenSolenoidPin(int);
      void setSensingSolenoidPin(int);
  
  
      // Getters for device pins
      int getOxygenFanPin();
      int getNitrogenSolenoidPin();
      int getSensingSolenoidPin();
  
      // Setters for solenoids states
      void setN2solState(int);
      void setO2fanState(int);
      void setSenseSolState(int);
  
      // Getters for solenoid states
      int getN2solState();
      int getO2fanState();
      int getSenseSolState();

      // Setter for room name
      void setRoomName(String);

      // Getter for room name
      String getRoomName();

      // Getter for current apple
      String getCurrentApple();

      // Setter for current apple
      void setCurrentApple(String);

      // Public fields
      struct sensingSolenoid senseSol;

    private:

      // Private fields
      String roomName;
      int roomNum;
      struct oxygenFan O2fan;
      struct nitrogenSolenoid N2sol;
      Apple appleType;
      String currentApple;
      bool active;
      bool underTest;
    
};
#endif
