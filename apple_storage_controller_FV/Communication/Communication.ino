// USES M7 CORE

/*
  Code which communicates with the Raspberry Pi and state machine code to modify the system
*/

#include "C:\Users\Cmsha\OneDrive - University of Vermont\Semester 8\Capstone\apple_storage_SEED\apple_storage_SEED\apple_storage_controller_NO_O2\Config.h"
#include "System.h"
#include "CO2Sensor.h"
#include "O2Sensor.h"
#include <RPC.h>

// *************************************************************************
// GLOBAL SYSTEM VARIABLES
// *************************************************************************

// Create system object
System VRS(SYSTEM_NAME, SYSTEM_PUMP_PIN, 0, SYSTEM_SOLENOID_PIN, 0, NUM_ROOMS);

//***************************************************************************
// CREATE APPLES
//*************************************************************************** 
Apple DEFAULT("Default", 0, 0, 0, 0);

// Apple objects being used for testing (Defaults to 0 to test Node-Red sliders)
Apple Cortland("Cortland", 0, 0, 0, 0);
Apple Empire("Empire", 0, 0, 0, 0);
Apple Gala("Gala", 0, 0, 0, 0);
Apple Honeycrisp("Honeycrisp", 0, 0, 0, 0);
Apple Macoun("Macoun", 0, 0, 0, 0);
Apple Mcintosh("Mcintosh", 0, 0, 0, 0);

// Custom Apples
Apple Custom1("Custom1", 0, 0, 0, 0);
Apple Custom2("Custom2", 0, 0, 0, 0);
Apple Custom3("Custom3", 0, 0, 0, 0);
Apple Custom4("Custom4", 0, 0, 0, 0);
Apple Custom5("Custom5", 0, 0, 0, 0);
Apple Custom6("Custom6", 0, 0, 0, 0);


//***************************************************************************
// CREATE ROOMS
//***************************************************************************
// If room 1 is defined than set up room 1
#ifdef ROOM_1_NAME
  Room room1(ROOM_1_NAME, 1, DEFAULT, R1_O2_FAN_PIN, R1_N2_SOLENOID_PIN, R1_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 2 is defined than set up room 2
#ifdef ROOM_2_NAME
  Room room2(ROOM_2_NAME, 2, DEFAULT, R2_O2_FAN_PIN, R2_N2_SOLENOID_PIN, R2_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 3 is defined than set up room 3
#ifdef ROOM_3_NAME
  Room room3(ROOM_3_NAME, 3, DEFAULT, R3_O2_FAN_PIN, R3_N2_SOLENOID_PIN, R3_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 4 is defined than set up room 4
#ifdef ROOM_4_NAME
  Room room4(ROOM_4_NAME, 4, DEFAULT, R4_O2_FAN_PIN, R4_N2_SOLENOID_PIN, R4_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 5 is defined than set up room 5
#ifdef ROOM_5_NAME
  Room room5(ROOM_5_NAME, 5, DEFAULT, R5_O2_FAN_PIN, R5_N2_SOLENOID_PIN, R5_SENSING_SOLENOID_PIN, 0, 0, 0);
#endif

// If room 6 is defined than set up room 6
#ifdef ROOM_6_NAME
  Room room6(ROOM_6_NAME, 6, DEFAULT, R6_O2_FAN_PIN, R6_N2_SOLENOID_PIN, R6_SENSING_SOLENOID_PIN, 0, 0, 0);
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
  "",                    // Room under test [1]
  "",                    // Oxygen Value [2]
  "",                    // Carbon Value [3]
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
void updateDashboard(String sysUpdate, String room, String roomUnderTest, String oxygen, String carbon, String o2state, String n2state) {

  // Check for new information and if any exists than update the dashboard
  if (room != "NA") {
    dataArray[0] = room;                          // Specify room for update
  }
  else {
    dataArray[0] = "empty";
  }

  if (roomUnderTest != "NA") {
    dataArray[1] = roomUnderTest;
  }
  else {
    dataArray[1] = "empty";
  }

  if (oxygen != "NA") {
    dataArray[2] = oxygen;                        // Create oxygen update
  }
  else {
    dataArray[2] = "empty";
  }

  if (carbon != "NA") {
    dataArray[3] = carbon;                        // Create carbon update
  }
  else {
    dataArray[3] = "empty";
  }

  if (o2state != "NA") {
    dataArray[4] = o2state;                       // Create update about oxygen solenoid
  }
  else {
    dataArray[4] = "empty";
  }

  if (n2state != "NA") {
    dataArray[5] = n2state;                       // Create update about nitrogen solenoid   
  }
  else {
    dataArray[5] = "empty";
  }

  if (sysUpdate != "NA") {
    dataArray[6] = sysUpdate;                     // Create update about system
  }  
  else {
    dataArray[6] = "oof";
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
  // If the "room" is the atmospheric solenoid than it will always be active
  if (x == 101) {
    return 1;
  }
  
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
void senseState(bool state, int x) {

  // Set under test variable
  VRS.rooms[x].setUnderTest(state);

    // Set pump to the value of state
  VRS.pumpOn(state, x);

  if (x == 101 && state == true) {
    updateDashboard("'Calibrating sensors'", "NA", "NA", "NA", "NA", "NA", "NA");
  }
  else if (x == 101 && state == false) {
    updateDashboard("'Calibration finished'", "NA", "NA", "NA", "NA", "NA", "NA");
  }
  else if (state) {
    updateDashboard("'Pump turned on'", "room" + String(x + 1), "true", "NA", "NA", "NA", "NA");
  }
  else {
    updateDashboard("'Pump turned off'", "room" + String(x + 1), "NA", "NA", "NA", "NA", "NA");
  }
}

/* Function which calls the calibrate functions from the sensor libraries
 * Takes in nothing
 * Returns a value to signify the end of the function
 */
int calibrateSensors() {
  
  // Call CO2 calibration function
  explorCO2.calibrate();

  // Call O2 calibration function
  //EZO2.calibrate();               // This sensor is factor calibrated so we do not need to calibrate it for approx. 1 year as of 4/15/2024

  // When finished return a value
  return 1;
}

/* Function which returns the O2 reading from the RPC
 * Takes in nothing
 * Returns the oxygen percent as a float
 */
float getO2Reading() {
  // This is what we would normally do
  //return EZO2.getPercent();

  // For the sake of the demo, we want to be able to manipulate the O2 values

  // Read the analog value of the potentiometer
  float potentVal = analogRead(POTENTIOMETER_DEMO_PIN);

  float fakeO2 = map(potentVal, 0, 1023, 1.00, 4.00);

  //float fakeO2 = 2.00;

  //Serial.println("O2: " + String(fakeO2));

  return fakeO2;
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
void evaluate(int x, float oxygenValue, float carbonValue) {

  // Perform the evaluation
  VRS.rooms[x].evaluateRoom(oxygenValue, carbonValue);

  // Get the states of each solenoid
  int oxygenFanState = VRS.rooms[x].getO2fanState();
  int nitrogenSolenoidState = VRS.rooms[x].getN2solState();
          
  // Report measurements to dashboard
  updateDashboard("'Done taking reading'", "room" + String(x + 1), "false", String(oxygenValue), String(carbonValue), String(oxygenFanState), String(nitrogenSolenoidState));
}

/* Function which turns the system off, can be called from the state machine
 * Takes in nothing
 * Returns nothing
 */
void turnSystemOff() {
  VRS.setSystemState(false);

  updateDashboard("'System turned off'", "NA", "NA", "NA", "NA", "NA", "NA");
}

/* Function which checks to see if all of the rooms are deactivated
 * Takes in nothing
 * Returns 1 if all roooms are deactivated and 0 if at least one is active
 */
int allRoomsDeactivated() {

  for (int x = 0; x < NUM_ROOMS; x++) {
    if (VRS.rooms[x].isActive()) {
      // If some are active
      return 0;
    }
  }
  
  // If none are active
  return 1;
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
    VRS.rooms[2].deactivate();                                               // Room shall be deactivated until activated in the dashboard

    //VRS.rooms[2].activate(); // For testing

    pinMode(VRS.rooms[2].getOxygenFanPin(), OUTPUT);                        // Initialize the pin for the oxygen solenoid in room 3
    pinMode(VRS.rooms[2].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 3
    pinMode(VRS.rooms[2].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 3
  #endif

  // If room 4 is defined than set up room 4
  #ifdef ROOM_4_NAME
    VRS.addRoom(room4);                                                      // Add the room to the system
    VRS.rooms[3].deactivate();                                              // Room shall be deactivated until activated in the dashboard
        
    //VRS.rooms[3].activate(); // For testing

    pinMode(VRS.rooms[3].getOxygenFanPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 4
    pinMode(VRS.rooms[3].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 4
    pinMode(VRS.rooms[3].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 4
  #endif

  // If room 5 is defined than set up room 5
  #ifdef ROOM_5_NAME
    VRS.addRoom(room5);                                                     // Add the room to the system
    VRS.rooms[4].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    //VRS.rooms[4].activate(); // For testing
 

    pinMode(VRS.rooms[4].getOxygenFanPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 5
    pinMode(VRS.rooms[4].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 5
    pinMode(VRS.rooms[4].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 5
  #endif

  // If room 6 is defined than set up room 6
  #ifdef ROOM_6_NAME
    VRS.addRoom(room6);                                                     // Add the room to the system
    VRS.rooms[5].deactivate();                                              // Room shall be deactivated until activated in the dashboard

    //VRS.rooms[5].activate(); // For testing

    pinMode(VRS.rooms[5].getOxygenFanPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 6
    pinMode(VRS.rooms[5].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 6
    pinMode(VRS.rooms[5].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 6
  #endif

  //*****************************************************************************
  // GENERAL SETUP
  // ****************************************************************************

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
  //EZO2.initialize();
  delay(O2_CONNECT_DELAY);


  // ******************************************************************************
  // SETUP RPC FUNCTIONS
  // ******************************************************************************

  RPC.bind("systemOn", systemOn);
  RPC.bind("checkRoomActive", checkRoomActive);
  RPC.bind("senseState", senseState);
  RPC.bind("calibrateSensors", calibrateSensors);
  RPC.bind("getO2Reading", getO2Reading);
  RPC.bind("getCO2Reading", getCO2Reading);
  RPC.bind("evaluate", evaluate);
  RPC.bind("turnSystemOff", turnSystemOff);
  RPC.bind("allRoomsDeactivated", allRoomsDeactivated);

  // Initiation sequence
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);

  if (HAL_GetCurrentCPUID() == CM7_CPUID) {
    blink(LEDB, 100); //blink blue LED (M7 core)
    } 
  else {
    blink(LEDG, 100); //blink green LED (M4 core)
  }

  updateDashboard("'Set up complete'", "NA", "NA", "NA", "NA", "NA", "NA");
}

// ********************************************************************************
// Loop
// ********************************************************************************
void loop() {

  // Check for RPC.print statements from the M4
  String buffer = "";
  while (RPC.available()) {
    buffer += (char)RPC.read();  // Fill the buffer with characters
  }
  if (buffer.length() > 0) {
    Serial.print(buffer);
  }

  // ------------------------------------------------------------------------------
  // Check for Serial messages from Serial Monitor or Dashboard
  // ------------------------------------------------------------------------------
  if (Serial.available()) {

    // Get value from serial
    String funcString = Serial.readString();

    // Parse the message
    int index = funcString.indexOf("_");
    String keyword = funcString.substring(0, index);
    String operation = funcString.substring(index + 1, funcString.length());
    int room_num;
    String appleParams;

    // Determine if the operation represents a room number or the apple parameters
    // Use for loop if adding more operation types
    if (operation.length() >= 7) {
      appleParams = operation;
    }
    else {
      room_num = operation.toInt();
    }

    if (keyword == "honeycrisp") {
      VRS.rooms[room_num - 1].setAppleType(Honeycrisp, "Honeycrisp");

      updateDashboard("'Room " + String(room_num) + " honeycrisp'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "empire") {
      VRS.rooms[room_num - 1].setAppleType(Empire, "Empire");

      updateDashboard("'Room " + String(room_num) + " fuji'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "mcintosh") {
      VRS.rooms[room_num - 1].setAppleType(Mcintosh, "Mcintosh");

      updateDashboard("'Room " + String(room_num) + " mcintosh'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "macoun") {
      VRS.rooms[room_num - 1].setAppleType(Macoun, "Macoun");

      updateDashboard("'Room " + String(room_num) + " macoun'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "cortland") {
      VRS.rooms[room_num - 1].setAppleType(Cortland, "Cortland");

      updateDashboard("'Room " + String(room_num) + " cortland'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "gala") {
      VRS.rooms[room_num - 1].setAppleType(Gala, "Gala");

      updateDashboard("'Room " + String(room_num) + " gala'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "systemOn") {
      VRS.setSystemState(true);

      updateDashboard("'System turned on'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "systemOff") {
      VRS.setSystemState(false);

      updateDashboard("'System turned off'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "activateRoom") {
      VRS.activateRoom(room_num - 1);

      updateDashboard("'Room " + String(room_num) + " activated'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "deactivateRoom") {
      VRS.deactivateRoom(room_num - 1);

      updateDashboard("'Room " + String(room_num) + " deactivated'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "oxygenSolOff") {
      VRS.rooms[room_num - 1].setO2fanState(0);

      updateDashboard("'Room " + String(room_num) + " oxygen solenoid closed'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "nitrogenSolOff") {
      VRS.rooms[room_num - 1].setN2solState(0);

      updateDashboard("'Room " + String(room_num) + " nitrogen solenoid closed'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "bothSolsOff") {
      VRS.rooms[room_num - 1].setN2solState(0);
      VRS.rooms[room_num - 1].setO2fanState(0);

      updateDashboard("'Room " + String(room_num) + " fan and sol off'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "oxygenSolOn") {
      VRS.rooms[room_num - 1].setO2fanState(1);
      VRS.rooms[room_num - 1].setN2solState(0); // In this set up, if the O2 is on than the N2 is not on

      updateDashboard("'Room " + String(room_num) + " oxygen solenoid opened'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "nitrogenSolOn") {
      VRS.rooms[room_num - 1].setN2solState(1);
      VRS.rooms[room_num - 1].setO2fanState(0); // In this set up, if the N2 is on than the O2 is not on

      updateDashboard("'Room " + String(room_num) + " nitrogen solenoid opened'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "bothSolsOn") {
      VRS.rooms[room_num - 1].setN2solState(1);
      VRS.rooms[room_num - 1].setO2fanState(1);

      updateDashboard("'Room " + String(room_num) + " fan and sol on'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyHoneycrisp") {
      Honeycrisp.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Honeycrisp") {
          VRS.rooms[i].setAppleType(Honeycrisp, "Honeycrisp");
        }
      }

      updateDashboard("'Honeycrisp modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyEmpire") {
      Empire.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Empire") {
          VRS.rooms[i].setAppleType(Empire, "Empire");
        }
      }

      updateDashboard("'Empire modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyMcintosh") {
      Mcintosh.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Mcintosh") {
          VRS.rooms[i].setAppleType(Mcintosh, "Mcintosh");
        }
      }

      updateDashboard("'Mcintosh modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyMacoun") {
      Macoun.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Macoun") {
          VRS.rooms[i].setAppleType(Macoun, "Macoun");
        }
      }

      updateDashboard("'Macoun modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyCortland") {
      Cortland.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Cortland") {
          VRS.rooms[i].setAppleType(Cortland, "Cortland");
        }
      }

      updateDashboard("'Cortland modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyGala") {
      Gala.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Gala") {
          VRS.rooms[i].setAppleType(Gala, "Gala");
        }
      }

      updateDashboard("'Gala modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyCustom1") {
      Custom1.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Custom1") {
          VRS.rooms[i].setAppleType(Custom1, "Custom1");
        }
      }

      updateDashboard("'Custom Apple 1 modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyCustom2") {
      Custom2.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Custom2") {
          VRS.rooms[i].setAppleType(Custom2, "Custom2");
        }
      }

      updateDashboard("'Custom Apple 2 modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyCustom3") {
      Custom3.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Custom3") {
          VRS.rooms[i].setAppleType(Custom3, "Custom3");
        }
      }

      updateDashboard("'Custom Apple 3 modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyCustom4") {
      Custom4.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Custom4") {
          VRS.rooms[i].setAppleType(Custom4, "Custom4");
        }
      }

      updateDashboard("'Custom Apple 4 modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyCustom5") {
      Custom5.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Custom5") {
          VRS.rooms[i].setAppleType(Custom5, "Custom5");
        }
      }

      updateDashboard("'Custom Apple 5 modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else if (keyword == "modifyCustom6") {
      Custom6.modifyAppleParams(appleParams);

      for (int i = 0; i < NUM_ROOMS; i++) {
        if (VRS.rooms[i].getCurrentApple() == "Custom6") {
          VRS.rooms[i].setAppleType(Custom6, "Custom6");
        }
      }

      updateDashboard("'Custom Apple 6 modified to " + String(appleParams) + "'", "NA", "NA", "NA", "NA", "NA", "NA");
    }
    else {
      // Invalid input, something went wrong
      updateDashboard("Error! Invalid input command from Node-Red.", "NA", "NA", "NA", "NA", "NA", "NA");
    }
  }
  else {
    // No serial available
  }
}






