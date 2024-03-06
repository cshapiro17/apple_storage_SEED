// USES M7 CORE

#include "C:\Users\Cmsha\OneDrive - University of Vermont\Semester 8\Capstone\apple_storage_SEED\apple_storage_SEED\sprint6_3\Config.h"
#include "System.h"
#include "CO2Sensor.h"
#include "O2Sensor.h"
#include <RPC.h>

// *************************************************************************
// GLOBAL SYSTEM VARIABLES
// *************************************************************************

// Create system object
System VRS(SYSTEM_NAME, SYSTEM_PUMP_PIN, 0, NUM_ROOMS);

//***************************************************************************
// CREATE APPLES
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
  Room room1(ROOM_1_NAME, 1, TEST_APPLE, R1_O2_FAN_PIN, R1_N2_SOLENOID_PIN, R1_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 2 is defined than set up room 2
#ifdef ROOM_2_NAME
  Room room2(ROOM_2_NAME, 2, TEST_APPLE, R2_O2_FAN_PIN, R2_N2_SOLENOID_PIN, R2_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 3 is defined than set up room 3
#ifdef ROOM_3_NAME
  Room room3(ROOM_3_NAME, 3, TEST_APPLE, R3_O2_FAN_PIN, R3_N2_SOLENOID_PIN, R3_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 4 is defined than set up room 4
#ifdef ROOM_4_NAME
  Room room4(ROOM_4_NAME, 4, TEST_APPLE, R4_O2_FAN_PIN, R4_N2_SOLENOID_PIN, R4_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 5 is defined than set up room 5
#ifdef ROOM_5_NAME
  Room room5(ROOM_5_NAME, 5, TEST_APPLE, R5_O2_FAN_PIN, R5_N2_SOLENOID_PIN, R5_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 6 is defined than set up room 6
#ifdef ROOM_6_NAME
  Room room6(ROOM_6_NAME, 6, TEST_APPLE, R6_O2_FAN_PIN, R6_N2_SOLENOID_PIN, R6_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

//****************************************************************************
// CO2 VARIABLES
//****************************************************************************
CO2Sensor explorCO2;
float carbonValue;

//****************************************************************************
// OXYGEN VARIABLES
//****************************************************************************
float oxygenValue;
float currentO2;
float previousO2;
O2Sensor EZO2;

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

String dataString;       // String representing the data to Node-Red

// *****************************************************************************************
// FUNCTIONS
// *****************************************************************************************

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

void blink(int led, int delaySeconds) {
  for (int i = 0; i < 10; i++) {
    digitalWrite(led, LOW);
    delay(delaySeconds);
    digitalWrite(led, HIGH);
    delay(delaySeconds);
  }
  RPC.begin();
}

// ******************************************************************************
// RPC CALL FUNCTIONS
// ******************************************************************************

/* Function which gets the state of the system
 * Takes in nothing
 * Returns the state of the system as a bool
 */
int systemOn() {
  if (VRS.getSystemState()) {
    return 1;
  }
  else {
    return 0;
  }
}

/* Function which gets the state of the rooms
 * Takes in the room in question
 * Returns the state of the room
 */
int checkRoomActive(int x) {
  
  // Return the state of the room
  if (VRS.rooms[x].isActive()) {
    return 1;
  }
  else {
    return 0;
  }
}

/* Functions which turns on and off the pumps and solenoids used for sensing
 * Takes in the desired state of the pump and the room for the solenoid as an int
 * Returns nothing
 */
void senseState(bool state, int room) {

  // Set pump to the value of state
  VRS.pumpOn(state, VRS.rooms[room].getRoomNum() - 1);

  if (state) {
    updateDashboard("'Pump turned on'");
  }
  else {
    updateDashboard("'Pump turned off'");
  }

}

/* Function which turns off all solenoids and pumps to put the system in "Stand by" mode
 * Takes in nothing
 * Returns nothing
 */
void standByState() {

  // Make sure that everything is in "Stand by" mode by closing all solenoid valves
  for (int i = 0; i < NUM_ROOMS; i++) {
    VRS.rooms[i].setN2solState(0);
    VRS.rooms[i].setO2fanState(0);
    VRS.rooms[i].setSenseSolState(0);
  }

  // Turn off pump
  VRS.setPumpState(0);

  updateDashboard("'System in stand by mode'");
}

/* Function which calls the calibrate functions from the sensor libraries
 * Takes in nothing
 * Returns nothing
 */
void calibrateSensors() {
  
  // Call CO2 calibration function
  explorCO2.calibrate();

  updateDashboard("'CO2 calibrated'");
}

/* Function which returns the O2 reading from the RPC
 * Takes in nothing
 * Returns the oxygen percent as a float
 */
float getO2Reading() {
  return EZO2.getPercent();
}

/* Function which returns the CO2 reading from the RPC
 * Takes in nothing
 * Returns the carbon percent as a float
 */
float getCO2Reading() {
  return explorCO2.getPercent();
}

/* Function which evaluates the room and makes necessary changes
 * Takes in nothing
 * Returns nothing
 */
void evaluate(int x) {

  // Get oxygen value from sensor
  oxygenValue = EZO2.getPercent();

  // Get carbon dioxide value from sensor
  carbonValue = explorCO2.getPercent();

  // Get random temp because it is not implemented
  float roomTemp = random(25, 35);

  // Perform the evaluation
  VRS.rooms[x].evaluateRoom(oxygenValue, carbonValue);

  // Get the states of each solenoid
  int oxygenFanState = VRS.rooms[x].getO2fanState();
  int nitrogenSolenoidState = VRS.rooms[x].getN2solState();
          
  // Report measurements to dashboard
  updateDashboard("'System OK'", "room" + String(x + 1), String(oxygenValue), String(carbonValue), String(roomTemp), String(oxygenFanState), String(nitrogenSolenoidState));
}

void setup() {

  //*****************************************************************************
  // SETUP ROOMS
  // ****************************************************************************
  
  // If room 1 is defined than set up room 1
  #ifdef ROOM_1_NAME
    VRS.addRoom(room1);                                                     // Add the room to the system
    VRS.rooms[0].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    //VRS.rooms[0].activate(); // For testing

    pinMode(VRS.rooms[0].getOxygenFanPin(), OUTPUT);                        // Initialize the pin for the oxygen solenoid in room 1
    pinMode(VRS.rooms[0].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 1
    pinMode(VRS.rooms[0].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 1
  #endif

  // If room 2 is defined than set up room 2
  #ifdef ROOM_2_NAME
    VRS.addRoom(room2);                                                     // Add the room to the system
    VRS.rooms[1].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    //VRS.rooms[1].activate(); // For testing

    pinMode(VRS.rooms[1].getOxygenFanPin(), OUTPUT);                        // Initialize the pin for the oxygen solenoid in room 2
    pinMode(VRS.rooms[1].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 2
    pinMode(VRS.rooms[1].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 2
  #endif

  // If room 3 is defined than set up room 3
  #ifdef ROOM_3_NAME
    VRS.addRoom(room3);                                                     // Add the room to the system
// VRS.rooms[2].deactivate();                                               // Room shall be deactivated until activated in the dashboard

    VRS.rooms[2].activate(); // For testing

    pinMode(VRS.rooms[2].getOxygenFanPin(), OUTPUT);                        // Initialize the pin for the oxygen solenoid in room 3
    pinMode(VRS.rooms[2].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 3
    pinMode(VRS.rooms[2].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 3
  #endif

  // If room 4 is defined than set up room 4
  #ifdef ROOM_4_NAME
    VRS.addRoom(Room);                                                      // Add the room to the system
    // VRS.rooms[3].deactivate();                                              // Room shall be deactivated until activated in the dashboard
        
    VRS.rooms[3].activate(); // For testing

    pinMode(VRS.rooms[3].getOxygenFanPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 4
    pinMode(VRS.rooms[3].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 4
    pinMode(VRS.rooms[3].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 4
  #endif

  // If room 5 is defined than set up room 5
  #ifdef ROOM_5_NAME
    VRS.addRoom(room5);                                                     // Add the room to the system
    // VRS.rooms[4].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    VRS.rooms[4].activate(); // For testing
 

    pinMode(VRS.rooms[4].getOxygenFanPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 5
    pinMode(VRS.rooms[4].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 5
    pinMode(VRS.rooms[4].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 5
  #endif

  // If room 6 is defined than set up room 6
  #ifdef ROOM_6_NAME
    VRS.addRoom(room6);                                                     // Add the room to the system
    // VRS.rooms[5].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    VRS.rooms[5].activate(); // For testing

    pinMode(VRS.rooms[5].getOxygenFanPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 6
    pinMode(VRS.rooms[5].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 6
    pinMode(VRS.rooms[5].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 6
  #endif

  //*****************************************************************************
  // GENERAL SETUP
  // ****************************************************************************

  // Initialize "Remote Procedure Call" library
  //RPC.begin();

  // Initialize serial connection @ 9600 Baud Rate for serial monitor/dashboard communication
  // Initialize serial connection @ 9600 Baud Rate for serial monitor
  Serial.begin(9600);
  // Wait for serial port to open
  while(!Serial);

  //initialize Serial1 communication @ 9600 Baud Rate for CO2 sensor (RX/TX 0)
  Serial1.begin(9600);
  // Wait for serial1 port to open
  while(!Serial1);

  //initialize Serial2 communication @ 9600 Baud Rate for O2 sensor (RX/TX 1)
  Serial2.begin(9600);
  // Wait for serial1 port to open
  while(!Serial2);

  // Initialize CO2 sensor
  explorCO2.initialize();
  delay(CO2_CONNECT_DELAY);

  // Initialize O2 sensor
  EZO2.initialize();
  delay(O2_CONNECT_DELAY);

  // ******************************************************************************
  // SETUP RPC FUNCTIONS
  // ******************************************************************************

  RPC.bind("systemOn", systemOn);
  RPC.bind("checkRoomActive", checkRoomActive);
  RPC.bind("senseState", senseState);
  RPC.bind("standByState", standByState);
  RPC.bind("calibrateSensors", calibrateSensors);
  RPC.bind("getO2Reading", getO2Reading);
  RPC.bind("getCO2Reading", getCO2Reading);
  RPC.bind("evaluate", evaluate);

  // Initiation sequence
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);

  if (HAL_GetCurrentCPUID() == CM7_CPUID) {
    blink(LEDB, 100); //blink blue LED (M7 core)
    } 
  else {
    blink(LEDG, 100); //blink green LED (M4 core)
  }

  updateDashboard("'Set up complete'");
}

// ********************************************************************************
// Loop
// ********************************************************************************
void loop() {

  String buffer = "";
  while (RPC.available()) {
    buffer += (char)RPC.read();  // Fill the buffer with characters
  }
  if (buffer.length() > 0) {
    Serial.println(buffer);
  }

  // ------------------------------------------------------------------------------
  // Check for Serial messages
  // ------------------------------------------------------------------------------
  if (Serial.available()) {

    // Get value from serial
    String funcString = Serial.readString();

    // Parse the message
    int index = funcString.indexOf("_");
    String keyword = funcString.substring(0, index);
    String operation = funcString.substring(index + 1, funcString.length());
    int room_num = operation.toInt();

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
      VRS.setSystemState(true);

      updateDashboard("'System turned on'");
    }
    else if (keyword == "systemOff") {
      VRS.setSystemState(false);

      updateDashboard("'System turned off'");
    }
    else if (keyword == "activateRoom") {
      VRS.rooms[room_num - 1].activate();

      updateDashboard("'Room " + String(room_num) + " activated'");
      delay(DASH_DELAY);
    }
    else if (keyword == "deactivateRoom") {
      VRS.rooms[room_num - 1].deactivate();

      updateDashboard("'Room " + String(room_num) + " deactivated'");
      delay(DASH_DELAY);
    }
    else if (keyword == "oxygenSolOff") {
      VRS.rooms[room_num - 1].setO2fanState(0);

      updateDashboard("'Room " + String(room_num) + " oxygen solenoid closed'");
    }
    else if (keyword == "nitrogenSolOff") {
      VRS.rooms[room_num - 1].setN2solState(0);

      updateDashboard("'Room " + String(room_num) + " nitrogen solenoid closed'");
    }
    else if (keyword == "oxygenSolOn") {
      VRS.rooms[room_num - 1].setO2fanState(1);

      updateDashboard("'Room " + String(room_num) + " oxygen solenoid opened'");
    }
    else if (keyword == "nitrogenSolOn") {
      VRS.rooms[room_num - 1].setN2solState(1);

      updateDashboard("'Room " + String(room_num) + " nitrogen solenoid opened'");
    }

    else {
      // Invalid input, something went wrong
      updateDashboard("Error! Invalid input command from Node-Red.");
    }
  }
  else {
    // No serial available
  }
}






