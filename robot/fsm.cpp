#include "Arduino.h"
#include "fsm.h"

void fsm::setUp() {
  sensors.sensorSetUp();
}

void fsm::resetMachine() {
  if(!trayYReset) {
    motion.jogSliderBeltDown();
    if(!sensors.readSensor(6)) {
      motion.motionStop();
      trayYReset = true;
    }
  }
  if(trayYReset) { // Can add more conditions to exit resetMachine function
    systemReady = true;
  }
}

void fsm::mainLogic() {
  if(!systemReady) {
    resetMachine();
  }
}
