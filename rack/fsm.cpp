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
    readyForDocking = false;
    commandSentDeposit = false;

    // HARDCODE ITEM TYPE HERE
    itemType = 'A';
    itemCount = 4;
  }
  else if(next == "retrieve") {
    retrieveMode = true;
    readyForDocking = false;
    commandSentRetrieve = false;
    // HARDCODE ITEM TYPE HERE
    itemType = 'A';
    itemCount = 4;

  }
  else if(next == "done") {
    if(depositMode) {
      depositMode = false;
      storageMode = true;
    }
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
      if(Serial1.available()) {
        String grblMSG = getGrblReply();
        if(grblMSG == "ok") {
          replyCount += 1;
        }
      }

      if(replyCount == commandSent) {
        // SEND A MESSAGE TO THE ROBOT TO DOCK AND DEPOSIT.
        comms.sendMessage("ready for deposit");
      }
    }
    else{
      readyForDocking = true;
    }
  }
  else {
    // Ready for docking procedures
    Serial.println("Ready for docking");
  }
}

/************************************************
 * This loop will run until robot is done storing
 * the items.
 ***********************************************/

 void fsm::storageLoop() {
  if(storageSwitch == 0) {
    if(!commandSentStorage){
      // Move slider to the correct position.
      moveToAndUpdate(rack.toRobotChannelX(currX), rack.toRobotChannelY(currY));
      commandSentStorage = true;
    }
    else if (replyCount < commandSent){
      if(Serial1.available()) {
        String grblMSG = getGrblReply();
        if(grblMSG == "ok") {
          replyCount += 1;
        }
      }
      if(replyCount == commandSent) {
        // Reached the buffer position to collect
        // Collect one item from the buffer.
        storageSwtich = 1;
        commandSentStorage = false;
      }
    }
  else {
    if(!commandSentStorage){
      // Move slider to the correct position.
      moveToAndUpdate(rack.diffX(currX, storageTarget.xOffset), rack.diffY(currY, storageTarget.yOffset));
      commandSentStorage = true;
    }
    else if (replyCount < commandSent){
      if(Serial1.available()) {
        String grblMSG = getGrblReply();
        if(grblMSG == "ok") {
          replyCount += 1;
        }
      }
      if(replyCount == commandSent) {
        // Reached the target position to deposit
        // Remove one item from the slider
        storageSwtich = 0;
        commandSentStorage = false;
        itemCount -= 1;
      }
    }
  }
  if (itemCount == 0) {
    // Finish storage the items.
    storageMode = false;
  }
}

void fsm::retrieveLoop() {

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
  else if (storageMode) {
    // Rack in storage mode. Contains items of a certain number in the buffer1.
    if (storagetTarget == nullptr) {
      // find the next free storage channel.
      storageTarget = rack.nextFree(itemType);
      // TO DO error checking, if no more free storage, activate seesaw!!!!!!!!!!
    }
    else {
      storageLoop();
    }
  }
  else if (retrieveMode) {
    // In retrieveMode when robot is coming to collect the items.
    if(retrieveTarget == nullptr) {
      retrieveTarget = rack.locateChannel(itemType);
      // if retrieveTarget is still nullptr, error as item not inside.
      // Send worker to insert into rack.
    }
    else if()
  }
  else {
    // idle mode check for system modes via communication
    communicationCheck();
    if(depositMode) {
      depositLoop();
    }
  }
}