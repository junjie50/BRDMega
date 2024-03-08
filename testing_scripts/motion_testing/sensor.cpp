#include "sensor.h"
#include "Arduino.h"

void sensorManager::sensorSetUp() {
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);
  pinMode(SENSOR6, INPUT);

  pinMode(TOUCH1, INPUT);
  pinMode(TOUCH2, INPUT);
}

bool sensorManager::armEntryEmpty() {
  return !sensorActivated(armEntrySensor);
}
bool sensorManager::rackEntryEmpty() {
  return !sensorActivated(rackEntrySensor);
}

// sensorActivated returns true if objected is detected or touch sensor is pressed.
bool sensorManager::sensorActivated(int num) {
  return readSensor(num) == 0;
}

int sensorManager::readSensor(int num){
  return digitalRead(sensors[num]);
}
