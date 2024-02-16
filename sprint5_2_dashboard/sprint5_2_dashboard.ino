/*Team 16--Apple Storage Controller MAIN Code for sprint 5 -- Sensing room able to pump in air from 2 rooms, 
* along with sense room flushing capabilities to make sure sensor is reading values properly.
* Also able to read both O2 and CO2, making changes to both. Has CO2 Sensor Integration
* Setup to work with Arduino Giga R1 Wifi
*/

#include "MillisTimerLib.h"
#include "DFRobot_OxygenSensor.h"
#include "CO2Sensor.h"

#include "System.h"
#include "Config.h"
#include "Build.h"


Build sys;

// **************************************************************************
// GLOBAL VARIABLES
// **************************************************************************
  // Set calibrate to active
  boolean calibrationNecessary = true;

  // Intialize previous millis and current millis
  unsigned long previousMillis = 0;
  unsigned long currentMillis;

  // Initialize the room under test variable to start at the first room
  int x = 0;

  // Initialize the number of tests since last calibration
  int numTestsSinceCalibration = 0;

  // The variable which holds the state of the system
  static unsigned int state;

//****************************************************************************
// CO2 VARIABLES
// To be replaced by new CO2 sensor
//****************************************************************************
CO2Sensor explorCO2;
// #define atmCO2 = 418.51;
// float Rl = 22000;
// float R0 = 228067;
// float minvalue = 80;
float CO2Percent;

//****************************************************************************
// OXYGEN VARIABLES
// Likely to be replaced by new O2 sensor
//****************************************************************************

float oxygenValue;
float currentO2;
float previousO2;
DFRobot_OxygenSensor oxygen;
#define collectNumber 10
#define Oxygen_IIC_Address ADDRESS_3\

//***************************************************************************
// DATA COLLECTION AND STORAGE
//***************************************************************************

String dataArray[7] = {
  "",                    // Room [0]
  "",                    // Oxygen Value [1]
  "",                    // Carbon Value [2]
  "",                    // Temperature Value [3]
  "",                    // Adding oxygen? [4]
  "",                    // Adding nitrogen? [5]
  "OK"                   // General System Status [6]
  };

String dataString;       // String to hold all data to be sent over serial connection

// *****************************************************************************************
// FUNCTIONS
// *****************************************************************************************

/* Function which will monitor sensor readings and return true when values are stable
 * Takes in nothing
 * Returns nothing
 */
void checkSensorLevel() {

  // This delay can be used to run the pump for a certain amount of time before checking levels
  delay(PUMP_DELAY);

  boolean sensorsNotLevel = true;
  
  while(sensorsNotLevel) {
    
    // Get oxygen value from sensor
    currentO2 = oxygen.getOxygenData(collectNumber);

    // Check to see if the oxygen sensor has read consistent values for 2 cycles
    if(currentO2 < previousO2*1.01 && currentO2 > previousO2*.99){ 

      //setprevious to 0 for next time, so that pump stays on for at least 20 Sec
      previousO2 = 0; 

      // SensorsNotLevel is now false and while loop will break
      sensorsNotLevel = false;
    }

    // Sensors are not level
    else {

      // Set previous oxygen value for comparison in the future
      previousO2 = currentO2;
    }
  }
}

/* Converts an array of strings into a string of comma-seperated values 
 * Takes in the dataArray 
 * Returns the dataString
 */
String arrayToString(String dataArray[]) {

  for (int i = 0; i < 6; i++) {
    dataString = dataString + dataArray[i] + ",";
  }
  
  dataString = dataString + dataArray[6];

  return dataString;
}

//***************************************************************************
//Setup
//***************************************************************************

void setup() {
  // Run build.cpp
  sys.setUpSystem();
  
  // Initialize serial connection @ 9600 Baud Rate for serial monitor
  Serial.begin(9600);
  // Wait for serial port to open
  while(!Serial);
  
  //initialize Serial1 communication @ 9600 Baud Rate for CO2 sensor (RX/TX 0)
  Serial1.begin(9600);
  // Wait for serial1 port to open
  while(!Serial1);

  //EXPLORIR CO2 connection
  explorCO2.initialize();
  delay(CO2_CONNECT_DELAY);                 

  // Begin state machine
  state = 1;
}

//****************************************************************************
//Loop 
//****************************************************************************
void loop(){

  // Set current time in millis
  currentMillis = millis();

  // Create state machine
  switch(state){
    
    // Reset state ********************************************************************************************
    // Happens on start up and activates rooms
    case 1:

      // Set all rooms to be active
      VRS.rooms[0].activate();
      VRS.rooms[1].activate();
      
      delay(RESET_DELAY);

      // Proceed to the administrative state
      state = 2;
    break;
    
    // Administrative State **********************************************************************************
    // This state performs system checks to make sure that the system is OK
    case 2:

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= TIMER_DELAY){
        
        // Set previousMillis for comparison later
        previousMillis = currentMillis;
        
        // Check to see if calibration is necessary
        if(calibrationNecessary){
   
          // Proceed to the calibration state
          state = 3;
        }
        else {
          
          // If all checks have been passed, pass message to dashboard
          dataArray[6] = "System OK";                     // Create update about system
          dataString = "";                                // Reset dataString
          dataString = arrayToString(dataArray);          // Convert data to string
          Serial.println(dataString);                     // Send update
  
          // Proceed to the measurement state
          state = 4;
        }
      }
    break;

    // Calibration State ********************************************************************************
    case 3:

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= TIMER_DELAY) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;
        
        // All code relavent to calibrating the sensors will be located here
        
        // For now we will just tell the dashboard that we are calibrating and delay for 10 seconds
        dataArray[6] = "Calibrating...";                // Create update about room under test
        dataString = "";                                // Reset dataString
        dataString = arrayToString(dataArray);          // Convert data to string
        Serial.println(dataString);                     // Send update 

        delay(SIMULATE_CALIBRATION);

        // Calibration is no longer necessary
        calibrationNecessary = false;
        numTestsSinceCalibration = 0;
        
        // Return to the administrative state
        state = 2;
      }
    break;

    // Measurement state *********************************************************************************
    // This state is responsible for capturing sensor data and making changes to rooms
    case 4:

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= TIMER_DELAY) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        // Check to see if the room in question is active
        if (VRS.rooms[x].isActive()) {

          // Clear data in preperation for next room
          dataArray[1] = "";
          dataArray[2] = "";
          dataArray[3] = "";
          dataArray[4] = "";
          dataArray[5] = "";
          
          // If active, update dashboard
          dataArray[0] = "room " + String(x + 1);         // Create update about room under test
          dataString = "";                                // Reset dataString
          dataString = arrayToString(dataArray);          // Convert data to string
          Serial.println(dataString);                     // Send update 

          // This turns on the pump to pull in air into the measuring environment
          VRS.pumpOn(true, VRS.rooms[x].getRoomNum() - 1);

          // Call function to make sure sensors are level before proceeding
          checkSensorLevel();

          // When sensors are leveled, we can turn off the pump
          VRS.pumpOn(false, VRS.rooms[x].getRoomNum() - 1);

          // Sensors should be level and we can now take measurements
          oxygenValue = oxygen.getOxygenData(collectNumber);                       // Get oxygen percentage
          CO2Percent = explorCO2.getPercent();                                     // Get percentage of CO2
          int roomTemp = 36;                                                       // Placeholder for temperature
     
          // Evaluate the room, this function will set the states of the solenoids to their necessary positions (open or closed)
          VRS.rooms[x].evaluateRoom(oxygenValue, CO2Percent);

          // Get the states of each solenoid
          int oxygenSolenoidState = VRS.rooms[x].getO2solState();
          int nitrogenSolenoidState = VRS.rooms[x].getN2solState();

          // Report changes to dashboard
          dataArray[1] = String(oxygenValue);
          dataArray[2] = String(CO2Percent);
          dataArray[3] = String(roomTemp);
          dataArray[4] = String(oxygenSolenoidState);
          dataArray[5] = String(nitrogenSolenoidState);
          dataString = "";
          dataString = arrayToString(dataArray);
          Serial.println(dataString);
        }
        
        // Change room
        if (x == NUM_ROOMS) {
          x = 0;
        }
        else {  
          x++;
        }

        // Check if calibration is necessary
        if (numTestsSinceCalibration == (CYCLES_PER_CALIBRATION * NUM_ROOMS)) {
          calibrationNecessary = true;
        }
        else {
          numTestsSinceCalibration++;
        }

        // Go to the administrative state
        state = 2;
      }
      
      break;
    } 
  }
