// Sensor
#define SENSOR1 30 // buffer entry
#define SENSOR2 31
#define SENSOR3 32 // human entry
#define SENSOR4 33
#define SENSOR5 34
#define SENSOR6 35
#define TOUCHX 36 // Touch sensor at the bottomright
#define TOUCHY 37
#define BUFFERENTRY 0

// define the sensor index in the list.
#define TOUCHXNUMBER 6
#define TOUCHYNUMBER 7

/**
 * This class manages the functionality of the sensor.
 */
#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"
class sensorManager {
  const int activationDelay = 1500;
  public:
    void sensorSetUp();
    bool sensorActivated(int num);
    int readSensor(int num);
    bool bufferEntryActivated();

  private:
    byte sensors[8] = {SENSOR1, SENSOR2, SENSOR3, SENSOR4, SENSOR5, SENSOR6, TOUCHX, TOUCHY};
};
#endif