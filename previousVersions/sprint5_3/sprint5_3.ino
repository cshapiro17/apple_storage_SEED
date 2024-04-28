/*Team 16--Apple Storage Controller MAIN Code for sprint 5 -- Sensing room able to pump in air from 2 rooms, 
* along with sense room flushing capabilities to make sure sensor is reading values properly.
* Also able to read both O2 and CO2, making changes to both. Has CO2 Sensor Integration
* Setup to work with Arduino Giga R1 Wifi
*/

#include "Config.h"
#include "System.h"

#include <cmath>

#include "MillisTimerLib.h"
#include "DFRobot_OxygenSensor.h"
#include "CO2Sensor.h"


// **************************************************************************
// GLOBAL SYSTEM VARIABLES
// **************************************************************************

// Create system object
System VRS(SYSTEM_NAME, SYSTEM_PUMP_PIN, 0, NUM_ROOMS);

// Set calibrate to active
boolean calibrationNecessary = true;

// Intialize previous millis and current millis
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

// Initialize the room under test variable to start at the first room
int x = 0;

// Initialize the number of tests since last calibration
int numTestsSinceCalibration = 0;

bool systemOn = false;

// The variable which holds the state of the system
static unsigned int state;


//****************************************************************************
// CO2 VARIABLES
//****************************************************************************
CO2Sensor explorCO2;
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

//***************************************************************************
// CREATE APPLE
//*************************************************************************** 
Apple TEST_APPLE("TEST_APPLE", 0, 0, 0, 0);

// Apple objects being used for testing (These are not accurate numbers)
Apple Honeycrisp("Honeycrisp", 3, 1, 2, 0);
Apple Fuji("Fuji", 6, 4, 5, 3);
Apple Mcintosh("Mcintosh", 9, 7, 8, 6);
  
//***************************************************************************
// CREATE ROOMS
//***************************************************************************
// If room 1 is defined than set up room 1
#ifdef ROOM_1_NAME
  Room4 room1(ROOM_1_NAME, 1, TEST_APPLE, R1_O2_SOLENOID_PIN, R1_N2_SOLENOID_PIN, R1_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 2 is defined than set up room 2
#ifdef ROOM_2_NAME
  Room4 room2(ROOM_2_NAME, 2, TEST_APPLE, R2_O2_SOLENOID_PIN, R2_N2_SOLENOID_PIN, R2_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 3 is defined than set up room 3
#ifdef ROOM_3_NAME
  Room4 room3(ROOM_3_NAME, 3, TEST_APPLE, R3_O2_SOLENOID_PIN, R3_N2_SOLENOID_PIN, R3_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 4 is defined than set up room 4
#ifdef ROOM_4_NAME
  Room4 room4(ROOM_4_NAME, 4, TEST_APPLE, R4_O2_SOLENOID_PIN, R4_N2_SOLENOID_PIN, R4_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 5 is defined than set up room 5
#ifdef ROOM_5_NAME
  Room4 room5(ROOM_5_NAME, 5, TEST_APPLE, R5_O2_SOLENOID_PIN, R5_N2_SOLENOID_PIN, R5_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 6 is defined than set up room 6
#ifdef ROOM_6_NAME
  Room4 room6(ROOM_6_NAME, 6, TEST_APPLE, R6_O2_SOLENOID_PIN, R6_N2_SOLENOID_PIN, R6_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif


// *****************************************************************************************
// FUNCTIONS
// *****************************************************************************************

/* Function which will monitor sensor readings and return true when the standard deviation of the last 3 values is low
 * Takes in nothing
 * Returns nothing
 */
void checkSensorLevel() {

  // This delay can be used to run the pump for a certain amount of time before checking levels
  delay(PUMP_DELAY);

  int secondsPumping = 0;

  while (secondsPumping < PUMP_DELAY) {
    checkDashboardInput();

    if (!systemOn) {

    }
  }

  bool sensorsLevel = false;

  int oldestValue = 0;
  int size = 3;
  float sumO2 = 0.0, sumCO2 = 0.0, meanO2, meanCO2, variationO2, variationCO2, sdO2, sdCO2;

  float o2Array[3] = {};
  float co2Array[3] = {};

  // Fill the arrays
  for (int i = 0; i < size; i++) {
    o2Array[i] = oxygen.getOxygenData(collectNumber);
    co2Array[i] = explorCO2.getPercent();

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
      o2Array[oldestValue] = oxygen.getOxygenData(collectNumber);
      co2Array[oldestValue] = explorCO2.getPercent();

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

/* Function which collects data to send to the dashboard
 * Takes in the messages which are sent to the dashboard
 * Nothing
 */
void updateDashboard(String sysUpdate = "", String room = "", String oxygen = "", String carbon = "", String temperature = "", String o2state = "", String n2state = "") {

  // Check for new information and if any exists than update the dashboard
  if (room != "") {
    dataArray[0] = room;                          // Specify room for update
  }
  else {
    dataArray[0] = "empty";
  }

  if (oxygen != "") {
    dataArray[1] = oxygen;                        // Create oxygen update
  }
  else {
    dataArray[1] = "empty";
  }

  if (carbon != "") {
    dataArray[2] = carbon;                        // Create carbon update
  }
  else {
    dataArray[2] = "empty";
  }

  if (temperature != "") {
    dataArray[3] = temperature;                   // Create temperature update
  }
  else {
    dataArray[3] = "empty";
  }

  if (o2state != "") {
    dataArray[4] = o2state;                       // Create update about oxygen solenoid
  }
  else {
    dataArray[4] = "empty";
  }

  if (n2state != "") {
    dataArray[5] = n2state;                       // Create update about nitrogen solenoid   
  }
  else {
    dataArray[5] = "empty";
  }

  if (sysUpdate != "") {
    dataArray[6] = sysUpdate;                     // Create update about system
  }  
  
  dataString = "";                                // Reset dataString
  dataString = arrayToString(dataArray);          // Convert data to string
  Serial.println(dataString);                     // Send update
}

/* Function which reads button and switch output from the dashboard and executes some output
 * Takes in nothing
 * Returns nothing
 * 
 * We want to check this as much as possible
 */
void checkDashboardInput() {

  // Check if there is information in the serial buffer
  if (Serial.available()) {

    // Get value from serial
    String funcString = Serial.readString();

    // Parse the message
    int index = funcString.indexOf("_");
    String keyword = funcString.substring(0, index);
    String operation = funcString.substring(index + 1, funcString.length());
    int room_num = operation.toInt();

    /****** In progress ******
    if (operation.length() == 1) {
        int room_num = operation.toInt();
    }
    else {
      int room_num = -1;
    }
    */

    if (keyword == "honeycrisp") {
      VRS.rooms[room_num - 1].setAppleType(Honeycrisp);

      updateDashboard("'Room " + String(room_num) + " apple honeycrisp'");
      delay(DASH_DELAY);
    }
    else if (keyword == "fuji") {
      VRS.rooms[room_num - 1].setAppleType(Fuji);

      updateDashboard("'Room " + String(room_num) + " apple fuji'");
      delay(DASH_DELAY);
    }
    else if (keyword == "mcintosh") {
      VRS.rooms[room_num - 1].setAppleType(Mcintosh);

      updateDashboard("'Room " + String(room_num) + " apple mcintosh'");
      delay(DASH_DELAY);
    }
    else if (keyword == "systemOn") {
      systemOn = true;
    }
    else if (keyword == "systemOff") {
      systemOn = false;
    }
    else if (keyword == "activateRoom") {
      VRS.rooms[room_num - 1].activate();

      updateDashboard("'Room " + String(room_num) + " activated'");
    }
    else if (keyword == "deactivateRoom") {
      VRS.rooms[room_num - 1].deactivate();

      updateDashboard("'Room " + String(room_num) + " deactivated'");
    }
    else if (keyword == "oxygenSolOff") {
      VRS.rooms[room_num - 1].setO2solState(0);

      updateDashboard("'Room " + String(room_num) + " oxygen solenoid closed'");
    }
    else if (keyword == "nitrogenSolOff") {
      VRS.rooms[room_num - 1].setN2solState(0);

      updateDashboard("'Room " + String(room_num) + " nitrogen solenoid closed'");
    }
    else if (keyword == "oxygenSolOn") {
      VRS.rooms[room_num - 1].setO2solState(1);

      updateDashboard("'Room " + String(room_num) + " oxygen solenoid opened'");
    }
    else if (keyword == "nitrogenSolOn") {
      VRS.rooms[room_num - 1].setN2solState(1);

      updateDashboard("'Room " + String(room_num) + " nitrogen solenoid opened'");
    }

    /****** In progress ******
    else if (keyword == "modifyApples") {
      modifyApples(operation);
    }
    */

    else {
      // Invalid input, something went wrong
    }
  }
  else {
    // No serial available
  }
}

/****** In progress ******
void modifyApples(String appleString) {
  int index = appleString.indexOf("/");

  Apple appleName = appleString.substring(0, index);

  String newString = appleString.substring(index + 1, appleString.length());
  index = newString.indexOf("/");

  String highO2 = newString.substring(0, index);
  float highO2f = highO2.toFloat();
  appleName.setOxygenHighLevel(highO2f);
  
  newString == appleString.substring(index + 1, newString.length());
  index = newString.indexOf("/");

  String lowO2 = newString.substring(0, index);
  float lowO2f = lowO2.toFloat();
  appleName.setOxygenLowLevel(lowO2f);

  newString == appleString.substring(index + 1, newString.length());
  index = newString.indexOf("/");

  String highCO2 = newString.substring(0, index);
  float highCO2f = highCO2.toFloat();
  appleName.setCarbonHighLevel(highCO2f);

  String lowCO2 == appleString.substring(index + 1, newString.length());
  float lowCO2f = lowCO2.toFloat();
  appleName.setCarbonLowLevel(lowCO2f);
}
*/

void setup() {

  //*****************************************************************************
  // TEST PINS
  //*****************************************************************************
  pinMode(8, OUTPUT);

  //*****************************************************************************
  // SETUP ROOMS
  // ****************************************************************************
  // If room 1 is defined than set up room 1
  #ifdef ROOM_1_NAME
    VRS.addRoom(room1);                                                     // Add the room to the system
    // VRS.rooms[0].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    VRS.rooms[0].activate(); // For testing

    pinMode(VRS.rooms[0].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 1
    pinMode(VRS.rooms[0].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 1
    pinMode(VRS.rooms[0].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 1
  #endif

  // If room 2 is defined than set up room 2
  #ifdef ROOM_2_NAME
    VRS.addRoom(room2);                                                     // Add the room to the system
    // VRS.rooms[1].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    VRS.rooms[1].activate(); // For testing

    pinMode(VRS.rooms[1].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 2
    pinMode(VRS.rooms[1].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 2
    pinMode(VRS.rooms[1].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 2
  #endif

  // If room 3 is defined than set up room 3
  #ifdef ROOM_3_NAME
    VRS.addRoom(room3);                                                     // Add the room to the system
    // VRS.rooms[2].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    VRS.rooms[2].activate(); // For testing

    pinMode(VRS.rooms[2].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 3
    pinMode(VRS.rooms[2].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 3
    pinMode(VRS.rooms[2].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 3
  #endif

  // If room 4 is defined than set up room 4
  #ifdef ROOM_4_NAME
    VRS.addRoom(room4);                                                     // Add the room to the system
    // VRS.rooms[3].deactivate();                                              // Room shall be deactivated until activated in the dashboard
        
    VRS.rooms[3].activate(); // For testing

    pinMode(VRS.rooms[3].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 4
    pinMode(VRS.rooms[3].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 4
    pinMode(VRS.rooms[3].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 4
  #endif

  // If room 5 is defined than set up room 5
  #ifdef ROOM_5_NAME
    VRS.addRoom(room5);                                                     // Add the room to the system
    // VRS.rooms[4].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    VRS.rooms[4].activate(); // For testing
 

    pinMode(VRS.rooms[4].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 5
    pinMode(VRS.rooms[4].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 5
    pinMode(VRS.rooms[4].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 5
  #endif

  // If room 6 is defined than set up room 6
  #ifdef ROOM_6_NAME
    VRS.addRoom(room6);                                                     // Add the room to the system
    // VRS.rooms[5].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    VRS.rooms[5].activate(); // For testing

    pinMode(VRS.rooms[5].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 6
    pinMode(VRS.rooms[5].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 6
    pinMode(VRS.rooms[5].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 6
  #endif

  //*****************************************************************************
  // GENERAL SETUP
  // ****************************************************************************
  // Initialize serial connection @ 9600 Baud Rate for serial monitor/dashboard communication
  Serial.begin(9600);
  // Wait for serial port to open
  while(!Serial);
  
  // Initialize Serial1 communication @ 9600 Baud Rate for CO2 sensor (RX/TX 0)
  Serial1.begin(9600);
  // Wait for serial1 port to open
  while(!Serial1);

  // EXPLORIR CO2 connection
  explorCO2.initialize();
  delay(CO2_CONNECT_DELAY); 

  // Rand num generator
  randomSeed(analogRead(0)); 

  updateDashboard("'Setup Complete'");
  delay(DASH_DELAY);               

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
    
    // Administrative State **********************************************************************************
    // This state performs system checks to make sure that the system is OK
    case 1:

      // Check dashboard input
      checkDashboardInput();

      // Debug statement
      updateDashboard("'In State 1'");
      delay(DASH_DELAY);

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= TIMER_DELAY){
        
        // Set previousMillis for comparison later
        previousMillis = currentMillis;
        
        // Check to see if the system is on
        if (!systemOn) {

          // Proceed to stand by state
          state = 4;
        }
        // Check to see if calibration is necessary
        else if (calibrationNecessary){
   
          // Proceed to the calibration state
          state = 2;
        }
        else {

          // Report system status to dashboard
          updateDashboard("'System OK'");
  
          // Proceed to the measurement state
          state = 3;
        }
      }
    break;

    // Calibration State ********************************************************************************
    case 2:

      // Check dashboard input
      checkDashboardInput();

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= TIMER_DELAY) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;
        
        // All code relavent to calibrating the sensors will be located here

        // Report the calibration status to the dashboard
        updateDashboard("'Calibrating...'"); 

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

      // Check dashboard input
      checkDashboardInput();

      // Debug statement
      updateDashboard("'In State 3'");
      delay(1000);

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= TIMER_DELAY) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        // Check to see if the room in question is active
        if (VRS.rooms[x].isActive()) {

          // Report the room being tested to the dashboard
          updateDashboard("'Testing room " + String(x + 1) + "...'", "room" + String(x + 1), "", "", "", "", "");

          // This turns on the pump to pull in air into the measuring environment
          VRS.pumpOn(true, VRS.rooms[x].getRoomNum() - 1);

          // Call function to make sure sensors are level before proceeding
          //checkSensorLevel();
          delay(10000);

          // When sensors are leveled, we can turn off the pump
          VRS.pumpOn(false, VRS.rooms[x].getRoomNum() - 1);

          // Sensors should be level and we can now take measurements
          
          //oxygenValue = oxygen.getOxygenData(collectNumber);                       // Get oxygen percentage
          oxygenValue = random(0, 10);
          CO2Percent = explorCO2.getPercent();                                     // Get percentage of CO2
          //CO2Percent = 0.4;
          float roomTemp = random(0, 15);                                                   // Placeholder for temperature
     
          // Evaluate the room, this function will set the states of the solenoids to their necessary positions (open or closed)
          VRS.rooms[x].evaluateRoom(oxygenValue, CO2Percent);

          // Get the states of each solenoid
          int oxygenSolenoidState = VRS.rooms[x].getO2solState();
          int nitrogenSolenoidState = VRS.rooms[x].getN2solState();

          // Report measurements to dashboard
          updateDashboard("'System OK'", "room" + String(x + 1), String(oxygenValue), String(CO2Percent), String(roomTemp), String(oxygenSolenoidState), String(nitrogenSolenoidState));
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

      // Debug statement
      updateDashboard("'In State 4'");
      delay(1000);

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= TIMER_DELAY) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        // Make sure that everything is in "Stand by" mode by closing all solenoid valves
        for (int i = 0; i < NUM_ROOMS; i++) {
          VRS.rooms[i].setN2solState(0);
          VRS.rooms[i].setO2solState(0);
          VRS.rooms[i].setSenseSolState(0);
        }

        // Turn off the pump
        VRS.setPumpState(0);

        updateDashboard("'System Off'");

        while (!systemOn) {
          // Wait until systemOn is true
          checkDashboardInput();
        }

        updateDashboard("'System On'");

        state = 1;
      }
    break;

    // Wait between cycles state **************************************************************************
    // This is the state which the system exists in between measurement cycles
    // All previously opened solenoids should stay open during this state unless manually closed using dashboard
    case 5:

      // Set currentmillis
      currentMillis = millis();

      if ((currentMillis - previousMillis) >= TIMER_DELAY) {
        
        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        int secondsToWait = 0;

        // We want to tell the user something about the state of the system
        updateDashboard("'Waiting between cycles'");

        while (secondsToWait < SECONDS_BETWEEN_CYCLE) {
          checkDashboardInput();

          // Move to power off state if system is toggled off
          if (!systemOn) {
            state = 4;
            continue;
          }

          delay(1000);

          // Increment time
          secondsToWait++;
        }
      }
    break;
  }
}
