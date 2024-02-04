//header file for Room Class

#ifndef Room3_h
#define Room3_h
#include "Arduino.h"
#include "DFRobot_OxygenSensor.h"
//#include <Vector.h>

class Room3{
    public:
    Room3(int roomNum, int highLevel, int lowLevel, int oxygenSolenoidPin,int nitrogenSolenoidPin,int pumpSolenoidPin, int pumpPin, int oxygenSolenoidState, int nitrogenSolenoidState);
	
    void activate();

    void deactivate();

    float senseOxygen(DFRobot_OxygenSensor, int);

    // Function which takes in an oxygen value, carbon value, and solenoid state array
    int evaluateRoom(float, float,int arr[2]);

    void pumpOn(boolean);

    void nitrogenSolenoidChange();

    void oxygenSolenoidChange();
    
    boolean isActive();

    // Setter for room number
    void setRoomNum(int);

    // Getter for room number
    int getRoomNum();

    // Setters for high and low levels
    void setHighLevel(int);
    void setLowLevel(int);

    // Getters for high and low levels
    int getHighLevel();
    int getLowLevel();

    // Setters for device pins
    void setPumpSolenoidPin(int);
    void setOxygenSolenoidPin(int);
    void setNitrogenSolenoidPin(int);
    void setPumpPin(int);

    // Getters for device pins
    int getPumpSolenoidPin();
    int getOxygenSolenoidPin();
    int getNitrogenSolenoidPin();
    int getPumpPin();

    // Setters for solenoids states
    void setN2solState(int);
    void setO2solState(int);

    // Getters for solenoid states
    int getN2solState();
    int getO2solState();

    private:
    int _roomNum;
    int _highLevel;
    int _lowLevel;
    int _nitrogenSolenoidPin;
    int _oxygenSolenoidPin;
    int _pumpSolenoidPin;
    int _pumpPin;
    int _oxygenSolenoidState;
    int _nitrogenSolenoidState;
    boolean _active;
    boolean _previousSolenoid;

	

};
#endif
