#include "sensor.h"
#include "fsm.h"

void fsm::setUp() {
  sensor.sensorSetUp();
}

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

void fsm::systemReset() {
  if(!xReset) {
    motion.jogSliderRight();
    if (sensor.sensorActivated(TOUCHXNUMBER)) {
      motion.motionStop();
      xReset = true;
    }
  }
  if(xReset) {
    systemReady = true;
  }
}

// This function is used to check for the commands from the robot.
void fsm::communicationCheck() {
  String next = comms.nextCommand();
  if(next.length() != 0) {
    Serial.println(next);
  }

  if(next == "deposit") {
    depositMode = true;
    resetDepositLoop();
    // HARDCODE ITEM TYPE HERE
    itemType = 'A';
  }
  else if(next == "retrieve") {
    retrieveMode = true;
    readyForDocking = false;
    // HARDCODE ITEM TYPE HERE
    itemType = 'A';
  }
  else if(!rack.robot.empty()) {
    storageMode = true;
  }
}

// Function used to filter the reply from grbl.
String fsm::getGrblReply() {
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
      comms.sendMessage("ready for deposit");
      readyForDocking = true;
    }
  }
  else {
    // Ready for docking procedures.
    // TODO: Add in function to allow deposit of 4 items.
    // First sensor sense item, move belt by a certain amount.
    // Until 4 items are deposited.
    if(!dockingSetUp) {
      rack.robot.label = itemType;
      dockingSetUp = true;
    }
    else {
      // Ending the deposit mode loop
      rack.robot.numberOfItems = 4;
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
      moveToAndUpdate(rack.diffX(currX, storageTarget.xOffset), rack.diffY(currY, storageTarget.yOffset));
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
      storageTarget.addOneItem();
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
  if(!systemReady) {
    // reset the system variables.
    systemReset();
  }
  else {
    communicationCheck(); // This part updates the rack's state.
    if(storageMode) {
      // Rack in storage mode. Contains items of a certain number in the buffer1.
      if (storageTarget == rackManager.NONE) {
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
      if(retrieveTarget == rackManager.NONE) {
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