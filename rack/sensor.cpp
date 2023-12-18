#include "sensor.h"
#include "Arduino.h"

void sensorManager::sensorSetUp() {
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);
  pinMode(SENSOR6, INPUT);

  pinMode(TOUCHX, INPUT);
  pinMode(TOUCHY, INPUT);
}


// sensorActivated returns true if objected is detected or touch sensor is pressed.
bool sensorManager::sensorActivated(int num) {
  return readSensor(num) == 0;
}

int sensorManager::readSensor(int num){
  return digitalRead(sensors[num]);
}