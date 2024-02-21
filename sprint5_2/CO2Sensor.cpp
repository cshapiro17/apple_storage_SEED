/* UART_code2 is used to integrate the CO2Sensor library to takes CO2 readings
*  Using the ExplorIR CO2 sensor. Make sure the sensor is connected to Serial1 on Giga
*/

#include <Arduino.h>
#include "CO2Sensor.h"


//commands to give to ExplorIR Sensor
const char* writeCommandPolling = "K 2\r\n";
const char* writeCommandFilter = "a\r\n";         //command to respond with digital filter value (default 16?)
const char* writeCommandCalibrate = "G\r\n";      //command to calibrate sensor to fresh air value (400 ppm)
const char* writeCommandMultiplier = ".\r\n";     //command to receieve the multiplier value of the sensor
const char* writeCommandGetValue = "Z \r\n";      //command to receieve the CO2 value

/* CO2Sensor Constructor
 * Sets all values to 0, as they will be obtained once initialized
 */
CO2Sensor::CO2Sensor(){
  _filterValue = 0;
  _multiplier = 0;
  _calibrateValue = 0;
  _currentPPM = 0;
  _currentPercent = 0.0; // Assuming _currentPercent should be a float
  bufferPosition = 0; // Reset buffer position

}

/* CO2Sensor Initialize block
 * Sets the sensor to a polling state
 * Finds and Records Filter value
 * Finds and Records Multiplier Value
 * returns nothing
 */
void CO2Sensor::initialize(){
  Serial1.write(writeCommandPolling);                       //switch to the polling state
  delay(100);                                               //100 ms delay to allow process time
  _pollState = getResponse(writeCommandPolling);            //record polling state, should always be 2
  delay(1000);
  Serial1.write(writeCommandFilter);                        //find the filter value
  delay(100);                                               //100 ms delay to allow process time
  _filterValue = getResponse(writeCommandFilter);           //record filter value
  Serial.print("filter: ");                                 // print filter value
  Serial.println(_filterValue);
  delay(1000);
  Serial1.write(writeCommandMultiplier);                    //find the multiplier
  delay(100);                                               //100 ms delay to allow process time
  _multiplier = getResponse(writeCommandMultiplier);        //record the multiplier value
  Serial.print("multiplier: ");                             //print multiplier value
  Serial.println(_multiplier);
}

/* CO2Sensor Calibrate block
 * Calibrates the sensor to atmospheric air (400 PPM)
 * Checks that the Value after the fact is around 400 PPM to signal correct calibration
 * returns nothing
 */
void CO2Sensor::calibrate(){
  Serial1.write(writeCommandCalibrate);                     //calibrate command
  delay(2000);
  Serial1.write(writeCommandGetValue);                      //get CO2 value
  delay(100);                                               //100 ms delay to allow process time       
  _calibrateValue = getResponse(writeCommandCalibrate);     //get the calibrate value, even though this number does not matter
  if(_calibrateValue*_multiplier <390 || _calibrateValue*_multiplier > 410){   //make sure it is around the expected value
    Serial.println("error calibrating CO2 sensor");
  }
  else{
    Serial.println("CO2 calibration success");
  }
}

/* CO2Sensor getPPM 
 * Takes a reading of CO2
 * returns the PPM of CO2 
 */
int CO2Sensor::getPPM(){
  Serial1.write(writeCommandGetValue);
  delay(100);                                               //100 ms delay to allow process time
  _currentPPM = getResponse(writeCommandGetValue);          //get CO2 PPM
  return _currentPPM * _multiplier;                         //return PPM, needs to be multiplied by multiplier
}

/* CO2Sensor getPercent
 * uses getPPM() to get the ppm value, then converts to a percentage
 * returns the percent of CO2 as a float
 */
float CO2Sensor::getPercent(){
  _currentPPM = getPPM();                                   //get CO2 PPM
  _currentPercent = _currentPPM / 10000.00;                  //convert to a percentage
  return _currentPercent;                                   //return percentage
}

/* CO2Sensor getResponse
 * takes in a string (going to change this)
 * records the response from the Serial1 port
 * returns the number as an integer from the response 
 */
int CO2Sensor::getResponse(String command){
  String _response = ""; 
  while (Serial1.available() > 0) {
    char incomingByte = Serial1.read();           // Read the incoming byte.
    // Check if the incoming byte is a newline character, indicating the end of a message.
    if (incomingByte == '\n') {
      sensorDataBuffer[bufferPosition] = '\0';    // string terminator to make it a proper C-style string.   
      Serial.println(sensorDataBuffer);           // sensorDataBuffer contains a complete message. 
      bufferPosition = 0;                         // Reset the buffer position for the next message.
      break;

    } else {
      // If it's not the newline character, add the byte to the buffer.
      if (bufferPosition < sizeof(sensorDataBuffer) - 1) {
        sensorDataBuffer[bufferPosition] = incomingByte;
        _response += incomingByte;
        bufferPosition++;
      }
    }
  }
  _numberResponse = extractNumber(_response);     //get the number only as an integer
  return _numberResponse;                         //return the number only
}

/* CO2Sensor extractNumber
 * Takes in string (such as "Z 00040")
 * and parses that to only obtain the number as an integer (int 40)
 * returns the number as an integer
 */
int CO2Sensor::extractNumber(String data) {
  String numberString = "";                         // This will store the numeric part of the message
  for (int i = 0; i < data.length(); i++) {
    if (isDigit(data[i])) {                         // Check if the character is a digit
      numberString += data[i];                      // Append the digit to 'numberString'
    }
  }
  if (numberString.length() > 0) {
    return numberString.toInt();                    // Convert the numeric string to an integer
  }
  return 0;                                         // Return 0 as a default
}