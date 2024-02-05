/*Code to work with and test the EXPLORIR-M-20 CO2 Sensor
  This sensor uses UART protocol to communicat with master
  /r/n is carriage return and line feed, corresponds to hex 0D and 0A

*/
// constant char writeCommandPolling = "K 1\r\n";       //command to set sensor to 'polling mode'
// char writeCommandFilter = "a \r\n";         //command to respond with digital filter value (default 16?)
// char writeCommandCalibrate = "G \r\n";      //command to calibrate sensor to fresh air value (400 ppm)
// char writeCommandMultiplier = ". \r\n";     //command to receieve the multiplier value of the sensor

void setup() {
  Serial.begin(9600);   //sensor uses 9600 BaudRate
  Serial.println("Z");
}

void loop() {
  

  // Wait for the response from the sensor
  while (!Serial.available()) {
    // Wait for data to become available
  }

  // Read the response from the sensor
  char response[64];
  int i = 0;
  while (Serial.available()) {
    char c = Serial.read();
    //Serial.print(c);
    if (c == '\n') {
      break;
    }
    response[i] = c;
    i++;
  }
  response[i] = '\0';

  // Print the response to the serial monitor
  //Serial.println("Return:")
  Serial.println(response);
}
