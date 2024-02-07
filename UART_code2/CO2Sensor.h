
#ifndef CO2Sensor_h
#define CO2Sensor_h
#include "Arduino.h"

class CO2Sensor{
  public:
  CO2Sensor(void);

  void calibrate();
  /*getPPM writes to the CO2 sensor and waits for a response using getResponse()
  * @return integer value
  */
  int getPPM();

  /*getPercent writes to the CO2 sensor using getPPM(), then converts the PPM value to a percent
  * which it returns
  */
  float getPercent();

  void initialize();

  int getResponse(String command);

  int extractNumber(String data);

  private:
  int _filterValue;
  int _pollState;
  int _multiplier;
  int _calibrateValue;
  int _currentPPM;
  float _currentPercent;
  char sensorDataBuffer[256];           //buffer character
  int bufferPosition = 0;               // Variable to keep track of the buffer position.
  String _response;
  int _numberResponse;

};
#endif