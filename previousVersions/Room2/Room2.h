//header file for Room Class

#ifndef Room2_h
#define Room2_h
#include "Arduino.h"
#include "DFRobot_OxygenSensor.h"
#include <Vector.h>

class Room2{
    public:
    Room2(int roomNum, int highLevel, int lowLevel, int solenoidO2,int solenoidN2,int solenoidPump, int pumpPin);

	
    void activate();

    void deactivate();

    float senseOxygen(DFRobot_OxygenSensor, int);

    void evaluateRoom(float, float,int arr[2]);

    void roomDisplay(uint8_t frame[8][12],int LEDA, int LEDB);

    void pumpChange(boolean);

    void solenoidChange(int arr[2]);
    
    boolean isActive();

    private:
        int _roomNum;
        int _highLevel;
        int _lowLevel;
        int _solenoidN2;
        int _solenoidO2;
		int _solenoidPump;
        int _pumpPin;
		boolean _active;
        boolean _previousSolenoid;
	void setRoomNum(int);
	
	void setHighLevel(int);
	
	void setLowLevel(int);

	void setSolPump(int);
	
	void setSolO2(int);
	
	void setSolN2(int);

    void setpumpPin(int);
	
	

};
#endif