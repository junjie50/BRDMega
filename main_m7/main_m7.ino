#include "fsm.h"

fsm mainSystem;

void setup() {
  // Serial 0 for talking with PC
  Serial.begin(115200);
  // Serial 1 for talking with black box
  Serial1.begin(115200);

  // Setting up the sensors
  mainSystem.setUp();
  delay(1000);
}


void loop() {
  mainSystem.mainLogic();
}
