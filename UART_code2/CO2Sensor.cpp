#include <Arduino.h>
#include "CO2Sensor.h"


//String writeCommandPolling = "K 2\r\n";       //command to set sensor to 'polling mode'
const char* writeCommandPolling = "K 2\r\n";

const char* writeCommandFilter = "a\r\n";         //command to respond with digital filter value (default 16?)
const char* writeCommandCalibrate = "G\r\n";      //command to calibrate sensor to fresh air value (400 ppm)
const char* writeCommandMultiplier = ".\r\n";     //command to receieve the multiplier value of the sensor
const char* writeCommandGetValue = "Z \r\n";     //command to receieve the CO2 value

CO2Sensor::CO2Sensor(){
  _filterValue = 0;
  _multiplier = 0;
  _calibrateValue = 0;
  _currentPPM = 0;
  _currentPercent = 0.0; // Assuming _currentPercent should be a float
  bufferPosition = 0; // Reset buffer position

}

void CO2Sensor::initialize(){
  Serial1.write(writeCommandPolling);
  delay(100);
  _pollState = getResponse(writeCommandPolling);

  delay(2000);
  Serial.println("INITIAL TEST");
  Serial1.write(writeCommandFilter);
  delay(100);
  _filterValue = getResponse(writeCommandFilter);
  delay(1000);
  Serial.print("filter: ");
  Serial.println(_filterValue);
  delay(1000);
  Serial1.write(writeCommandMultiplier);
  delay(100);
  _multiplier = getResponse(writeCommandMultiplier);
  Serial.print("multiplier: ");
  Serial.println(_multiplier);
}


void CO2Sensor::calibrate(){
  Serial1.write(writeCommandCalibrate);                     //calibrate command
  delay(2000);
  Serial1.write(writeCommandGetValue);  
  delay(100);                     //get CO2 value
  _calibrateValue = getResponse(writeCommandCalibrate);
  if(_calibrateValue <390 || _calibrateValue > 410){
    Serial.println("error calibrating CO2 sensor");
  }
  else{
    Serial.println("CO2 calibration success");
  }
}

int CO2Sensor::getPPM(){
  Serial1.write(writeCommandGetValue);
  //Serial.println("In getPPM()");
  _currentPPM = getResponse(writeCommandGetValue);                              //get CO2 PPM
  return _currentPPM * _multiplier;                                       //return PPM
}

float CO2Sensor::getPercent(){
  _currentPPM = getPPM();                                   //get CO2 PPM
  _currentPercent = _currentPPM / 10000.0;                    //convert to a percentage
  // Serial.print("getPercent(): ");
  // Serial.println(_currentPercent);
  return _currentPercent;                                   //return percentage
}


int CO2Sensor::getResponse(String command){
  String _response = "";
  bool commandMatched = false;
  // while (Serial1.available() > 0) {
  //   char incomingByte = Serial1.read(); // Read the incoming byte.
    
  //   if (!commandMatched) {
  //     // Accumulate characters to match with the command.
  //     _response += incomingByte;
  //     // Check if the accumulated response starts with the command.
  //     if (_response.startsWith(command)) {
  //       commandMatched = true;
  //        //_response = "";
  //     }
  //     else{
  //       _response = "";
  //     }
    
  //   } else {
  //     // Once the command is matched, proceed as before.
  //     if (incomingByte == '\n') {
  //       sensorDataBuffer[bufferPosition] = '\0'; // String terminator to make it a proper C-style string.
  //       Serial.println(sensorDataBuffer); // sensorDataBuffer contains a complete message.
  //       bufferPosition = 0; // Reset the buffer position for the next message.
  //       break; // Break the loop if the end of a message is detected.
  //     } else {
  //       // If it's not the newline character, add the byte to the buffer.
  //       // Make sure not to overflow the buffer.
  //       if (bufferPosition < sizeof(sensorDataBuffer) - 1) {
  //         sensorDataBuffer[bufferPosition] = incomingByte;
  //         _response += incomingByte;
  //         bufferPosition++;
  //       }
  //     }
  //   }
  // }
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
      // Make sure not to overflow the buffer.
      if (bufferPosition < sizeof(sensorDataBuffer) - 1) {
        sensorDataBuffer[bufferPosition] = incomingByte;
        _response += incomingByte;
        bufferPosition++;
      }
    }
  }
  _numberResponse = extractNumber(_response);
  return _numberResponse;
}

int CO2Sensor::extractNumber(String data) {
  String numberString = ""; // This will store the numeric part of the message
  for (int i = 0; i < data.length(); i++) {
    if (isDigit(data[i])) { // Check if the character is a digit
      numberString += data[i]; // Append the digit to 'numberString'
    }
  }
  if (numberString.length() > 0) {
    return numberString.toInt(); // Convert the numeric string to an integer
  }
  return 0; // Return 0 or an appropriate value if no number was found
}
