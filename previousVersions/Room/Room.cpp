

#include "Room.h"
#include <Arduino.h>
#include "DFRobot_OxygenSensor.h"


//void Room::Room(void){
	
//}

Room::Room(int roomNum, int highLevel, int lowLevel, int solenoidPin1,int solenoidPin2, int pumpPin1) {
    setRoomNum(roomNum);
	setHighLevel(highLevel);
    setLowLevel(lowLevel);
    setSolPin1(solenoidPin1);
	setSolPin2(solenoidPin2);
    setPumpPin1(pumpPin1);
    boolean _previousSolenoid = false;
    long _highMargin = (_highLevel+_lowLevel)/2 + (_highLevel-(_highLevel+_lowLevel)/2)/3;
    long _lowMargin = (_highLevel+_lowLevel)/2 - (_highLevel-(_highLevel+_lowLevel)/2)/3;
    boolean _active = false;
}

//Activates the room so we know if in use
void Room::activate(){
    _active = true;
}

//Deactivates the room so we know if in use
void Room::deactivate(){
    _active = false;
}

float Room::senseOxygen(DFRobot_OxygenSensor oxygen, int collectData){
    return oxygen.getOxygenData(collectData);
}

/*evaluateRoom is used to measure the value of the potentiometer and determine if a
 * solenoid should be changed or not.
 * Inputs: potentiometer value, matrix used for on board LEDs
 * Output: Boolean true or false to mimick solenoid 
 */
boolean Room::evaluateRoom(int potValue,float oxygenValue, uint8_t frame[8][12]){
  boolean solenoid = _previousSolenoid;
       //low level it has to come back to in order to close valve
  float highMargin = 3.5;
  float lowMargin = 2.5;
  float value = oxygenValue*potValue/4000;
  //+potValue/10000
  Serial.println(value);
  if(value>_highLevel){
    roomDisplay(frame,1,0);
    solenoid = true;
  }
  else if(value<_lowLevel){
    roomDisplay(frame,0,1);
    solenoid = true;
  }
  else if(value<highMargin && value>lowMargin){
    roomDisplay(frame,1,1);
    solenoid = false;
  }
  else{
    return _previousSolenoid;
  }
  return solenoid;
}

/*roomDisplay displays the current room evaluation on onboard LEDs
*Inputs: LED Matrix, LED1 and LED2 on onboard LED (based on room number, from bottom of matrix)
*Outputs: N/A
*/
void Room::roomDisplay(uint8_t frame[8][12], int LEDA, int LEDB){
  for (int i = 0; i < 12; i++) {
    frame[8-_roomNum][i] = 0;
  }
  frame[8-_roomNum][0] = LEDA;
  frame[8-_roomNum][1] = LEDB;
}

/*pumpChange changes the pump that brings air in from that room to the sensing unit
*Inputs: boolean pumpSwitch, whether the pump should be turned on or off
*Outputs: rights to serial monitor whether the pump is on or off when called
*/
void Room::pumpChange(boolean pumpSwitch){
  if(pumpSwitch){
    digitalWrite(_solenoidPin1,HIGH);
    digitalWrite(_pumpPin1,HIGH);
    Serial.print("Pump ");
    Serial.print(_roomNum);
    Serial.println(" on for 10 Seconds.");
    delay(10);
  }
  else{
    digitalWrite(_pumpPin1,LOW);
    digitalWrite(_solenoidPin1,LOW);
    Serial.print("Pump ");
    Serial.print(_roomNum);
    Serial.println(" off");
    delay(10);
  }
}

/*solenoidChange opens the solenoid to allow the pump to turn on into the sensing unit
* Inputs: boolean solenoid, whether the valve should be on or off
* Outputs: displays to serial monitor whether room solenoid is open or closed
*/
void Room::solenoidChange(boolean solenoid){
  if(solenoid == true){
    if(!digitalRead(_solenoidPin2)){
    Serial.print("Room ");
    Serial.print(_roomNum);
    Serial.println(" open");
    delay(10);
    }
    digitalWrite(_solenoidPin2,HIGH);
  }
  else{
    if(digitalRead(_solenoidPin2)){
      Serial.print("Room ");
      Serial.print(_roomNum);
      Serial.println(" closed");
      delay(10);
    }
    digitalWrite(_solenoidPin2,LOW);
  }
  delay(500);
}

/*isActive returns whether the room is active or not
* Inputs: N/A
* Outputs: boolean true/false
*/
boolean Room::isActive(){
    return _active;
}

void Room::setRoomNum(int RoomNum){
	_roomNum = RoomNum;
}

void Room::setHighLevel(int highLevel){
	_highLevel = highLevel;
}

void Room::setLowLevel(int lowLevel){
	_lowLevel = lowLevel;
}

void Room::setSolPin1(int solenoidPin1){
	_solenoidPin1 = solenoidPin1;
}

void Room::setSolPin2(int solenoidPin2){
	_solenoidPin2 = solenoidPin2;
}

void Room::setPumpPin1(int pumpPin1){
	_pumpPin1 = pumpPin1;
}