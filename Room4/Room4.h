//header file for Room Class

#ifndef Room3_h
#define Room3_h
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

class Room3{
    public:

      // Default Constructor
      Room3();
  
      // Constructor
      Room3(int roomNum, Apple appleType);
  	
      void activate();
  
      void deactivate();
  
      // Function which takes in an oxygen value, carbon value, and solenoid state array
      int evaluateRoom(float, float);
  
      void nitrogenSolenoidChange();
  
      void oxygenSolenoidChange();
      
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
  
  
      // Getters for device pins
      int getOxygenSolenoidPin();
      int getNitrogenSolenoidPin();
  
      // Setters for solenoids states
      void setN2solState(int);
      void setO2solState(int);
  
      // Getters for solenoid states
      int getN2solState();
      int getO2solState();

    private:

      // Fields
      int roomNum;
      struct oxygenSolenoid O2sol;
      struct nitrogenSolenoid N2sol;
      Apple appleType;
      boolean active;
    
};
#endif
