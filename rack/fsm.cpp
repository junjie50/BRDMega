#include "sensor.h"
#include "fsm.h"

/************************************************
Setup functions for different classes.
 ***********************************************/
void fsm::setUp() {
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
void fsm::moveToAndUpdate(int x_amt, int y_amt) {
  if(x_amt != 0) {
    motion.moveX(x_amt);
    currX += x_amt;
    commandSent += 1;
  }
  if(y_amt != 0){
    motion.moveY(y_amt);
    currY += y_amt;
    commandSent += 1;
  }
}

void fsm::moveZAndUpdate(int amt) {
  motion.moveZ(amt);
  currZ += amt;
}


/************************************************
Communication utility functions.
 ***********************************************/

// This function checks for the command and update
// the state of the rack.
void fsm::stateUpdate() {
  String firstTwo = nextCmd.substring(0,2);
  if(nextCmd.length() != 0) {
    Serial.println(next);
  }
  if(firstTwo == "de") {
    depositMode = true;
    int start = 0;
    int ind = nextCmd.indexOf(',', start);
    while(start < nextCmd.length() && ind != -1) {
      target[targetLength] = nextCmd.charAt(ind + 1);
      start = ind + 2;
      targetLength += 1;
      int ind = nextCmd.indexOf(',', start);
    }
    resetDepositLoop();
  }
  else if(nextCmd == "re") {
    retrieveMode = true;
    readyForDocking = false;
    depositMode = true;
    int start = 0;
    int ind = nextCmd.indexOf(',', start);
    while(start < nextCmd.length() && ind != -1) {
      target[targetLength] = nextCmd.charAt(ind);
      start = ind + 1;
      targetLength += 1;
      int ind = nextCmd.indexOf(',', start);
    }
  }
  else if(!rack.robot.empty()) {
    storageMode = true;
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
      // Move slider to the highest position.
      moveToAndUpdate(0, rack.diffHighestY(currY));
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
  else {
    // Ready for docking procedures.
    // TODO: Add in function to allow deposit of 4 items.
    // First sensor sense item, move belt by a certain amount.
    // Until 4 items are deposited.
    if(sensor.bufferEntryActivated()) {
      bufferLength += 1;
    }

    if(bufferLength == targetLength() {
      depositMode = false;
    }

    
  }
}

void fsm::resetDepositLoop() {
  commandSentDeposit = false;
  dockingSetUp = false;
  readyForDocking = false;
  replyCount = 0;
  commandSent = 0;
}


/************************************************
 * This loop will run until robot is done storing
 * the items.
 ***********************************************/

 void fsm::storageLoop() {
  if (rack.robot.empty()) {
    // Finish storage the items.
    storageMode = false;
  }
  if(storageSwitch == 0) {
    if(!commandSentStorage){
      // Move slider to the correct position of the robot channel.
      moveToAndUpdate(rack.toRobotChannelX(currX), rack.toRobotChannelY(currY));
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
      rack.robot.removeOneItem();
    }
  }
  else {
    if (!commandSentStorage){
      // Move slider to the correct position.
      moveToAndUpdate(rack.diffX(currX, storageTarget->xOffset), rack.diffY(currY, storageTarget->yOffset));
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
      storageTarget->addOneItem();
    }
  }
}

void fsm::retrieveLoop() {
  storageSwitch = 0;
  commandSentStorage = false;
}

void fsm::resetMainFlags() {
  depositMode = false;
}


/************************************************
 * This is the starting point of the program
 ***********************************************/
void fsm::fsmMain() {
  nextCmd = comms.nextCommand();

  if(!systemReady) {
    // reset the system variables.
    systemReset();
  }
  else {
    stateUpdate();
    if(storageMode) {
      // Rack in storage mode. Contains items of a certain number in the buffer1.
      if (storageTarget == nullptr) {
        // find the next free storage channel.
        storageTarget = rack.nextFree(itemType);
        // TO DO error checking, if no more free storage, activate seesaw!
      }
      else {
        storageLoop();
      }
    }
    else if(retrieveMode) {
      // In retrieveMode when robot is coming to collect the items.
      if(retrieveTarget == nullptr) {
        retrieveTarget = rack.locateChannel(itemType);
        // if retrieveTarget is still NULL, error as item not inside.
        // Send worker to insert into rack.
      }
      else {
        retrieveLoop();
      }
    }
    else if(depositMode) {
      // For robot to deposit the items.
      depositLoop();
    }
  }
}