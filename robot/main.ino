#include "fsm.h"

fsm mainSystem;

void setup() {
  // Serial 0 for talking with PC
  Serial.begin(110000);
  // Serial 1 for talking with black box
  Serial1.begin(110000);

  // Setting up the sensors
  mainSystem.setUp();
}


void loop() {
  
}
