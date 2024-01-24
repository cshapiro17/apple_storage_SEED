//header file for Room Class

#ifndef Room_h
#define Room_h
#include "Arduino.h"
#include "DFRobot_OxygenSensor.h"

class Room{
    public:
    Room(int roomNum, int highLevel, int lowLevel, int solenoidPin1, int solenoidPin2,int pumpPin1);

	//Room();
	
    void activate();

    void deactivate();

    float senseOxygen(DFRobot_OxygenSensor, int);

    boolean evaluateRoom(int, float, uint8_t frame[8][12]);

    void roomDisplay(uint8_t frame[8][12],int LEDA, int LEDB);

    void pumpChange(boolean);

    void solenoidChange(boolean);
    
    boolean isActive();

    private:
        int _roomNum;
        int _highLevel;
        int _lowLevel;
        int _solenoidPin1;
		int _solenoidPin2;
        int _pumpPin1;
		boolean _active;
        boolean _previousSolenoid;
	void setRoomNum(int);
	
	void setHighLevel(int);
	
	void setLowLevel(int);

	void setSolPin1(int);
	
	void setSolPin2(int);
	
	void setPumpPin1(int);
	
	

};
#endif