// USES M4 CORE
// The M4 core does not have access to serial ports which means that RPCs are 
// needed to get sensor values

#include <Arduino.h>

#include "MillisTimerLib.h"
#include "C:\Users\Cmsha\OneDrive - University of Vermont\Semester 8\Capstone\apple_storage_SEED\apple_storage_SEED\sprint6_3\Config.h"
#include <RPC.h>

#include <cmath>

using namespace rtos;

//****************************************************************************
// GLOBAL SYSTEM VARIABLES
//****************************************************************************

// Set calibrate to active
bool calibrationNecessary = true;

// Intialize previous millis and current millis
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

// Initialize the room under test variable to start at the first room
int x = 0;

// Initialize the number of tests since last calibration
int numTestsSinceCalibration = 0;

// The variable which holds the state of the system
static unsigned int state;

// Integer representing the state of the system (1 = on, 0 = off)
int systemOn;

// ***************************************************************************
// FUNCTIONS
// ***************************************************************************

/* Function which will monitor sensor readings and return true when the standard deviation of the last 3 values is low
 * Takes in nothing
 * Returns nothing
 */
void checkSensorLevel(int x) {

  bool sensorsLevel = false;
  bool exit = false;

  int oldestValue = 0;
  int size = 3;
  float sumO2 = 0.0, sumCO2 = 0.0, meanO2 = 0.0, meanCO2 = 0.0, variationO2 = 0.0, variationCO2 = 0.0, sdO2, sdCO2;

  float o2Array[3] = {};
  float co2Array[3] = {};
  
  /*
  We should pump in air for a set amount of time but we also want to check
  for many system conditions. To do this, we will check every second and exit
  if necessary
  */


  for (int i = 0; i < PUMP_DELAY; i++) {
    int roomActive = RPC.call("checkRoomActive", x).as<int>();

    if (roomActive == 0) {
      exit = true;
      break;
    }
    else {
      delay(DELAY_1_SEC);
    }
  }

  if (!exit) {
    // Fill the arrays
    for (int i = 0; i < size; i++) {
      int roomActive = RPC.call("checkRoomActive", x).as<int>();

      if (roomActive == 0) {
        exit = true;
        break;
      }

      o2Array[i] = RPC.call("getO2Reading").as<float>();

      delay(250);

      co2Array[i] = RPC.call("getCO2Reading").as<float>();

      // Delay to allow for more air to pump in
      delay(CALIBRATION_VALUE_DELAY * DELAY_1_SEC);
    }

    while(!sensorsLevel && !exit) {

      // Get sum of arrays
      for (int i = 0; i < size; i++) {
        sumO2 += o2Array[i];
        sumCO2 += co2Array[i]; 
      }

      // Get mean of arrays
      meanO2 = sumO2 / size;
      meanCO2 = sumCO2 / size;

      // Find variation
      for (int i = 0; i < size; ++i) { 
            variationO2 += pow(o2Array[i] - meanO2, 2); 
            variationCO2 += pow(co2Array[i] - meanCO2, 2);
        } 

      // Find standard deviation
      sdO2 = sqrt(variationO2 / size);
      sdCO2 = sqrt(variationCO2 / size);

      // Check to see if the oxygen sensor and carbon dioxide sensor are consistent
      if(sdO2 < 0.5 && sdCO2 < 0.5){ 

        // Sensors are level and while loop will break
        sensorsLevel = true;
      }
      else {
        
        // Replace oldest value
        o2Array[oldestValue] = RPC.call("getO2Reading").as<float>();

        delay(250);

        co2Array[oldestValue] = RPC.call("getCO2Reading").as<float>();

        // Oldest value is now next value in the array
        if (oldestValue == 2) {
          oldestValue = 0;
        }
        else {
          oldestValue++;
        }
      }
    }
  }
}

void setup() {

  RPC.begin();

  // Go to administrative state
  state = 1;
}

void loop() {

  currentMillis = millis();

  switch(state) {

    // Administrative State **********************************************************************************
    // This state performs system checks to make sure that the system is OK
    case 1:

      // Set currentMillis
      currentMillis = millis();

      if ((currentMillis - previousMillis) >= TIMER_DELAY) {
        
        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        //RPC.call("updateDashM4", "'In state 1'");

        // RPC to get state of the system
        systemOn = RPC.call("systemOn").as<int>();

        // Check to see if the system is on
        if (systemOn == 0) {
          
          // Proceed to stand by state
          state = 4;
        }

        // Check to see if calibration is necessary
        else if (calibrationNecessary) {

          // Proceed to the calibration state
          state = 2;
        }
        else {

          // System is OK

          // Proceed to the measurement state
          state = 3;
        }
      }
    break;

    // Calibration State ********************************************************************************
    case 2:

      // Set currentMillis
      currentMillis = millis();

      if ((currentMillis - previousMillis) >= TIMER_DELAY) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        //RPC.call("updateDashM4", "'In state 2'");

        /*
        We will need to pull atmospheric air in first
        Then make sure the values are level
        Then call calibrate functions
        */

        // Turn the pump on to pull atm air into the sensing room
        RPC.call("senseState", true, 101);

        // Make sure that sensors have leveled
        checkSensorLevel(101);

        // Turn the pump off and callibrate the sensors
        RPC.call("senseState", false, 101);

        // The sensors should now be level and we can calibrate to atmospheric air
        
        // We should be making sure that the values being checked are near atm

        // RPC call to calibrate the sensors
        RPC.call("calibrateSensors");

        // Calibration is no longer necessary
        calibrationNecessary = false;
        numTestsSinceCalibration = 0;

        // Return to the administrative state
        state = 1;
      }
    break;

    // Measurement state *********************************************************************************
    // This state is responsible for capturing sensor data and making changes to rooms  
    case 3:

      // Set currentMillis
      currentMillis = millis();

      if ((currentMillis - previousMillis) >= TIMER_DELAY) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        //RPC.call("updateDashM4", "'In state 3'");

        // Make RPC call to get room state
        int active = RPC.call("checkRoomActive", x).as<int>();

        if (active == 1) {

          // Make RPC call to turn pump on for the room
          RPC.call("senseState", true, x);

          // Keep the pump on until the sensors have leveled
          checkSensorLevel(x);

          // Make RPC call to turn pump off for the room
          RPC.call("senseState", false, x);

          // Sensors should be level and we can now take measurements

          // Get oxygen value from sensor
          float oxygenValue = RPC.call("getO2Reading").as<float>();

          // Delay between reading values
          delay(READ_VALUE_DELAY);

          // Get carbon value from sensor
          float carbonValue = RPC.call("getCO2Reading").as<float>();

          // We should double check the state of the room and system before we evaluate
          systemOn = RPC.call("systemOn").as<int>();
          active = RPC.call("checkRoomActive", x).as<int>();
          if (systemOn == 0) {
            state = 4;
          }
          else if (active == 1) {
            // RPC call to evaluate the room based on sensor values
            RPC.call("evaluate", x, oxygenValue, carbonValue);
          }
          else {
            // Cycle to the next room
          }
        }

        if (state != 4) {
          // If just tested last room, set room to first room and wait between cycles
          if (x == (NUM_ROOMS - 1)) {
            x = 0;

            state = 1;
          }

          // If not last room, increment room and go to admin state
          else {
            x++;

            state = 1;
          }
        }

        // Check if calibration is necessary
        if (numTestsSinceCalibration == (CYCLES_PER_CALIBRATION * NUM_ROOMS)) {
          calibrationNecessary = true;
        }
        else {
          numTestsSinceCalibration++;
        }
      }
    break;

    // Stand by state *************************************************************************************
    // This state essentially acts as an "OFF" state for when the system should not do anything until the user turns it back "ON"
    case 4:

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= TIMER_DELAY) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        //RPC.call("updateDashM4", "'In state 4'");

        // RPC to get state of the system for redundancy
        systemOn = RPC.call("systemOn").as<int>();

        while (systemOn == 0) {
          systemOn = RPC.call("systemOn").as<int>();
        }

        // Go to administration state
        state = 1;
      }
    break;

    // "Wait between cycle" state *************************************************************************
    // This state acts as a simple rest state where no sensing is necessary in any of the rooms but the
    // system should still be active
    case 5: 

      // Set currentMillis
      currentMillis = millis();

      if ((currentMillis - previousMillis) >= TIMER_DELAY) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        //delay(WAIT_DELAY * DELAY_1_MIN);

        delay(1000);      // Temporary delay

        state = 1;
      }
    break;
  }
}

