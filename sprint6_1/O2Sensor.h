
#ifndef O2Sensor_h
#define O2Sensor_h
#include "Arduino.h"

class O2Sensor{
  public:
  O2Sensor(void);

  void calibrate();
  /*getPPM writes to the CO2 sensor and waits for a response using getResponse()
  * @return integer value
  */
  //int getPPM();

  /*getPercent writes to the CO2 sensor using getPPM(), then converts the PPM value to a percent
  * which it returns
  */
  float getPercent();

  void initialize();

  float getResponse(String command);

  //int extractNumber(String data);

  int getOK();

  private:
  int _pollState;
  int _calibrateValue;
  float _currentPercent;
  char sensorDataBuffer[256];           //buffer character
  int bufferPosition = 0;               // Variable to keep track of the buffer position.
  String _response;
  String _response2;
  int _SensorReady;
  boolean sensor_string_complete;
  String sensorstring;
  const char* writeCommandPolling = "C,0\r";        //sets the sensor to be polling mode
  const char* writeCommandCalibrate = "Cal20.9\r";  //command to calibrate sensor to fresh air value (400 ppm)
  const char* writeCommandGetValue = "R\r";         //command to receieve the CO2 value
};
#endif