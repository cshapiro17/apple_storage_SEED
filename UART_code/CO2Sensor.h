#include "Arduino.h"
#ifndef CO2Sensor_h
#define CO2Sensor_h


class CO2Sensor{
  public:
  CO2Sensor(void);

  void calibrate();

  int getPPM();

  float getPercent();

  void initialize();

  int getResponse();

  int extractNumber(String data);

  private:
  int _filterValue;
  int _multiplier;
  int _calibrateValue;
  int _currentPPM;
  int _currentPercent;
  char sensorDataBuffer[256];           //buffer character
  int bufferPosition = 0;               // Variable to keep track of the buffer position.
  String _response;
  int _numberResponse;

};
#endif