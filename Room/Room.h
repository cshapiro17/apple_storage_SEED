//header file for Room Class

#ifndef Room_h
#define Room_h
#include "Arduino.h"

class Room{
    public:
    Room(int roomNum, int highLevel, int lowLevel, int solenoidPin1, int solenoidPin2,int pumpPin1);

    void activate();

    void deactivate();

    boolean evaluateRoom(int potValue, uint8_t frame[8][12]);

    void roomDisplay(uint8_t frame[8][12],int LEDA, int LEDB);

    void pumpChange(boolean);

    void solenoidChange(boolean);
    
    boolean isActive();

    private:
        int _roomNum;
        int _highLevel;
        int _lowLevel;
        int _solenoidPin1;
        int _pumpPin1;
        boolean _previousSolenoid;


};
#endif