// USES M4 CORE
// The M4 core does not have access to serial ports which means that RPCs are 
// needed to get sensor values

#include <Arduino.h>

#include "MillisTimerLib.h"
#include "C:\Users\Cmsha\OneDrive - University of Vermont\Semester 8\Capstone\apple_storage_SEED\apple_storage_SEED\apple_storage_controller_NO_O2\Config.h"
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

// Boolean representing if the sensors are reading level values in the sensing box
bool level;

// Values for carbon and oxygen
float oxygenValue;
float carbonValue;

// ***************************************************************************
// FUNCTIONS
// ***************************************************************************

/* Function which will monitor sensor readings and return true when the standard deviation of the last 3 values is low
 * Takes in nothing
 * Returns nothing
 */
bool checkSensorLevel(int x) {

  bool sensorsLevel = false;
  bool exit = false;

  int oldestValue = 0;
  int numMeasurements = 5;
  float sumO2 = 0.0, sumCO2 = 0.0, meanO2 = 0.0, meanCO2 = 0.0, variationO2 = 0.0, variationCO2 = 0.0, sdO2, sdCO2;

  float o2Array[numMeasurements] = {};
  float co2Array[numMeasurements] = {};
  
  /*
  We should pump in air for a set amount of time but we also want to check
  to see if the system has been updated in the dashbaord. 
  To do this, we will check every second and exit if necessary
  */
  for (int i = 0; i < PUMP_DELAY; i++) {
    int roomActive = RPC.call("checkRoomActive", x).as<int>();

    int pressure = 1000; // Default value for sensor

    //int pressure = analogRead(A0);

    if (roomActive == 0) {
      exit = true;

      return sensorsLevel;
    }
    else if (pressure < 870) {
      RPC.call("turnSystemOff");

      exit = true;

      return sensorsLevel;
    }
    else {
      delay(DELAY_1_SEC);
    }
  }

  // Fill the arrays
  for (int i = 0; i < numMeasurements; i++) {
    int roomActive = RPC.call("checkRoomActive", x).as<int>();

    int pressure = 1000; // Default value for sensor

    //int pressure = analogRead(A0);

    if (roomActive == 0) {
      exit = true;

      return sensorsLevel;
    }
    else if (pressure < 870) {
      RPC.call("turnSystemOff");

      exit = true;

      return sensorsLevel;
    }  

    o2Array[i] = RPC.call("getO2Reading").as<float>();

    delay(READ_VALUE_DELAY);

    co2Array[i] = RPC.call("getCO2Reading").as<float>();

    delay(READ_VALUE_DELAY);

    // Delay to allow for more air to pump in
    delay((CALIBRATION_VALUE_DELAY * DELAY_1_SEC) - (2 * READ_VALUE_DELAY));
  }

  while (!sensorsLevel) {
    // Reset sums
    sumO2 = 0.0;
    sumCO2 = 0.0;

    // Get sum of arrays
    for (int i = 0; i < numMeasurements; i++) {
      sumO2 += o2Array[i];
      sumCO2 += co2Array[i]; 
    }

    // Get mean of arrays
    meanO2 = sumO2 / numMeasurements;
    meanCO2 = sumCO2 / numMeasurements;

    variationO2 = 0.0;
    variationCO2 = 0.0;

    // Find variation
    for (int i = 0; i < numMeasurements; ++i) { 
          variationO2 += pow(o2Array[i] - meanO2, 2); 
          variationCO2 += pow(co2Array[i] - meanCO2, 2);
      } 

    // Find standard deviation
    sdO2 = sqrt(variationO2 / numMeasurements);
    sdCO2 = sqrt(variationCO2 / numMeasurements);

    // Check to see if the oxygen sensor and carbon dioxide sensor are consistent and not extraneous
    if(sdO2 < 0.02 && sdCO2 < 0.02 && sumO2 != 0.0 && sumO2 < 75.0 && sumCO2 != 0.0){ 

      // Sensors are level and while loop will break
      sensorsLevel = true;
      return sensorsLevel;
    }

    // Take an additional reading
    else {
      // First we want to see if the room is still active
      int roomActive = RPC.call("checkRoomActive", x).as<int>();

      int pressure = 1000;  // Temporary pressure value

      // int pressure = analogRead(A0);

      if (roomActive == 0) {
        exit = true;

        // sensorsLevel will return false if room is not active
        return sensorsLevel;
      }
      else if (pressure < 870) {
        // Shut the system off if pressure is too low
        RPC.call("turnSystemOff");

        exit = true;

        // sensorsLevel will return false if emergency exit due to pressure
        return sensorsLevel;
      }

      // If everything passes than we can get new readings

      o2Array[oldestValue] = RPC.call("getO2Reading").as<float>();

      delay(READ_VALUE_DELAY);

      co2Array[oldestValue] = RPC.call("getCO2Reading").as<float>();

      delay(READ_VALUE_DELAY);

      // Oldest value is now next value in the array
      if (oldestValue == (numMeasurements - 1)) {
        oldestValue = 0;
      }
      else {
        oldestValue++;
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

        /*
          Add more checks here as necessary

          - Maybe manual mode check
        */

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

        /*
        We will need to pull atmospheric air in first
        Then make sure the values are level
        Then call calibrate functions
        */

        // Turn the pump on to pull atm air into the sensing room
        RPC.call("senseState", true, 101);

        // Make sure that sensors have leveled
        level = checkSensorLevel(101);

        // The sensors should now be level and we can calibrate to atmospheric air
        
        // We should be making sure that the values being checked are near atm

        if (level) {
          // RPC call to calibrate the sensors
          int calibrated = RPC.call("calibrateSensors").as<int>();
        }

        // Turn the pump off and callibrate the sensors
        RPC.call("senseState", false, 101);

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

        // Make RPC call to get room state
        int active = RPC.call("checkRoomActive", x).as<int>();

        if (active == 1) {

          // Make RPC call to turn pump on for the room
          RPC.call("senseState", true, x);

          // Keep the pump on until the sensors have leveled
          level = checkSensorLevel(x);

          // Sensors should be level and we can now take measurements
          if (level) {

            // Get oxygen value from sensor
            oxygenValue = RPC.call("getO2Reading").as<float>();

            // Delay between reading values
            delay(READ_VALUE_DELAY);

            // Get carbon value from sensor
            carbonValue = RPC.call("getCO2Reading").as<float>();

            delay(READ_VALUE_DELAY);
          }

          // Make RPC call to turn pump off for the room
          RPC.call("senseState", false, x);
          
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

        int allRoomsDeactivated = RPC.call("allRoomsDeactivated").as<int>();

        if (state != 4) {
          // If just tested last room, set room to first room and wait between cycles
          if (x == (NUM_ROOMS - 1)) {
            x = 0;

            // Unless all rooms are deactivated
            if (allRoomsDeactivated == 0) {
              state = 5;    // Change to state 5 eventually
            }
            else {
              state = 1;
            }
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
        else if (allRoomsDeactivated == 0) {
          numTestsSinceCalibration++;
        }
        else {
          // Do not increment 
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

      // Will want to use the millisTimer to delay this state instead of delay func
      if ((currentMillis - previousMillis) >= 5000) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        state = 1;
      }
    break;
  }
}

