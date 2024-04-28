/*Team 16--Apple Storage Controller MAIN Code for sprint 5 rev 2 -- 
* Sensing room able to pump in air from 2 rooms, capabaility of any number of rooms 
* along with sense room flushing capabilities to make sure sensor is reading values properly.
* Also able to read both O2 and CO2, from ExplorIR CO2 sensor and DFRobot Oxygen sensor.
* Setup to work with Arduino Giga R1 Wifi -- make sure Giga is in mode to accept upload by 
* double tapping reset button (on board LED will blink green).
*/

#include "MillisTimerLib.h"
#include "System.h"
#include "DFRobot_OxygenSensor.h"
#include "CO2Sensor.h"
#include "O2Sensor.h"


//****************************************************************************
// SYSTEM
//****************************************************************************
// Create the system object
System VRS;

// ******************************************************************************
// ROOMS
// ******************************************************************************
// Create room objects
Room4 room1;
Room4 room2;

// *****************************************************************************
// APPLES
// *****************************************************************************
// Create an apple object for default
Apple Honeycrisp;

//****************************************************************************
//GLOBAL VARIABLES
//****************************************************************************
// Set previousMillis to 0
unsigned long previousMillis = 0;       // Compare to value for currentMillis
unsigned long currentMillis;            // Holds current value from millis()

int DELAY_1S = 1000;              //1 second delay
int DELAY_10S = 10000;            //10 second delay

static unsigned int state;              //state variable

static int MAXROOMS = 1;                // Constant value of number of rooms

int x = 0;                              // x is the RUT (Room under Test)
int calibrateCount = 0;                 // The number of times the sensors have been calibrated since the last calibration
boolean calibration_necessary;          // Boolean which determines if calibration is necessary

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
O2Sensor EZO2;

//****************************************************************************
// Potetntiometer values for offline debug and validation
//****************************************************************************
#define potpin1 A8
#define potpin2 A9
float potValue1;
float potValue2;
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
  delay(DELAY_10S);

  boolean sensorsNotLevel = true;
  
  while(sensorsNotLevel) {
    // Get oxygen value from sensor
    //currentO2 = oxygen.getOxygenData(collectNumber);
    currentO2 = 2.5;                                                              //placeholder
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

/*
 * Function which is responsible for initializing and setting up the room objects and system
 * This is where you can modify parameters such as pin numbers
 * Takes in nothing 
 * Returns nothing
 */
void setUpSystem() {
  // Add the rooms to the system
  VRS.addRoom(room1);
  VRS.addRoom(room2);

  // Set up system variables
  VRS.setSystemName("VRS Apple Storage Control");         // Name of system
  VRS.setNumRooms(2);                                     // Number of rooms
  VRS.setPumpPin(52);                                     // Pin for pump

  // Set up room1 variables
  VRS.rooms[0].setRoomNum(1);                             // Number of room
  VRS.rooms[0].setAppleType(Honeycrisp);                  // Type of apple in room
  VRS.rooms[0].setOxygenSolenoidPin(42);                  // (pin 42) Pin for oxygen solenoid/fan 
  VRS.rooms[0].setNitrogenSolenoidPin(43);                // (pin 43) Pin for nitrogen solenoid
  VRS.rooms[0].setSensingSolenoidPin(30);                 // (pin 30) Pin for the solenoid for measurement in room1
  VRS.rooms[0].setO2solState(0);                          // Initial state of the oxygen solenoid/fan
  VRS.rooms[0].setN2solState(0);                          // Initial state of the nitrogen solenoid
  VRS.rooms[0].setSenseSolState(0);                       // Initial state of the sensing solenoid

  // Set up room2 variables
  VRS.rooms[1].setRoomNum(2);                             // Number of room
  VRS.rooms[1].setAppleType(Honeycrisp);                  // Type of apple in room
  VRS.rooms[1].setOxygenSolenoidPin(44);                  // (pin 44) Pin for oxygen solenoid/fan
  VRS.rooms[1].setNitrogenSolenoidPin(45);                // (pin 45) Pin for nitrogen solenoid
  VRS.rooms[1].setSensingSolenoidPin(32);                 // (pin 32) Pin for the solenoid for measurement in room2
  VRS.rooms[1].setO2solState(0);                          // Initial state of the oxygen solenoid/fan
  VRS.rooms[1].setN2solState(0);                          // Initial state of the nitrogen solenoid
  VRS.rooms[1].setSenseSolState(0);                       // Initial state of the sensing solenoid
}

/* Function which is used to test the functionality of the program
 * Takes in nothing
 * Returns nothing
 */
void testSetUp() {
  // Fill in test code here
}

//***************************************************************************
//Setup
//***************************************************************************
void setup() {
  setUpSystem();

  testSetUp();
  
  // pin classifications
  pinMode(VRS.rooms[0].getOxygenSolenoidPin()  , OUTPUT);
  pinMode(VRS.rooms[1].getOxygenSolenoidPin()  , OUTPUT);
  pinMode(VRS.rooms[0].getNitrogenSolenoidPin(), OUTPUT);
  pinMode(VRS.rooms[1].getNitrogenSolenoidPin(), OUTPUT);
  pinMode(VRS.rooms[0].getSensingSolenoidPin() , OUTPUT);
  pinMode(VRS.rooms[1].getSensingSolenoidPin() , OUTPUT);
  //pinMode(potpin1,INPUT);
  //pinMode(potpin2,INPUT);
  pinMode(VRS.getPumpPin(), OUTPUT);

  // Initialize serial connection @ 9600 Baud Rate for serial monitor
  Serial.begin(9600);
  // Wait for serial port to open
  while(!Serial);
  Serial.println("...(SETUP): Serial Communication started");
  //initialize Serial1 communication @ 9600 Baud Rate for CO2 sensor (RX/TX 0)
  Serial1.begin(9600);
  // Wait for serial1 port to open
  while(!Serial1);
  Serial.println("...(SETUP): Serial1 Communication started");
  //initialize Serial2 communication @ 9600 Baud Rate for O2 sensor (RX/TX 1)
  Serial2.begin(9600);
  // Wait for serial1 port to open
  while(!Serial2);
  Serial.println("...(SETUP): Serial2 Communication started");
  //EXPLORIR CO2 connection
  explorCO2.initialize();
  Serial.println("...(SETUP): CO2 sensor setup");
  delay(2000);

  EZO2.initialize();
  delay(100);
  //Oxygen I2C connection
  // while(!oxygen.begin(Oxygen_IIC_Address)){

  //   // Send update to dashboard
  //   dataArray[6] = "I2C device number error!";      // Create update about I2C connection failure
  //   dataString = "";                                // Reset dataString
  //   dataString = arrayToString(dataArray);          // Convert data to string
  //   Serial.println(dataString);                     // Send update
    
  //   delay(1000);
  // }

  // Send update to dashboard
  dataArray[6] = "...(SETUP): I2C connect success";           // Create update about successful I2C connection
  dataString = "";                                // Reset dataString
  dataString = arrayToString(dataArray);          // Convert data to string
  Serial.println(dataString);                     // Send update

  // Set calibrate to active
  calibration_necessary = true;

  // Begin state machine
  Serial.println("...(SETUP): going to state 1");
  state = 1;
}

//****************************************************************************
//Loop 
//****************************************************************************
void loop(){
  // Set current time in millis
  currentMillis = millis();

  // Enter state machine
  switch(state){
    
    // Reset state ********************************************************************************************
    // Happens on start up and activates rooms
    case 1:

      // Set all rooms to be active
      Serial.println("...(INFO): activating rooms");
      VRS.rooms[0].activate();
      VRS.rooms[1].activate();
      
      delay(DELAY_1S);

      // Proceed to state 2
      Serial.println("...(INFO): Going to state 2");
      state = 2;
    break;
    
    // Administrative State **********************************************************************************
    // This state performs system checks to make sure that the system is OK
    case 2:

      // Set currentMillis
      currentMillis = millis();
      if ((currentMillis - previousMillis) >= DELAY_1S){
        
        // Set previousMillis for comparison later
        previousMillis = currentMillis;
        
        // Check to see if calibration is necessary
        if(calibration_necessary){
   
          // Proceed to state 3
          Serial.println("...(INFO): Going to state 3");
          state = 3;
        }
        else {
          
          // If all checks have been passed, pass message to dashboard
          dataArray[6] = "System OK";                     // Create update about system
          dataString = "";                                // Reset dataString
          dataString = arrayToString(dataArray);          // Convert data to string
          Serial.println(dataString);                     // Send update
  
          // Proceed to state 4
          Serial.println("...(INFO): Going to state 4");
          state = 4;
        }
      }
    break;

    // Calibration State ********************************************************************************
    case 3:

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= DELAY_10S) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;
        
        // All code relavent to calibrating the sensors will be located here
        
        // For now we will just tell the dashboard that we are calibrating and delay for 10 seconds
        dataArray[6] = "Calibrating...";                // Create update about room under test
        dataString = "";                                // Reset dataString
        dataString = arrayToString(dataArray);          // Convert data to string
        Serial.println(dataString);                     // Send update 

        delay(DELAY_10S);

        // Calibration is no longer necessary
        calibration_necessary = false;
        
        // Return to state 2
        Serial.println("...(INFO): Going to state 2");
        state = 2;
      }
    break;

    // Measurement state *********************************************************************************
    // This state is responsible for capturing sensor data and making changes to rooms
    case 4:

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= DELAY_1S) {

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
          Serial.println("...(INFO): Updating Dashboard");
          dataArray[0] = "room " + String(x + 1);         // Create update about room under test
          dataString = "";                                // Reset dataString
          dataString = arrayToString(dataArray);          // Convert data to string
          Serial.println(dataString);                     // Send update 

          // This turns on the pump to pull in air into the measuring environment
          Serial.println("...(INFO): Turning pump on");
          VRS.pumpOn(true, VRS.rooms[x].getRoomNum() - 1);

          // Call function to make sure sensors are level before proceeding
          Serial.println("...(INFO): Checking sensors leveled out");
          checkSensorLevel();

          // When sensors are leveled, we can turn off the pump
          VRS.pumpOn(false, VRS.rooms[x].getRoomNum() - 1);

          // Sensors should be level and we can now take measurements
          //oxygenValue = oxygen.getOxygenData(collectNumber);
          //oxygenValue = 2.5;
          potValue1 = analogRead(potpin1);
          potValue2 = analogRead(potpin2);
          oxygenValue = EZO2.getPercent() * potValue1 / 5000;                             //potvalue debug O2 value
          //CO2Percent = explorCO2.getPercent();                           //get percentage of CO2
          CO2Percent = explorCO2.getPercent() * potValue2 / 30 ;           //potvalue debug CO2 value
          int roomTemp = 36;                                              // Placeholder for temperature
          Serial.println(potValue1);
          Serial.println(potValue2);
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

          // Increment the room variable
          if (x == MAXROOMS) {
            x = 0;
          }
          else {
            x++;
          }
          
          // Once all measurements have been completed, we can proceed to next room and go back to state 2
          state = 2;
        }
        
        // If not active, then proceed to next room and go back to state 2
        else {

          // Change room
          if (x == MAXROOMS) {
            x = 0;
          }
          else {
            x++;
          }

          // Go to state 2
          state = 2;
        }
      }
      break;
    } 
  }
