#ifndef fsmClass_h
#define fsmClass_h
#include "Arduino.h"
#include "sensor.h"
#include "motion.h"
#include "hole.h"
#include "rackManager.h"
#include "comms.h"

class fsm {
  public:
    void moveToAndUpdate(int x_amt, int y_amt);
    void moveZAndUpdate(int amt);
    void depositLoop();
    void setUp();
    void jogSliderUpUntilTouch();
    void systemReset();
    void communicationCheck();
    String getGrblReply();
    void resetMainFlags();
    void fsmMain();

  private:
    sensorManager sensor;
    motionManager motion;
    rackManager rack;
    commsManager comms;
    
    // Reset system flags
    bool systemReady = false;
    bool readyForDocking = false;
    bool depositMode = false;
    bool yReset = false;
    bool xReset = false;
    int currX = 0;
    int currY = 0;
    int currZ = 0;
    int test = 0;
    int commandSent = 0;
    int replyCount = 0;

    // Deposit logic flags
    bool commandSentDeposit = false;
};
#endif