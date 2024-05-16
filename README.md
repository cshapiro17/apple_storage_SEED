# apple_storage_SEED
All code relevant to monitoring, measuring, and recording apple storge data for controlled atmosphere rooms

-------------------------------------------------------------------------------------------------------------

The Arduino Giga R1 communicates serially with a Raspberry Pi 5 which runs the user interface and manages the
data storage for the system. All Serial print statements in the Communication.ino loop function represent 
messages out to the RPi.

- Automation.ino runs the state machine on the M4 core of the Giga
- Communication.ino handles all communication between the Arduino and Raspberry Pi on the M7 core of the Giga

--------------------------------------------------------------------------------------------------------------

Communication from Arduino to Raspberry Pi

- String of comma-separated data
- Room, Room Under Test, Oxygen Value, Carbon Value, Oxygen Fan On/Off, Nitrogen Solenoid On/Off, System Status
- Example: "room1,1,3.24,0.81,1,0,'Room 1 activated'"

---------------------------------------------------------------------------------------------------------------

Communication from Raspberry Pi to Arduino

- String consisting of an underscore-separated function and room number
- Function_Room
- Example: "activateRoom_1"
- The example will activate room number 1
