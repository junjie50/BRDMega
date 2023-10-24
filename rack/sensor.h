// Sensor
#define SENSOR1 30
#define SENSOR2 31
#define SENSOR3 32
#define SENSOR4 33
#define SENSOR5 34
#define SENSOR6 35
#define TOUCH1 36
#define TOUCH2 37


// Define the class for sensorManager
#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"
class sensorManager {
  public:
    void sensorSetUp();
    bool sensorActivated(int num);
    int readSensor(int num);

  private:
    byte sensors[8] = {SENSOR1, SENSOR2, SENSOR3, SENSOR4, SENSOR5, SENSOR6, TOUCH1, TOUCH2};
};

#endif
