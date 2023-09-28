#ifndef fsmClass_h
#define fsmClass_h
#include "Arduino.h"
#include "sensor.h"

class fsm {
  public:
    void setUp();
    
  private:
    sensorManager sensors;
};

#endif
