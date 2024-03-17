#define PASS_SENSOR 50

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
    void moveToChannel(Channel target);
    void moveToAndUpdate(int x, int y);
    void moveZAndUpdate(int amt);

    void collectOneItem();
    void storeOneItem();

    void resetDepositLoop();
    void resetStorageLoop();
    void resetRetrieveLoop();
    void depositLoop();
    void storageLoop();
    void retrieveLoop();
    void setUp();
    void jogSliderUpUntilTouch();
    void systemReset();
    void stateUpdate();
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
    bool robotDocked = false;
    bool yReset = false;
    bool xReset = false;
    int currX = 1;
    int currY = 0;
    int currZ = 0;
    int commandSent = 0;
    int replyCount = 0;
    unsigned long lastDetect;

    //Communication variables
    String nextCmd;

    // Deposit loop logic flags
    bool depositMode = false;
    bool commandSentDeposit = false;
    bool dockingSetUp = false;

    // Storage loop logic flags
    bool storageMode = false;
    String currLabel;
    int storageSwitch = 0;
    Channel* storageTarget = nullptr;
    bool commandSentStorage = false;
    
    // Retrieve loop logic flags
    bool retrieveMode = false;
    Channel* retrieveTarget = nullptr;
    int retrieveSwitch = 0;
    bool commandSentRetrieve = false;
    
    // System variables
    String coneList[4];
    int coneLength = 0;
    int currPtr = 0;
};
#endif