#ifndef Config_h
#define Config_h

/**
 * This file contains the configuration for the system
 * This include pin declarations, device names, serial connections, etc.
 * 
 * Use this file to set up the system
 */

#include "System.h"

/**
 * Define the name of the system
 * This can be a string of your choice
 * It has been initialized as "VRS" for "Vermont Refridgerated Storage"
 * 
 * This must always be defined
 */
#define SYSTEM_NAME "VRS"

/**
 * Define the pin connection for the pump to the Arduino Gigi R1 Wifi
 */
#define SYSTEM_PUMP_PIN 52

/**
 * Define the number of rooms which are in the system
 * This should stay constant
 */
#define NUM_ROOMS 6

/**
 * To initialize a room, uncomment the line "ROOM_X_NAME" for the room which you want to activate
 */
#define ROOM_1_NAME room1
#define ROOM_2_NAME room2
//#define ROOM_3_NAME room3
//#define ROOM_4_NAME room4
//#define ROOM_5_NAME room5
//#define ROOM_6_NAME room6

/**
 * Define the pin connections for each of the solenoids to the Arduino Giga R1 Wifi
 * ROOM_1_NAME must be defined for this code block to apply
 * 
 * This is for the solenoids to room 1 only
 */
#ifdef ROOM_1_NAME
  #define R1_O2_SOLENOID_PIN 42
  #define R1_N2_SOLENOID_PIN 43
  #define R1_SENSING_SOLENOID_PIN 30
#endif

/**
 * Define the pin connections for each of the solenoids to the Arduino Giga R1 Wifi
 * ROOM_2_NAME must be defined for this code block to apply
 * 
 * This is for the solenoids to room 2 only
 */
#ifdef ROOM_2_NAME
  #define R2_O2_SOLENOID_PIN 44
  #define R2_N2_SOLENOID_PIN 45
  #define R2_SENSING_SOLENOID_PIN 34
#endif

/**
 * Set the pin connections for each of the solenoids to the Arduino Giga R1 Wifi
 * ROOM_3_NAME must be defined for this code block to apply
 * 
 * This is for the solenoids to room 3 only
 */
#ifdef ROOM_3_NAME
  #define R3_O2_SOLENOID_PIN
  #define R3_N2_SOLENOID_PIN
  #define R3_SENSING_SOLENOID_PIN
#endif

/**
 * Set the pin connections for each of the solenoids to the Arduino Giga R1 Wifi
 * ROOM_4_NAME must be defined for this code block to apply
 * 
 * This is for the solenoids to room 4 only
 */
#ifdef ROOM_4_NAME
  #define R4_O2_SOLENOID_PIN
  #define R4_N2_SOLENOID_PIN
  #define R4_SENSING_SOLENOID_PIN
#endif

/**
 * Set the pin connections for each of the solenoids to the Arduino Giga R1 Wifi
 * ROOM_5_NAME must be defined for this code block to apply
 * 
 * This is for the solenoids to room 5 only
 */
#ifdef ROOM_5_NAME
  #define R5_O2_SOLENOID_PIN
  #define R5_N2_SOLENOID_PIN
  #define R5_SENSING_SOLENOID_PIN
#endif

/**
 * Set the pin connections for each of the solenoids to the Arduino Giga R1 Wifi
 * ROOM_6_NAME must be defined for this code block to apply
 * 
 * This is for the solenoids to room 6 only
 */
#ifdef ROOM_6_NAME
  #define R6_O2_SOLENOID_PIN
  #define R6_N2_SOLENOID_PIN
  #define R6_SENSING_SOLENOID_PIN
#endif

/**
 * Sets the number of cycles to go through before recalibrating the sensors
 */
#define CYCLES_PER_CALIBRATION 5

/**
 * Delay definitions
 */
#define TIMER_DELAY 1000
#define RESET_DELAY 1000
#define PUMP_DELAY 10000
#define CO2_CONNECT_DELAY 2000

#define SIMULATE_CALIBRATION 10000


#endif
