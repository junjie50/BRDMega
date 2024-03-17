#include "sensor.h"
#include "fsm.h"
#include "comms.h"

/************************************************
Setup functions for different classes.
 ***********************************************/
void fsm::setUp() {
  comms.setup();
  sensor.sensorSetUp();
}

/************************************************
On boot reset function for the robot.
 ***********************************************/
void fsm::systemReset() {
  if(!xReset) {
    motion.jogSliderRight();
    if(sensor.sensorActivated(TOUCHXNUMBER)) {
      motion.motionStop();
      xReset = true;
    }
  }

  if(xReset) {
    systemReady = true;
  }
}

/************************************************
Movement utility functions.
 ***********************************************/
 // Calling this function moves the item to the the targeted channel.
void fsm::moveToChannel(Channel target) {
  moveToAndUpdate(target.xOffset, target.yOffset);
}

void fsm::moveToAndUpdate(int x, int y) {
  int x_amt = x - currX;
  int y_amt = y - currY;
  if(x_amt != 0 && x_amt != 0) {
    motion.moveXY(x_amt, y_amt);
    currX += x_amt;
    currY += y_amt;
    commandSent += 1;
  }
  else if(x_amt != 0) {
    motion.moveX(x_amt);
    currX += x_amt;
    commandSent += 1;
  }
  else if(y_amt != 0){
    motion.moveY(y_amt);
    currY += y_amt;
    commandSent += 1;
  }
}

void fsm::moveZAndUpdate(int amt) {
  commandSent += 1;
  motion.moveZ(amt);
  currZ += amt;
}

void fsm::collectOneItem() {
  motion.collectOneItem();
  commandSent += 1;
}

void fsm::storeOneItem() {
  motion.storeOneItem();
  commandSent += 1;
}

/************************************************
Communication utility functions.
 ***********************************************/

// This function checks for the command and update
// the state of the rack.
void fsm::stateUpdate() {
  // Short circuit and return the circuit.
  if(nextCmd.length() == 0) {
    return;
  }

  int start = 0;
  int end = nextCmd.indexOf(',');
  String command = nextCmd.substring(0, end);
  start = end;
  coneLength = 0;
  if(command == "PrepareRackEmpty" || command == "PrepareRackGet") {
    if(command == "PrepareRackEmpty") {
      depositMode = true;
      resetDepositLoop();
    }
    else{
      retrieveMode = true;
      resetRetrieveLoop();
    }

    // Updates the coneList according to the message received from magazine.
    while(start < nextCmd.length()) {
      coneList[coneLength] = nextCmd.substring(start + 1, start + 4);
      start = start + 4;
      coneLength += 1;
    }
  }
  else if(command == "EmptyMag" || command == "GetConeMag") {
    robotDocked = true;
    lastDetect = millis() - 1500;
    currPtr = 0;
  }
}

// Function used to filter the reply from grbl.
String fsm::getGrblReply(){
  String grblMSG = Serial1.readString();
  String result = "";
  for(char ch: grblMSG) {
    if (ch != '\r') {
      result += ch;
    }
  }
  return result;
}

// Call this function to wait for response and add 1 to the replyCount.
void fsm::waitForReply() {
  if(Serial1.available()) {
    String grblMSG = getGrblReply();
    if(grblMSG == "ok") {
      replyCount += 1;
    }
  }
}

/************************************************
Communication utility functions.
 ***********************************************/


/************************************************
 * This loop will run until robot is done depositing
 * the items.
 ***********************************************/
void fsm::depositLoop() {
  if(!readyForDocking) {
    if(!commandSentDeposit){
      // clear serial buffer
      comms.clearSerial1();

      // Move slider to the highest position.
      moveToAndUpdate(currX, rack.highest_y);
      commandSentDeposit = true;
    }
    else if (replyCount < commandSent){
      waitForReply();
    }
    else{
      // SEND A MESSAGE TO THE ROBOT TO DOCK AND DEPOSIT.
      comms.sendMessage("done");
      readyForDocking = true;
    }
  }
  else if(robotDocked) {
    // First sensor sense item, move belt by a certain amount.
    // Until all items are deposited.
    if(millis() - lastDetect > 1500 && sensor.bufferEntryActivated()) {
      lastDetect = millis();
      // move buffer left by fixed distance.
      motion.moveRobotBufferLeft(PASS_SENSOR);
      rack.robot.addOneItem(coneList[currPtr]);
      currPtr += 1;
      if(rack.robot.numberOfItems == coneLength) {
        depositMode = false;
        comms.sendMessage("done");
        storageMode = true; // Transit to store the cones.
      }
    }
  }
}

void fsm::resetDepositLoop() {
  comms.clearSerial1();
  commandSentDeposit = false;
  dockingSetUp = false;
  readyForDocking = false;
  replyCount = 0;
  commandSent = 0;
  robotDocked = false;
  currPtr = 0;
}


/************************************************
 * This loop will run until robot is done storing
 * the items.
 ***********************************************/

 void fsm::storageLoop() {
  if (rack.robot.empty()) {
    storageMode = false;
  }
  else if(storageSwitch == 0) {
    if(!commandSentStorage){
      // Move slider to the correct position of the robot channel.
      moveToChannel(rack.robot);
      collectOneItem();
      commandSentStorage = true;
    }
    else if (replyCount < commandSent) {
      waitForReply();
    }
    else {
      // Reached the buffer position to collect
      // Collect one item from the buffer.
      storageSwitch = 1;
      commandSentStorage = false;
      currLabel = rack.robot.pop();
      storageTarget = rack.nextFree(currLabel);
    }
  }
  else {
    if (!commandSentStorage){
      // Move slider to the correct position.
      moveToChannel(*storageTarget);
      storeOneItem();
      commandSentStorage = true;
    }
    else if (replyCount < commandSent){
      waitForReply();
    }
    else {
      // Reached the target position to deposit
      // Add one item to the storage channel.
      storageSwitch = 0;
      commandSentStorage = false;
      storageTarget->addOneItem(currLabel);
    }
  }
}

void fsm::resetStorageLoop() {
  comms.clearSerial1();
  commandSentStorage = false;
  replyCount = 0;
  commandSent = 0;
}

// Retrieving the items for according to the coneList.
void fsm::retrieveLoop() {
  // Check if the robot is ready for docking operation.
  // For retrieving we have to ensure that the cones in rack is according to coneList.
  if(!readyForDocking){
    if(rack.robot.numberOfItems == coneLength) {
      readyForDocking = true;
      comms.sendMessage("done"); // Tell robot that we are ready for retrival docking.
    }
    else if(retrieveSwitch == 0) {
      if(!commandSentRetrieve) {
        String target = coneList[rack.robot.numberOfItems];
        retrieveTarget = rack.locateChannel(target);
        moveToChannel(*retrieveTarget);
        collectOneItem();
        retrieveTarget->removeOneItem();
        commandSentRetrieve = true;
      }
      else if(commandSent > replyCount) {
        waitForReply();
      }
      else{
        retrieveSwitch = 1;
        commandSentRetrieve = false;
      }
    }
    else {
      if(!commandSentRetrieve) {
        moveToChannel(rack.robot);
        depositOneItem();
        rack.robot.addOneItem(coneList[rack.robot.numberOfItems]);
        commandSentRetrieve = true;
      }
      else if(commandSent > replyCount) {
        waitForReply();
      }
      else{
        retrieveSwitch = 0;
        commandSentRetrieve = false;
      }
    }
  }
  else {
    if(robotDocked) {
      if(millis() - lastDetect > 1500 && sensor.bufferEntryActivated()) {
        lastDetect = millis();
        rack.robot.removeOneItem();
        if(rack.robot.numberOfItems == 0) {
          motion.motionStop();
          retrieveMode = false;
          motion.moveRobotBufferRight(PASS_SENSOR);
        }
      }
      else {
        motion.jogRobotBufferRight();
      }
    }
  }
}

void fsm::resetRetrieveLoop() {
  comms.clearSerial1();
  retrieveSwitch = 0;
  commandSentRetrieve = false;
  commandSent = 0;
  replyCount = 0;
  readyForDocking = false;
  robotDocked = false;
}

void fsm::resetMainFlags() {
  depositMode = false;
}

/************************************************
 * This is the starting point of the program
 ***********************************************/
void fsm::fsmMain() {
  nextCmd = comms.nextCommand(); // Check for the next command.

  if(!systemReady) {
    // reset the system variables.
    systemReset();
  }
  else {
    stateUpdate();
    if(storageMode) {
      // Rack in storage mode.
      storageLoop();
    }
    else if(retrieveMode) {
      retrieveLoop();
    }
    else if(depositMode) {
      // For robot to deposit the items.
      depositLoop();
    }
  }
}