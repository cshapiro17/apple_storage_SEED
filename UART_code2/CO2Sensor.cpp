#include <Arduino.h>
#include "CO2Sensor.h"


char writeCommandPolling[] = "K 2\r\n";       //command to set sensor to 'polling mode'
char writeCommandFilter[] = "a\r\n";         //command to respond with digital filter value (default 16?)
char writeCommandCalibrate[] = "G\r\n";      //command to calibrate sensor to fresh air value (400 ppm)
char writeCommandMultiplier[] = ".\r\n";     //command to receieve the multiplier value of the sensor
char writeCommandGetValue[] = "Z \r\n";     //command to receieve the CO2 value

CO2Sensor::CO2Sensor(){
  initialize();

}

void CO2Sensor::initialize(){
  Serial1.write(writeCommandFilter);
  _filterValue = getResponse();
  delay(1000);
  Serial1.write(writeCommandMultiplier);
  _multiplier = getResponse();
}


void CO2Sensor::calibrate(){
  Serial1.write(writeCommandCalibrate);                     //calibrate command
  delay(2000);
  Serial1.write(writeCommandGetValue);                       //get CO2 value
  _calibrateValue = getResponse();
  if(_calibrateValue <390 || _calibrateValue > 410){
    Serial.println("error calibrating CO2 sensor");
  }
  else{
    Serial.println("CO2 calibration success");
  }
}

int CO2Sensor::getPPM(){
  Serial1.write(writeCommandGetValue);
  _currentPPM = getResponse();                              //get CO2 PPM
  return _currentPPM;                                       //return PPM
}

float CO2Sensor::getPercent(){
  _currentPPM = getPPM();                                   //get CO2 PPM
  _currentPercent = _currentPPM / 10000;                    //convert to a percentage
  return _currentPercent;                                   //return percentage
}


int CO2Sensor::getResponse(){
  String _response = "";
  while (Serial1.available() > 0) {
    char incomingByte = Serial1.read();           // Read the incoming byte.
    // Check if the incoming byte is a newline character, indicating the end of a message.
    if (incomingByte == '\n') {
      sensorDataBuffer[bufferPosition] = '\0';    // string terminator to make it a proper C-style string.   
      Serial.println(sensorDataBuffer);           // sensorDataBuffer contains a complete message. 
      bufferPosition = 0;                         // Reset the buffer position for the next message.

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
