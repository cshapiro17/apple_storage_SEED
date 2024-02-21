/*Team 16--Apple Storage Controller MAIN Code for sprint 5 -- Sensing room able to pump in air from 2 rooms, 
* along with sense room flushing capabilities to make sure sensor is reading values properly.
* Also able to read both O2 and CO2, making changes to both. Has CO2 Sensor Integration
* Setup to work with Arduino Giga R1 Wifi
*/

#include "Config.h"
#include "System.h"

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
 * I think we want to check this at the beginning of each state
 */
void checkDashboardInput() {

  // Check if there is information in the serial buffer
  if (Serial.available()) {

    // Get value from serial
    String funcString = Serial.readString();

    if (funcString == "LED_ON") {
      // Create call to system function to do something
      digitalWrite(8, HIGH);
    }
    else if (funcString == "LED_OFF") {
      // Create call to system function to do something
      digitalWrite(8, LOW);
    }
    else if (funcString == "HONEYCRISP_ROOM1") {
      VRS.rooms[0].setAppleType(Honeycrisp);
    }
    else if (funcString == "FUJI_ROOM1") {
      VRS.rooms[0].setAppleType(Fuji);
    }
    else if (funcString == "MCINTOSH_ROOM1") {
      VRS.rooms[0].setAppleType(Mcintosh);
    }
    else if (funcString == "HONEYCRISP_ROOM2") {
      VRS.rooms[1].setAppleType(Honeycrisp);
    }
    else if (funcString == "FUJI_ROOM2") {
      VRS.rooms[1].setAppleType(Fuji);
    }
    else if (funcString == "MCINTOSH_ROOM2") {
      VRS.rooms[1].setAppleType(Mcintosh);
    }
    else {
      // Invalid input, something went wrong
    }
  }
  else {
    // No serial available
  }
}

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
    VRS.rooms[0].activate();                                                // Activate the room

    pinMode(VRS.rooms[0].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 1
    pinMode(VRS.rooms[0].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 1
    pinMode(VRS.rooms[0].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 1
  #endif

  // If room 2 is defined than set up room 2
  #ifdef ROOM_2_NAME
    VRS.addRoom(room2);                                                     // Add the room to the system
    VRS.rooms[1].activate();                                                // Activate the room

    pinMode(VRS.rooms[1].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 2
    pinMode(VRS.rooms[1].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 2
    pinMode(VRS.rooms[1].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 2
  #endif

  // If room 3 is defined than set up room 3
  #ifdef ROOM_3_NAME
    VRS.addRoom(room3);                                                     // Add the room to the system
    VRS.rooms[2].activate();                                                // Activate the room

    pinMode(VRS.rooms[2].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 3
    pinMode(VRS.rooms[2].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 3
    pinMode(VRS.rooms[2].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 3
  #endif

  // If room 4 is defined than set up room 4
  #ifdef ROOM_4_NAME
    VRS.addRoom(room4);                                                     // Add the room to the system
    VRS.rooms[3].activate();                                                // Activate the room

    pinMode(VRS.rooms[3].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 4
    pinMode(VRS.rooms[3].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 4
    pinMode(VRS.rooms[3].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 4
  #endif

  // If room 5 is defined than set up room 5
  #ifdef ROOM_5_NAME
    VRS.addRoom(room5);                                                     // Add the room to the system
    VRS.rooms[4].activate();                                                // Activate the room 

    pinMode(VRS.rooms[4].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 5
    pinMode(VRS.rooms[4].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 5
    pinMode(VRS.rooms[4].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 5
  #endif

  // If room 6 is defined than set up room 6
  #ifdef ROOM_6_NAME
    VRS.addRoom(room6);                                                     // Add the room to the system
    VRS.rooms[5].activate();                                                // Activate the room

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

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= TIMER_DELAY){
        
        // Set previousMillis for comparison later
        previousMillis = currentMillis;
        
        // Check to see if calibration is necessary
        if(calibrationNecessary){
   
          // Proceed to the calibration state
          state = 2;
        }
        else {

          // Report system status to dashboard
          updateDashboard("System OK");
  
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
        updateDashboard("Calibrating..."); 

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

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= TIMER_DELAY) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        // Check to see if the room in question is active
        if (VRS.rooms[x].isActive()) {

          // Report the room being tested to the dashboard
          updateDashboard("Testing room " + String(x + 1) + "...", "room" + String(x + 1), "", "", "", "", "");

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
          updateDashboard("System OK", "room" + String(x + 1), String(oxygenValue), String(CO2Percent), String(roomTemp), String(oxygenSolenoidState), String(nitrogenSolenoidState));
        }
        
        // Change room
        if (x >= (NUM_ROOMS - 1)) {
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
        state = 1;
      }
      
      break;
    } 
  }
