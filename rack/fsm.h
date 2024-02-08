#ifndef fsmClass_h
#define fsmClass_h
#include "Arduino.h"
#include "sensor.h"
#include "motion.h"
#include "channel.h"
#include "rackManager.h"
#include "comms.h"

class fsm {
  public:
    void moveToAndUpdate(int x_amt, int y_amt);
    void moveZAndUpdate(int amt);
    void resetDepositLoop();
    void resetStorageLoop();
    void depositLoop();
    void storageLoop();
    void retrieveLoop();
    void setUp();
    void jogSliderUpUntilTouch();
    void systemReset();
    void communicationCheck();
    String getGrblReply();
    void resetMainFlags();
    void fsmMain();
    void waitForReply();

  private:
    sensorManager sensor;
    motionManager motion;
    rackManager rack;
    commsManager comms;
    
    // Reset system flags
    bool systemReady = false;
    bool readyForDocking = false;
    bool yReset = false;
    bool xReset = false;
    int currX = 0;
    int currY = 0;
    int currZ = 0;
    int commandSent = 0;
    int replyCount = 0;

    // Deposit loop logic flags
    bool depositMode = false;
    bool commandSentDeposit = false;
    bool dockingSetUp = false;

    // Storage loop logic flags
    bool storageMode = false;
    int storageSwitch = 0;
    Channel storageTarget = rackManager.NONE;
    bool commandSentStorage = false;

    // Retrieve loop logic flags
    bool retrieveMode = false;
    Channel retrieveTarget = rackManager.NONE;
    
    // System variables
    String itemType = "";
};
#endif