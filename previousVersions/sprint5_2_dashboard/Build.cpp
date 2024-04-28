/**
 * Build file which constructs the system and room given the information provided in "Config.h"
 * This code is responsible for putting the system in a working state
 * 
 * Because this code relies soley on "Config.h", it should not need to be modified
 */


#include "System.h"
#include "Config.h"
#include "Build.h"

using namespace std;


void Build::setUpSystem() {

  //****************************************************************************
  // SYSTEM
  //****************************************************************************
  
  #ifdef SYSTEM_NAME

    System VRS;                               // Create the system object

    VRS.setSystemName(SYSTEM_NAME);           // Name of system
    VRS.setNumRooms(NUM_ROOMS);               // Number of rooms in the system
    VRS.setPumpPin(SYSTEM_PUMP_PIN);          // Pin for pump
    pinMode(VRS.getPumpPin(), OUTPUT);        // Initialize the pin for the pump

    Apple TEST_APPLE;                         // Test apple for debugging
    VRS.addApple(TEST_APPLE);

  #endif
  
  // ******************************************************************************
  // ROOMS
  // ******************************************************************************
  
  // Create room objects
  #ifdef ROOM_1_NAME
  
    Room4 ROOM_1_NAME;                                                      // Create the room 1 object

    VRS.addRoom(ROOM_1_NAME);                                               // Name of room 1

    // Set up room1 variables
    VRS.rooms[0].setRoomNum(1);                                             // Number of room     (STILL IN DEVELOPMENT)
    VRS.rooms[0].setAppleType(TEST_APPLE);                                  // Type of apple in room    (STILL IN DEVELOPMENT)
    VRS.rooms[0].setOxygenSolenoidPin(R1_O2_SOLENOID_PIN);                  // Pin for oxygen solenoid/fan 
    VRS.rooms[0].setNitrogenSolenoidPin(R1_N2_SOLENOID_PIN);                // Pin for nitrogen solenoid
    VRS.rooms[0].setSensingSolenoidPin(R1_SENSING_SOLENOID_PIN);            // Pin for the solenoid for measurement in room1
    VRS.rooms[0].setO2solState(0);                                          // Initial state of the oxygen solenoid/fan
    VRS.rooms[0].setN2solState(0);                                          // Initial state of the nitrogen solenoid
    VRS.rooms[0].setSenseSolState(0);                                       // Initial state of the sensing solenoid


    pinMode(VRS.rooms[0].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 1
    pinMode(VRS.rooms[0].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 1
    pinMode(VRS.rooms[0].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 1

  #endif
  
  #ifdef ROOM_2_NAME
  
    Room4 ROOM_2_NAME;                                                       // Create the room 2 object

    VRS.addRoom(ROOM_2_NAME);                                               // Name of room 2

    // Set up room2 variables
    VRS.rooms[1].setRoomNum(1);                                             // Number of room     (STILL IN DEVELOPMENT)
    VRS.rooms[1].setAppleType(TEST_APPLE);                                  // Type of apple in room    (STILL IN DEVELOPMENT)
    VRS.rooms[1].setOxygenSolenoidPin(R2_O2_SOLENOID_PIN);                  // Pin for oxygen solenoid/fan 
    VRS.rooms[1].setNitrogenSolenoidPin(R2_N2_SOLENOID_PIN);                // Pin for nitrogen solenoid
    VRS.rooms[1].setSensingSolenoidPin(R2_SENSING_SOLENOID_PIN);            // Pin for the solenoid for measurement in room1
    VRS.rooms[1].setO2solState(0);                                          // Initial state of the oxygen solenoid/fan
    VRS.rooms[1].setN2solState(0);                                          // Initial state of the nitrogen solenoid
    VRS.rooms[1].setSenseSolState(0);                                       // Initial state of the sensing solenoid

    // pin classifications
    pinMode(VRS.rooms[0].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 2
    pinMode(VRS.rooms[0].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 2
    pinMode(VRS.rooms[0].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 2

  #endif
  
  #ifdef ROOM_3_NAME
  
    Room4 ROOM_3_NAME;                                                       // Create the room 3 object

    VRS.addRoom(ROOM_3_NAME);                                               // Name of room 3

    // Set up room3 variables
    VRS.rooms[2].setRoomNum(1);                                             // Number of room     (STILL IN DEVELOPMENT)
    VRS.rooms[2].setAppleType(TEST_APPLE);                                  // Type of apple in room    (STILL IN DEVELOPMENT)
    VRS.rooms[2].setOxygenSolenoidPin(R3_O2_SOLENOID_PIN);                  // Pin for oxygen solenoid/fan 
    VRS.rooms[2].setNitrogenSolenoidPin(R3_N2_SOLENOID_PIN);                // Pin for nitrogen solenoid
    VRS.rooms[2].setSensingSolenoidPin(R3_SENSING_SOLENOID_PIN);            // Pin for the solenoid for measurement in room1
    VRS.rooms[2].setO2solState(0);                                          // Initial state of the oxygen solenoid/fan
    VRS.rooms[2].setN2solState(0);                                          // Initial state of the nitrogen solenoid
    VRS.rooms[2].setSenseSolState(0);                                       // Initial state of the sensing solenoid

    pinMode(VRS.rooms[0].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 3
    pinMode(VRS.rooms[0].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 3
    pinMode(VRS.rooms[0].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 3
    
  #endif
  
  #ifdef ROOM_4_NAME
  
    Room4 ROOM_4_NAME;                                                       // Create the room 4 object

    VRS.addRoom(ROOM_4_NAME);                                               // Name of room 4

    // Set up room4 variables
    VRS.rooms[3].setRoomNum(1);                                             // Number of room     (STILL IN DEVELOPMENT)
    VRS.rooms[3].setAppleType(TEST_APPLE);                                  // Type of apple in room    (STILL IN DEVELOPMENT)
    VRS.rooms[3].setOxygenSolenoidPin(R4_O2_SOLENOID_PIN);                  // Pin for oxygen solenoid/fan 
    VRS.rooms[3].setNitrogenSolenoidPin(R4_N2_SOLENOID_PIN);                // Pin for nitrogen solenoid
    VRS.rooms[3].setSensingSolenoidPin(R4_SENSING_SOLENOID_PIN);            // Pin for the solenoid for measurement in room1
    VRS.rooms[3].setO2solState(0);                                          // Initial state of the oxygen solenoid/fan
    VRS.rooms[3].setN2solState(0);                                          // Initial state of the nitrogen solenoid
    VRS.rooms[3].setSenseSolState(0);                                       // Initial state of the sensing solenoid

    pinMode(VRS.rooms[0].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 4
    pinMode(VRS.rooms[0].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 4
    pinMode(VRS.rooms[0].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 4
    
  #endif
  
  #ifdef ROOM_5_NAME
  
    Room4 ROOM_5_NAME;                                                       // Create the room 5 object

    VRS.addRoom(ROOM_5_NAME);                                               // Name of room 5

    // Set up room5 variables
    VRS.rooms[4].setRoomNum(1);                                             // Number of room     (STILL IN DEVELOPMENT)
    VRS.rooms[4].setAppleType(TEST_APPLE);                                  // Type of apple in room    (STILL IN DEVELOPMENT)
    VRS.rooms[4].setOxygenSolenoidPin(R5_O2_SOLENOID_PIN);                  // Pin for oxygen solenoid/fan 
    VRS.rooms[4].setNitrogenSolenoidPin(R5_N2_SOLENOID_PIN);                // Pin for nitrogen solenoid
    VRS.rooms[4].setSensingSolenoidPin(R5_SENSING_SOLENOID_PIN);            // Pin for the solenoid for measurement in room1
    VRS.rooms[4].setO2solState(0);                                          // Initial state of the oxygen solenoid/fan
    VRS.rooms[4].setN2solState(0);                                          // Initial state of the nitrogen solenoid
    VRS.rooms[4].setSenseSolState(0);                                       // Initial state of the sensing solenoid

    pinMode(VRS.rooms[0].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 5
    pinMode(VRS.rooms[0].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 5
    pinMode(VRS.rooms[0].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 5
  #endif
  
  #ifdef ROOM_6_NAME
  
    Room4 ROOM_6_NAME;                                                       // Create the room 6 object
  
    VRS.addRoom(ROOM_6_NAME);                                               // Name of room 6
  
    // Set up room6 variables
    VRS.rooms[5].setRoomNum(1);                                             // Number of room     (STILL IN DEVELOPMENT)
    VRS.rooms[5].setAppleType(TEST_APPLE);                                  // Type of apple in room    (STILL IN DEVELOPMENT)
    VRS.rooms[5].setOxygenSolenoidPin(R6_O2_SOLENOID_PIN);                  // Pin for oxygen solenoid/fan 
    VRS.rooms[5].setNitrogenSolenoidPin(R6_N2_SOLENOID_PIN);                // Pin for nitrogen solenoid
    VRS.rooms[5].setSensingSolenoidPin(R6_SENSING_SOLENOID_PIN);            // Pin for the solenoid for measurement in room1
    VRS.rooms[5].setO2solState(0);                                          // Initial state of the oxygen solenoid/fan
    VRS.rooms[5].setN2solState(0);                                          // Initial state of the nitrogen solenoid
    VRS.rooms[5].setSenseSolState(0);                                       // Initial state of the sensing solenoid
    
    pinMode(VRS.rooms[0].getOxygenSolenoidPin(), OUTPUT);                   // Initialize the pin for the oxygen solenoid in room 6
    pinMode(VRS.rooms[0].getNitrogenSolenoidPin(), OUTPUT);                 // Initialize the pin for the nitrogen solenoid in room 6
    pinMode(VRS.rooms[0].getSensingSolenoidPin(), OUTPUT);                  // Initialize the pin for the sensing solenoid in room 6
  #endif

}
