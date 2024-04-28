/*Team 16--Apple Storage Controller MAIN Code for sprint 4 -- Sensing room able to pump in air from 2 rooms, 
*along with sense room flushing capabilities to make sure sensor is reading values properly.
* Also able to read both O2 and CO2, making changes to both
*/

#include "MillisTimerLib.h"
#include "Room3.h"
#include "DFRobot_OxygenSensor.h"

//****************************************************************************
//INPUTS
//****************************************************************************

int mq135Pin = A2;            //MQ135 CO2 sensor analog pin

//****************************************************************************
//OUTPUTS
//****************************************************************************

// Solenoids
int oxygenSolenoidPin[6] = {5,6,3,4,5,6};          //pins for solenoids that lets oxygen into room
int nitrogenSolenoidPin[6] = {8,9,9,10,11,12};       //pins for solenoids that lets nitrogen into room
int pumpSolenoidPin[6] = {11,12,15,16,17,18};  //pins for allowing air to be pumped in

// Pumps
int pumpPin = 1;             //pump 1 output pin (Room 1 Pump)
int pumpPin3= 5;             //pump 3 output pin (Fresh Air Pump)

//****************************************************************************
//GLOBAL VARIABLES
//****************************************************************************

// Set previousMillis to 0
unsigned long previousMillis = 0;       // Compare to value for currentMillis
unsigned long currentMillis;            // Holds current value from millis()

int delay_interval = 1000;                //interval for state 2 and 3 delay

static unsigned int state;              //state variable

int roomNum[6] = {1,2,3,4,5,6};         //room numbers
static int MAXROOMS = 1;                // Constant value of number of rooms

// Initialize solenoid states to be off initially
int oxygenSolenoidState = 0;
int nitrogenSolenoidState = 0;

boolean pumpOnOff;

// Create array of room objects
// Each room object will have an associated number, high threshold, low threshold, oxygen solenoid, nitrogen solenoid, fresh air pump, and ... 
Room3 room[2] = {
    Room3(roomNum[0], 4, 2, oxygenSolenoidPin[0], nitrogenSolenoidPin[0], pumpSolenoidPin[0], pumpPin, oxygenSolenoidState, nitrogenSolenoidState),
    Room3(roomNum[1], 4, 2, oxygenSolenoidPin[1], nitrogenSolenoidPin[1], pumpSolenoidPin[1], pumpPin, oxygenSolenoidState, nitrogenSolenoidState),
};

int x = 0;                        //x is the RUT (Room under Test)
int calibrateCount = 0;
boolean calibration_necessary;

//****************************************************************************
// CO2 VARIABLES
// To be replaced by new CO2 sensor
//****************************************************************************

#define atmCO2 = 418.51;
float Rl = 22000;
float R0 = 228067;
float minvalue = 80;
float CO2Value;

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
//
// Data is stored in an array of strings and printed to serial monitor
// Default values are set as empty strings
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
 */
void checkSensorLevel() {

  // This delay can be used to run the pump for a certain amount of time before checking levels
  delay(10000);

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
 *  Takes in the dataArray 
 *  Returns the dataString
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
  
  // pin classifications
  pinMode(oxygenSolenoidPin[0],OUTPUT);
  pinMode(oxygenSolenoidPin[1],OUTPUT);
  pinMode(nitrogenSolenoidPin[0],OUTPUT);
  pinMode(nitrogenSolenoidPin[1],OUTPUT);
  pinMode(pumpSolenoidPin[0],OUTPUT);
  pinMode(pumpSolenoidPin[1],OUTPUT);
  pinMode(pumpPin,OUTPUT);
  //pinMode(pumpPin2,OUTPUT);
  //pinMode(potent1,INPUT);

  //Serial Output
  Serial.begin(9600);

  // Wait for serial port to open
  while(!Serial);

  //Oxygen I2C connection
  while(!oxygen.begin(Oxygen_IIC_Address)){

    // Send update to dashboard
    dataArray[6] = "I2C device number error!";      // Create update about I2C connection failure
    dataString = "";                                // Reset dataString
    dataString = arrayToString(dataArray);          // Convert data to string
    Serial.println(dataString);                     // Send update
    
    delay(1000);
  }

  // Send update to dashboard
  dataArray[6] = "I2C connect success";           // Create update about successful I2C connection
  dataString = "";                                // Reset dataString
  dataString = arrayToString(dataArray);          // Convert data to string
  Serial.println(dataString);                     // Send update


  // Set calibrate to active
  calibration_necessary = true;

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
      room[0].activate();
      room[1].activate();
      
      delay(1000);

      // Proceed to state 2
      state = 2;
    break;
    
    // Administrative State **********************************************************************************
    // This state performs system checks to make sure that the system is OK
    case 2:

      // Set currentMillis
      currentMillis = millis();
      if ((currentMillis - previousMillis) >= delay_interval){
        
        // Set previousMillis for comparison later
        previousMillis = currentMillis;
        
        // Check to see if calibration is necessary
        if(calibration_necessary){
   
          // Proceed to state 3
          state = 3;
        }
        else {
          
          // If all checks have been passed, pass message to dashboard
          dataArray[6] = "System OK";                     // Create update about system
          dataString = "";                                // Reset dataString
          dataString = arrayToString(dataArray);          // Convert data to string
          Serial.println(dataString);                     // Send update
  
          // Proceed to state 4
          state = 4;
        }
      }
    break;

    // Calibration State ********************************************************************************
    case 3:

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= delay_interval) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;
        
        // All code relavent to calibrating the sensors will be located here
        
        // For now we will just tell the dashboard that we are calibrating and delay for 10 seconds
        dataArray[6] = "Calibrating...";                // Create update about room under test
        dataString = "";                                // Reset dataString
        dataString = arrayToString(dataArray);          // Convert data to string
        Serial.println(dataString);                     // Send update 

        delay(10000);

        // Calibration is no longer necessary
        calibration_necessary = false;
        
        // Return to state 2
        state = 2;
      }
    break;

    // Measurement state *********************************************************************************
    // This state is responsible for capturing sensor data and making changes to rooms
    case 4:

      // Set currentMillis
      currentMillis = millis();
      
      if ((currentMillis - previousMillis) >= delay_interval) {

        // Set previousMillis for comparison later
        previousMillis = currentMillis;

        // Check to see if the room in question is active
        if (room[x].isActive()) {

          // Clear data in preperation for next room
          dataArray[1] = "";
          dataArray[2] = "";
          dataArray[3] = "";
          dataArray[4] = "";
          dataArray[5] = "";
          
          // If active, update dashboard
          dataArray[0] = "room" + String(x + 1);          // Create update about room under test
          dataString = "";                                // Reset dataString
          dataString = arrayToString(dataArray);          // Convert data to string
          Serial.println(dataString);                     // Send update 

          // This turns on the pump to pull in air into the measuring environment
          room[x].pumpOn(true);

          // Call function to make sure sensors are level before proceeding
          checkSensorLevel();

          // When sensors are leveled, we can turn off the pump
          room[x].pumpOn(false);

          // Sensors should be level and we can now take measurements
          oxygenValue = oxygen.getOxygenData(collectNumber);
          CO2Value = 1.5;                                                         // Placeholder for CO2 measurement code
          int roomTemp = 36;                                                       // Placeholder for temperature
     
          // Evaluate the room, this function will set the states of the solenoids to their necessary positions (open or closed)
          room[x].evaluateRoom(oxygenValue, CO2Value);

          // Get the states of each solenoid
          int oxygenSolenoidState = room[x].getO2solState();
          int nitrogenSolenoidState = room[x].getN2solState();

          // Report changes to dashboard
          dataArray[1] = String(oxygenValue);
          dataArray[2] = String(CO2Value);
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
