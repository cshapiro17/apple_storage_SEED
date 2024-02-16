//header file for Room Class

#ifndef Room4_h
#define Room4_h
#include <Arduino.h>
#include "Apple.h"
//#include <Vector.h>


// Struct which holds the pin and state of the oxygen solenoid for the room
struct oxygenSolenoid {
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

class Room4{
    public:

      // Default Constructor
      Room4();
  
      // Constructor
      Room4(String roomName, int roomNum, Apple appleType, int o2SolenoidPin, int n2SolenoidPin, int senseSolenoidPin, int o2SolenoidState, int n2SolenoidState, int senseSolenoidState);

      // Activates the room
      void activate();

      // Deactivates the room
      void deactivate();
  
      // Function which takes in an oxygen value, carbon value, and solenoid state array
      int evaluateRoom(float, float);

      // Modifys the state of the nitrogen solenoid
      void nitrogenSolenoidChange();

      // Modifys the state of the oxygen solenoid/fan
      void oxygenSolenoidChange();

      // Returns if the room is active
      boolean isActive();
  
      // Setter for room number
      void setRoomNum(int);
  
      // Getter for room number
      int getRoomNum();

      // Setter for apple type
      void setAppleType(Apple);

      // Getter for apple type
      Apple getAppleType();
  
      // Setters for device pins
      void setOxygenSolenoidPin(int);
      void setNitrogenSolenoidPin(int);
      void setSensingSolenoidPin(int);
  
  
      // Getters for device pins
      int getOxygenSolenoidPin();
      int getNitrogenSolenoidPin();
      int getSensingSolenoidPin();
  
      // Setters for solenoids states
      void setN2solState(int);
      void setO2solState(int);
      void setSenseSolState(int);
  
      // Getters for solenoid states
      int getN2solState();
      int getO2solState();
      int getSenseSolState();

      // Setter for room name
      void setRoomName(String);

      // Getter for room name
      String getRoomName();

      // Public fields
      struct sensingSolenoid senseSol;

    private:

      // Private fields
      String roomName;
      int roomNum;
      struct oxygenSolenoid O2sol;
      struct nitrogenSolenoid N2sol;
      Apple appleType;
      boolean active;
    
};
#endif
