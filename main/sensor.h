// Sensor
#define SENSOR1 30 //Entrance from arm
#define SENSOR2 31 
#define SENSOR3 32
#define SENSOR4 33
#define SENSOR5 34 //Entrance from rack
#define SENSOR6 35 //Unused, legacy
#define TOUCH1 36
#define TOUCH2 37


// Define the class for sensorManager
#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"
class sensorManager {
  public:
    bool armEntryEmpty();
    bool rackEntryEmpty();

    void sensorSetUp();
    bool sensorActivated(int num);
    int readSensor(int num);

  private:
    const int armEntrySensor = 0;
    const int rackEntrySensor = 4;
    byte sensors[8] = {SENSOR1, SENSOR2, SENSOR3, SENSOR4, SENSOR5, SENSOR6, TOUCH1, TOUCH2};
};
#endif
