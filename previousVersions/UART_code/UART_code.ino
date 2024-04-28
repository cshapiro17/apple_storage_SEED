/*Code to work with and test the EXPLORIR-M-20 CO2 Sensor
  This sensor uses UART protocol to communicat with master
  /r/n is carriage return and line feed, corresponds to hex 0D and 0A
*/


// char writeCommandPolling = "K 2\r\n";       //command to set sensor to 'polling mode'
// char writeCommandFilter = "a\r\n";         //command to respond with digital filter value (default 16?)
// char writeCommandCalibrate = "G \r\n";      //command to calibrate sensor to fresh air value (400 ppm)
// char writeCommandMultiplier = ". \r\n";     //command to receieve the multiplier value of the sensor

int count = 0;
char sensorDataBuffer[256];           //buffer character
int bufferPosition = 0;               // Variable to keep track of the buffer position.

void setup() {
  Serial.begin(9600);                 //Serial Monitor 
  Serial1.begin(9600);                //CO2 sensor Serial RX0/TX0
  Serial1.write("K 2\r\n");           //Set CO2 messaging to polling state

  delay(1000);                        //1 second delay

  Serial1.write("a\r\n");             //find the digital filter value (should be 16)
  //Serial1.write("A 32\r\n");         
  delay(1000);
  Serial1.write(".\r\n");            //find the multiplier value
}

void loop() {
  delay(2000);
  if (count == 100){
    Serial.println("calibrating sensor");
    Serial1.write("G\r\n");
  }
  count ++;
  Serial.println("Given Z \r\n");                        //get CO2 value
  Serial1.write("Z \r\n");                        //get CO2 value

  while (Serial1.available() > 0) {
    char incomingByte = Serial1.read();           // Read the incoming byte.
    // Check if the incoming byte is a newline character, indicating the end of a message.
    if (incomingByte == '\n') {
      sensorDataBuffer[bufferPosition] = '\0';    // string terminator to make it a proper C-style string.   
      Serial.println(sensorDataBuffer);           // sensorDataBuffer contains a complete message. 
      Serial.println(extractNumber(sensorDataBuffer));
      bufferPosition = 0;                         // Reset the buffer position for the next message.

    } else {
      // If it's not the newline character, add the byte to the buffer.
      // Make sure not to overflow the buffer.
      if (bufferPosition < sizeof(sensorDataBuffer) - 1) {
        sensorDataBuffer[bufferPosition] = incomingByte;
        bufferPosition++;
      }
    }
  }
}

int extractNumber(String data) {
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