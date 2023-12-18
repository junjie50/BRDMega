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

void fsm::communicationCheck() {
  String next = comms.nextCommand();
  if(next.length() != 0) {
    Serial.println(next);
  }

  if(next == "deposit") {
    depositMode = true;
    readyForDocking = false;
    commandSentDeposit = false;
    replyCount = 0;
  }
  else if(next == "done") {
    resetMainFlags();
  }
}

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
      moveToAndUpdate(rack.diffX(currX, 0), 10);
      // rack.diffHighestY(currY)
      commandSentDeposit = true;
    }
    else if (replyCount < commandSent){
      if(Serial1.available()) {
        String grblMSG = getGrblReply();
        if(grblMSG == "ok") {
          replyCount += 1;
        }
      }
    }
    else{
      readyForDocking = true;
    }
  }
  else {
    Serial.println("Ready for docking");
  }
}

void fsm::resetMainFlags() {
  depositMode = false;
}

/************************************************
 * This is the starting point of the program
 ***********************************************/

int count = 1;
void fsm::fsmMain() {
  if(!systemReady) {
    systemReset();
  }
  else {
    //idle mode check for system modes via communication
    // communicationCheck();
    // if(depositMode) {
    //   depositLoop();
    // }
    if(count == 1) {
      count = 2;
      moveToAndUpdate(0, rack.diffHighestY(currY));
      delay(20000);
      moveToAndUpdate(0, rack.diffY(currY, 72));
      delay(20000);
      moveToAndUpdate(-1, rack.diffSecondY(currY));
      delay(20000);
      moveToAndUpdate(1, rack.diffY(currY, 72));
      delay(15000);
    }
  }
}