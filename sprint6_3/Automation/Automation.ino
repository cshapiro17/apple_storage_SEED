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
void checkSensorLevel() {

  // This delay can be used to run the pump for a certain amount of time before checking levels
  delay(PUMP_DELAY);

  int secondsPumping = 0;

  bool sensorsLevel = false;

  int oldestValue = 0;
  int size = 3;
  float sumO2 = 0.0, sumCO2 = 0.0, meanO2, meanCO2, variationO2, variationCO2, sdO2, sdCO2;

  float o2Array[3] = {};
  float co2Array[3] = {};

  // Fill the arrays
  for (int i = 0; i < size; i++) {
    o2Array[i] = RPC.call("getO2Reading").as<float>();
    co2Array[i] = RPC.call("getCO2Reading").as<float>();

    // Delay to allow for more air to pump in
    delay(1000);
  }

  while(sensorsLevel) {

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
    if(sdO2 < 0.1 && sdCO2 < 0.1){ 

      // Sensors are level and while loop will break
      sensorsLevel = true;
    }
    else {
      
      // Replace oldest value
      o2Array[oldestValue] = RPC.call("getO2Reading").as<float>();
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

void setup() {

  RPC.begin();

  // For testing
  randomSeed(analogRead(0));

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

        /*
        RPC.println("In M4 state 1");
        delay(1000);
        */

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

        /*
        RPC.println("In M4 state 2");
        delay(1000);
        */

        /*
        Keep in mind that we will need to pull atmospheric air in first
        Then make sure the values are level
        Then call calibrate functions
        */

        // RPC call to calibrate the sensors
        RPC.call("calibrateSensors");

        delay(SIMULATE_CALIBRATION);

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

        /*
        RPC.println("In M4 state 3");
        delay(1000);
        */

        // Make RPC call to get room state
        int active = RPC.call("checkRoomActive", x).as<int>();

        if (active == 1) {

          // Make RPC call to turn pump on for the room
          RPC.call("senseState", true, x);

          //checkSensorLevel();
          delay(10000);

          RPC.call("senseState", false, x);

          // Sensors should be level and we can now take measurements

          // RPC call to evaluate the room based on sensor values
          RPC.call("evaluate", x);
        }

        // Change room 
        if (x == (NUM_ROOMS - 1)) {
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

        // Check to see if system power has been toggled off
        if (state != 4) {
          // Go to Administrative state
          state = 1;
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

        /*
        RPC.println("In M4 state 4");
        delay(1000);
        */

        // RPC to turn off all solenoids and pumps
        RPC.call("standByState");

        // RPC to get state of the system
        systemOn = RPC.call("systemOn").as<int>();

        while (systemOn == 0) {
          systemOn = RPC.call("systemOn").as<int>();
        }

        state = 1;
      }
    break;
  }

}

