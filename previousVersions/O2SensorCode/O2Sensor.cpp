/* UART_code2 is used to integrate the O2Sensor library to takes O2 readings
*  Using the ExplorIR CO2 sensor. Make sure the sensor is connected to Serial1 on Giga
*/
//#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#include <Arduino.h>
#include "O2Sensor.h"

// #define rx 2                                          //define what pin rx is going to be
// #define tx 3                                          //define what pin tx is going to be

//commands to give to EZO O2 Sensor
// const char* writeCommandPolling = "C,0\r";        //sets the sensor to be polling mode
// const char* writeCommandCalibrate = "Cal20.9\r";  //command to calibrate sensor to fresh air value (400 ppm)
// const char* writeCommandGetValue = "R\r";         //command to receieve the CO2 value

/* CO2Sensor Constructor
 * Sets all values to 0, as they will be obtained once initialized
 */
O2Sensor::O2Sensor(){
  _pollState = 0;
  _calibrateValue = 0;
  _currentPercent = 0;

}

/* CO2Sensor Initialize block
 * Sets the sensor to a polling state
 * Turns off OK responses
 * locks the system to UART only
 * returns nothing
 */
void O2Sensor::initialize(){
  //Serial2.print("FACTORY\r");
  Serial2.print("*OK,0\r");                                         //turn off OK responses
  delay(500);
  Serial2.print("Plock,1\r");                                       //set the lock to UART only
  delay(100);                                             
  Serial2.print("C,0\r");                                           //switch to the polling state
  delay(100);
  Serial2.flush();                                                  //flush the serial2 port
  delay(100);                                                       //delay 100 ms

  float setupPercent = 0.00;                                        //initial percent value
  int setupComplete;
  while(setupComplete <= 3){                                        //keep asking for percentages until the value given is not 0 (trying to debug why this is happening)
    Serial.println("waiting for setup");
    setupPercent = getPercent();                                    //get percent
    if(setupPercent != 0.00){
      setupComplete ++;
    }
    else{
      setupComplete = 0;
    }
    //getOK();                                                        //get ok
    delay(2000);                                                    //delay 1s before asking again
  }
  //Serial.println("O2 Setup complete.");
  Serial2.flush();
}


/* O2Sensor Calibrate block
 * Calibrates the sensor to atmospheric air (20.9%)
 * Checks that the Value after the fact is around 20.9% to signal correct calibration
 * returns nothing
 */
void O2Sensor::calibrate(){
  Serial2.write("Cal,20.90\r");                          //calibrate command
  delay(5000);                                            //wait 5 seconds

  float calibrateValue = getPercent();                    //take a reading after calibration
  while(calibrateValue < 20.0 || calibrateValue > 22.0){  //if its not around atmosphere, get another reading until it is whats expected
    delay(2000);                                          //wait 2 seconds if not correct percent
    calibrateValue = getPercent();                         //get another value
  }
}

/* CO2Sensor getPercent
 * uses getPPM() to get the ppm value, then converts to a percentage
 * returns the percent of CO2 as a float
 */
float O2Sensor::getPercent(){
  //Serial.println("In function percent");
  Serial2.print("R\r");                                                 //write to O2 sensor for a reading
  _currentPercent = getResponse(writeCommandGetValue);                  //convert to a percentage
  //Serial.print("Current Percent: ");
  //Serial.println(_currentPercent);
  //_SensorReady = getOK();                                               //get the *OK message to know response finished
  // if(_SensorReady == 1){                                                //make sure the OK was received from tester
  //   //Serial.println("...(INFO): percent obtained");
  // }
  // else{                                                                 //if ok not received, something is wrong
  //   //Serial.println("...(INFO): error getting percent");
  // }
  Serial.print("in get percent  ");
  Serial.println(_currentPercent);
  return _currentPercent;                                               //return percentage read from sensor
}

/* CO2Sensor getResponse
 * takes in a string (going to change this)
 * records the response from the Serial1 port
 * returns the number as an integer from the response 
 */
float O2Sensor::getResponse(String command){
  _response = "";                                                   //clear the response
  while (Serial2.available() > 0) {                                 //while the O2 sensor port is available
    char incomingByte = (char)Serial2.read();                       //Read the incoming byte.
    _response += incomingByte;                                      //add it to the response
    if (incomingByte == '\r') {                                     //if the incoming character is a <CR>
      sensor_string_complete = true;                                //set the flag that it has been received (not important, but could be used)
      break;                                                        //break out of while loop
    }
  }
  if (sensor_string_complete == true) {                             //if a string from the O2 sensor has been received in its entirety
    //Serial.print("...(INFO): sensor String:   ");
  sensor_string_complete = false;  
  }
  //Serial.print("In Get Response: ");
  //Serial.println(_response);
  return _response.toFloat();                                       //return the full response
}

int O2Sensor::getOK(){
  _response2 = "";                                                  //clear OK response variable
  while (Serial2.available() > 0) {                                 //while the O2 sensor port is available
    char incomingByte = (char)Serial2.read();                       // Read the incoming byte.
    _response2 += incomingByte;                                     //add it to the response
    if (incomingByte == '\r') {                                     //if the incoming character is a <CR>
      sensor_string_complete = true;                                //set the flag
      break;                                                        //break
    }
  }
  if (sensor_string_complete == true) {                             //if a string from the Atlas Scientific product has been received in its entirety
    //Serial.print("OK:   ");
    //Serial.println(_response2);                                   //send that string to the PC's serial monitor
  sensor_string_complete = false;  
  }
  //Serial.print("OK Statement: ");
  //Serial.println(_response2);
  if(_response2.equals("*OK\r")){                                   //if the response is what we expect (an OK)
    return 1;                                                       //return 1
  }
  else{                                                             //if not
    return 0;                                                       //return 0
  }
}  

/* CO2Sensor extractNumber
 * Takes in string (such as "Z 00040")
 * and parses that to only obtain the number as an integer (int 40)
 * returns the number as an integer
 */
// int CO2Sensor::extractNumber(String data) {
//   String numberString = "";                         // This will store the numeric part of the message
//   for (int i = 0; i < data.length(); i++) {
//     if (isDigit(data[i])) {                         // Check if the character is a digit
//       numberString += data[i];                      // Append the digit to 'numberString'
//     }
//   }
//   if (numberString.length() > 0) {
//     return numberString.toInt();                    // Convert the numeric string to an integer
//   }
//   return 0;                                         // Return 0 as a default
// }