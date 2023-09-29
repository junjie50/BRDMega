#ifndef fsmClass_h
#define fsmClass_h
#include "Arduino.h"
#include "sensor.h"
#include "motion.h"

class fsm {
  public:
    void setUp();
    void resetMachine();
    void mainLogic();
    
  private:
    sensorManager sensors;
    motionManager motion;

     // Reset logical flags
    bool trayYReset = false;
    bool systemReady = false;
    
    // Logical Flags
    int armMode = 1; // 1 means taking item from arm 0 means taking item from rack
    bool idleState = true;
    bool entry1 = false;
    bool exit1 = true;
    bool entry2 = false;
    bool pause2 = false;
    bool exit2 = true;
    bool entry3 = false;
    bool exit3 = true;
    
    
    // Int side = 0 or 1 for y tray
    int currTrayY = 0;
    int startTrayY = 0;
};

#endif
