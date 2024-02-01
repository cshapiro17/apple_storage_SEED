

#include "Room2.h"
#include <Arduino.h>
#include "DFRobot_OxygenSensor.h"
#include <Vector.h>

//void Room::Room(void){
	
//}

Room2::Room2(int roomNum, int highLevel, int lowLevel, int solenoidO2,int solenoidN2,int solenoidPump, int pumpPin) {
  setRoomNum(roomNum);
	setHighLevel(highLevel);
  setLowLevel(lowLevel);
  setSolPump(solenoidPump);
	setSolO2(solenoidO2);
	setSolN2(solenoidN2);
  setpumpPin(pumpPin);
  boolean _previousSolenoid = false;
  long _highMargin = (_highLevel+_lowLevel)/2 + (_highLevel-(_highLevel+_lowLevel)/2)/3;
  long _lowMargin = (_highLevel+_lowLevel)/2 - (_highLevel-(_highLevel+_lowLevel)/2)/3;
  boolean _active = false;
} 

//Activates the room so we know if in use
void Room2::activate(){
    _active = true;
}

//Deactivates the room so we know if in use
void Room2::deactivate(){
    _active = false;
}

float Room2::senseOxygen(DFRobot_OxygenSensor oxygen, int collectData){
    return oxygen.getOxygenData(collectData);
}

/*evaluate is used to measure the value of the potentiometer and determine if a
 * solenoid should be changed or not.
 * Inputs: potentiometer value, matrix used for on board LEDs
 * Output: Boolean true or false to mimick solenoid 
 */
void Room2::evaluateRoom(float oxygenValue, float carbonValue, int solenoid[2]){
  int carbonLevel;
  int oxygenLevel;
  //boolean solenoid = _previousSolenoid;
       //low level it has to come back to in order to close valve
  float highMargin = 3.5;
  float lowMargin = 2.5;

  Serial.println(carbonValue);
  
  //0 = neutral, 1 = too low, 2 = too high, 3 = almost good?
  //carbon levels *************************************************
  if(carbonValue>_highLevel){
    carbonLevel = 2;
  }
  else if(carbonValue<_lowLevel){
	carbonLevel = 1;
  }
  else if(carbonValue<highMargin && carbonValue>lowMargin){
	carbonLevel = 0;
  }
  else{
	//stay the same
  }
  
  //oxygen levels *************************************************
  if(oxygenValue>_highLevel){
    oxygenLevel = 2;
  }
  else if(oxygenValue<_lowLevel){
	oxygenLevel = 1;
  }
  else if(oxygenValue<highMargin && oxygenValue>lowMargin){
	  //stay the same
	  oxygenLevel = 0;
  }
  else{
	//stay the same
  }
  
  //evaluation
  int levels[2] = {oxygenLevel,carbonLevel};
  //homeostasis ***************************************************
  if(oxygenLevel == 0 && carbonLevel == 0){
	  solenoid[0] = 0;
    solenoid[1] = 0;
  }
  //O2 good, CO2 too low ******************************************
  else if(oxygenLevel == 0 && carbonLevel == 1){
	  //flag that CO2 is too low? what can we do ???
    solenoid[0] = 0;
    solenoid[1] = 0;
  }
  //O2 good, CO2 too high *****************************************
  else if(oxygenLevel == 0 && carbonLevel == 2){
	  //turn on nitrogen solenoid
    solenoid[0] = 1;
    solenoid[1] = 0;
  }
  //O2 too low, CO2 good ******************************************
  else if(oxygenLevel == 1 && carbonLevel == 0){
    //turn on atmosphere solenoid
    solenoid[0] = 0;
    solenoid[1] = 1;
  }
   //O2 too low, CO2 too low ***************************************
  else if(oxygenLevel == 1 && carbonLevel == 1){
	  //turn on atmosphere solenoid ???
    solenoid[0] = 0;
    solenoid[1] = 1;
  } 
  //O2 too low, CO2 too high ***************************************
  else if(oxygenLevel == 1 && carbonLevel == 2){
	  //turn on atmosphere solenoid
    solenoid[0] = 0;
    solenoid[1] = 1;
  }
   //O2 too high, CO2 good *****************************************
  else if(oxygenLevel == 2 && carbonLevel == 0){
	  //turn on nitrogen solenoid
    solenoid[0] = 1;
    solenoid[1] = 0;
  } 
  //O2 too high, CO2 too low ****************************************
  else if(oxygenLevel == 2 && carbonLevel == 1){
	  //turn on both solenoids ????
    solenoid[0] = 1;
    solenoid[1] = 1;
  }
  //O2 too high, CO2 too high ***************************************
  else if(oxygenLevel == 2 && carbonLevel == 2){
	  //turn on both solenoids ????
    solenoid[0] = 1;
    solenoid[1] = 1;
  }
}
/*roomDisplay displays the current room evaluation on onboard LEDs
*Inputs: LED Matrix, LED1 and LED2 on onboard LED (based on room number, from bottom of matrix)
*Outputs: N/A
*/
void Room2::roomDisplay(uint8_t frame[8][12], int LEDA, int LEDB){
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
void Room2::pumpChange(boolean pumpSwitch){
  if(pumpSwitch){
    digitalWrite(_solenoidPump,HIGH);
    digitalWrite(_pumpPin,HIGH);
    Serial.print("Pump ");
    Serial.print(_roomNum);
    Serial.println(" on for 10 Seconds.");
    delay(10);
  }
  else{
    digitalWrite(_pumpPin,LOW);
    digitalWrite(_solenoidPump,LOW);
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
void Room2::solenoidChange(int solenoid[2]){
  //Nitrogen solenoid change
  if(solenoid[0] == 1){
    if(!digitalRead(_solenoidN2)){
    Serial.print("Room ");
    Serial.print(_roomNum);
    Serial.println("Nitrogen open.");
    delay(10);
    }
    digitalWrite(_solenoidN2,HIGH);
  }
  else{
    if(digitalRead(_solenoidN2)){
      Serial.print("Room ");
      Serial.print(_roomNum);
      Serial.println("Nitrogen closed.");
      delay(10);
    }
    digitalWrite(_solenoidN2,LOW);
  }
  
  delay(500);
  //atmosphere solenoid change
  if(solenoid[1] == 1){
    if(!digitalRead(_solenoidO2)){
    Serial.print("Room ");
    Serial.print(_roomNum);
    Serial.println("Atmosphere open");
    delay(10);
    }
    digitalWrite(_solenoidO2,HIGH);
  }
  else{
    if(digitalRead(_solenoidO2)){
      Serial.print("Room ");
      Serial.print(_roomNum);
      Serial.println("Atmosphere closed");
      delay(10);
    }
    digitalWrite(_solenoidO2,LOW);
  }
  delay(500);
}

/*isActive returns whether the room is active or not
* Inputs: N/A
* Outputs: boolean true/false
*/
boolean Room2::isActive(){
    return _active;
}

void Room2::setRoomNum(int RoomNum){
	_roomNum = RoomNum;
}

void Room2::setHighLevel(int highLevel){
	_highLevel = highLevel;
}

void Room2::setLowLevel(int lowLevel){
	_lowLevel = lowLevel;
}

void Room2::setSolPump(int solenoidPump){
	_solenoidPump = solenoidPump;
}

void Room2::setSolO2(int solenoidO2){
	_solenoidO2 = solenoidO2;
}

void Room2::setSolN2(int solenoidN2){
	_solenoidN2 = solenoidN2;
}

void Room2::setpumpPin(int pumpPin){
	_pumpPin = pumpPin;
}