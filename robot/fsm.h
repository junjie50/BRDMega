#define RIGHT_AMT 25
#define LEFT_AMT 25

#ifndef fsmClass_h
#define fsmClass_h
#include "Arduino.h"
#include "sensor.h"
#include "motion.h"
#include "comms.h"

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
    commsManager comms;

    // Flags for resetting system at the starting of robot.
    // Reset the trayY slider.
    bool trayYReset = false;
    // Flags used to indicate that the system is ready for main logic loop.
    bool systemReady = false; // 
    
    // Logical Flags
    int systemMode = -1; // 1 means taking item from arm 0 means taking item from rackrack
    bool idleState = true;
    bool entry1 = false;
    bool exit1 = true;
    bool entry2 = false;
    bool entry3 = false;
    bool exit3 = true;
    int itemInSystem = 0;
    int itemType = 0;
    bool trayThreeReady = false;
    
    
    // Int side = 0 or 1 for y tray
    // bottomtray is y = 0 while upper tray with the arm is y = 1.
    int currTrayY = 0;
    int startTrayY = 0;
    int endTrayY = 1;
};

#endif
