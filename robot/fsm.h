#define RIGHT_AMT 50
#define LEFT_AMT 50

#ifndef fsmClass_h
#define fsmClass_h
#include "Arduino.h"
#include "sensor.h"
#include "motion.h"

class fsm {
  public:
    // Utility
    void setUp();
    void jogSliderDownUntilTouch();
    void jogSliderUpUntilTouch();
    void resetMachine();
    void resetTrayOne();
    void resetTrayTwo();
    void resetTrayThree();
    void resetStates();

    // Logic control
    void idle(); 
    void flowLogicFirst();
    void flowLogicSecond();
    void flowLogicThird();
    void flowLogic();
    void mainLogic();
    
  private:
    sensorManager sensors;
    motionManager motion;

     // Reset logical flags
    bool trayYReset = false;
    bool systemReady = false;
    
    // Logical Flags
    int systemMode = 1; // 1 means taking item from arm 0 means taking item from rackrack
    bool idleState = true;
    bool entry1 = false;
    bool exit1 = true;
    bool entry2 = false;
    bool entry3 = false;
    bool exit3 = true;
    int itemInSystem = 0;
    bool trayThreeReady = false;
    
    
    // Int side = 0 or 1 for y tray
    int currTrayY = 0;
    int startTrayY = 0;
    int endTrayY = 1;
};

#endif
