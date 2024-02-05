/*Code to work with and test the EXPLORIR-M-20 CO2 Sensor
  This sensor uses UART protocol to communicat with master
  /r/n is carriage return and line feed, corresponds to hex 0D and 0A

*/
// constant char writeCommandPolling = "K 1\r\n";       //command to set sensor to 'polling mode'
// char writeCommandFilter = "a \r\n";         //command to respond with digital filter value (default 16?)
// char writeCommandCalibrate = "G \r\n";      //command to calibrate sensor to fresh air value (400 ppm)
// char writeCommandMultiplier = ". \r\n";     //command to receieve the multiplier value of the sensor
char sensorDataBuffer[256];
// Variable to keep track of the buffer position.
int bufferPosition = 0;

void setup() {
  Serial.begin(9600);                 //monitor 
  Serial1.begin(9600);    //CO2 sensor Serial RX0/TX0

  pinMode(6,OUTPUT);
  digitalWrite(6,1);
  Serial1.write("K 2\r\n");     //Set CO2 messaging to polling state
  delay(1000);
  Serial1.write("a\r\n");       //find the digital filter value (should be 16)
}

void loop() {
  //Serial1.write("a \r\n");
  delay(2000);
  Serial1.write("Z \r\n");    //get CO2 value
  while (Serial1.available() > 0) {
    // Read the incoming byte.
    char incomingByte = Serial1.read();
    // Check if the incoming byte is a newline character, indicating the end of a message.
    if (incomingByte == '\n') {
      // Add a string terminator to make it a proper C-style string.
      sensorDataBuffer[bufferPosition] = '\0';
      // Now sensorDataBuffer contains a complete message. Process it as needed.
      Serial.println(sensorDataBuffer);
      // Reset the buffer position for the next message.
      bufferPosition = 0;
    } else {
      // If it's not the newline character, add the byte to the buffer.
      // Make sure not to overflow the buffer.
      if (bufferPosition < sizeof(sensorDataBuffer) - 1) {
        sensorDataBuffer[bufferPosition] = incomingByte;
        bufferPosition++;
      }
    }

  // if (Serial1.available() > 0) {
  //   // Read the incoming byte from the sensor.
  //   int sensorData = Serial1.read();
    
  //   // Optionally, process the sensor data.

  //   // Print the sensor data to the Serial Monitor for debugging.
  //   Serial.print("Sensor Data: ");
  //   Serial.println(sensorData);
  // }

  // // Wait for the response from the sensor
  // while (!Serial.available()) {
  //   // Wait for data to become available
  // }

  // // Read the response from the sensor
  // char response[64];
  // int i = 0;
  // while (Serial.available()) {
  //   char c = Serial.read();
  //   //Serial.print(c);
  //   if (c == '\n') {
  //     break;
  //   }
  //   response[i] = c;
  //   i++;
  // }
  // response[i] = '\0';

  // // Print the response to the serial monitor
  // //Serial.println("Return:")
  // Serial.println(response);
}
}