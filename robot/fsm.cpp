#include "Arduino.h"
#include "fsm.h"

//######################### UTILITY FUNCTIONS ######################### //
void fsm::setUp() {
  sensors.sensorSetUp();
}

// Jog slider down to touch sensor, set the position control variable.
void fsm::jogSliderDownUntilTouch() {
  motion.jogSliderBeltDown();
  if(sensors.sensorActivated(6)) {
    motion.motionStop();
    currTrayY = startTrayY;
    motion.moveSliderBeltUp(4);
  }
}

// Jog slider up to touch sensor, set the position control variable.
void fsm::jogSliderUpUntilTouch() {
  motion.jogSliderBeltUp();
  if(sensors.sensorActivated(7)) {
    motion.motionStop();
    motion.moveSliderBeltDown(5);
    currTrayY = endTrayY;
  }
}

// Function to reset the states of the machine.
void fsm::resetMachine() {
  if(!trayYReset) {
    motion.jogSliderBeltDown();
    if(sensors.sensorActivated(6)) {
      motion.motionStop();
      trayYReset = true;
      motion.moveSliderBeltUp(2);
    }
  }

  if(trayYReset) { // Can add more conditions to exit resetMachine function here.
    systemReady = true;
  }
}

// Function to reset the states of the machine
void fsm::resetTrayOne() {
    entry1 = false;
    exit1 = false;
}

// Function to reset the states of the machine
void fsm::resetTrayTwo() {
    entry2 = false;
}

// Function to reset the states of the machine
void fsm::resetTrayThree() {
    entry3 = false;
    exit3 = false;
    trayThreeReady = false;
}

void fsm::resetStates() {
  resetTrayOne();
  resetTrayTwo();
  resetTrayThree();
  systemMode = -1;
}

//######################### END UTILITY FUNCTIONS #########################//

//######################### START LOGIC FUNCTIONS #########################//
//Function to call during the idle mode of the machine
void fsm::idle() {
  String nextCmd = comms.nextCommand();

  // Wait for ROS to update the status of the placement.
  // Signal from rack or arm.
  if(nextCmd.length() > 0) {
    if(sensors.sensorActivated(0)) {
      entry1 = true;
      idleState = false;
      if(itemInSystem == 0){
        itemInSystem += 1;
        systemMode = 1;
        itemType = 1; // Declaring the item type deposited.
      }
    }
    
    if(sensors.sensorActivated(4)) {
      entry3 = true;
      idleState = false;
      if(itemInSystem == 0) {
        itemInSystem += 1;
        systemMode = 0;
        itemType = 1; // Declaring the item type deposited.
      }
    }
  }
  // During idle mode, if system mode is activated, reset the slider for arm.
  // systemMode != -1 means the system is busy handling work.
  if(systemMode == 0) {
    if(currTrayY != startTrayY){
      jogSliderDownUntilTouch();//sets currTrayY
    }
  }
  else if(systemMode == 1) {
    if(currTrayY == startTrayY){
      jogSliderUpUntilTouch(); //sets currTrayY
    }
  }
  delay(10);
}

void fsm::flowLogicFirst(){
  if(systemMode == 1){
    if(currTrayY == startTrayY) { // Ensure the slider is Up.
      jogSliderUpUntilTouch();//sets currTrayY
    }
    else if(!exit1) { // do until object hit the end of tray 1.
      motion.jogArmBeltLeft();
      if(sensors.sensorActivated(1)) {
        exit1 = true;
      }
    }
    else if(!entry2) {
      motion.jogArmBeltLeft();
      if(sensors.sensorActivated(2)) {
        motion.motionStop();
        entry2 = true;
        resetTrayOne(); // Reset the control flags for tray 1.
      }
    }
  }
  else{
    motion.jogArmBeltRight();
    if(sensors.sensorActivated(0)) {
      motion.motionStop();
      entry1 = false;
      if(itemInSystem == 0) {
        resetStates();
      }
    }
  }
}

void fsm::flowLogicSecond(){
  if(systemMode == 1){
    if(currTrayY != startTrayY) {
      jogSliderDownUntilTouch();//sets currTrayY to down
    }
    else {
      motion.jogArmBeltRight();
      if(sensors.sensorActivated(3)) {
        //Reset 2nd tray
        motion.motionStop();
        resetTrayTwo();
        idleState = true;
      }
    }
  }
  else {
    if(currTrayY == startTrayY) {
      jogSliderUpUntilTouch();
    }
    else if(!sensors.sensorActivated(0)) {
      motion.jogArmBeltRight();
      if(sensors.sensorActivated(1)) {
        entry1 = true;
        //Reset 2nd tray
        entry2 = false;
        // Move the item pass the last sensor.
        motion.moveArmBeltRight(RIGHT_AMT);
      }
    }
  }
}


void fsm::flowLogicThird() {
  if(systemMode == 0) {
    if(currTrayY != startTrayY) {
      jogSliderDownUntilTouch();//sets currTrayY to down
    }
    else if(!trayThreeReady) {
      motion.moveArmBeltLeft(LEFT_AMT); // move a fixed amount to give space for the next object
      if(itemInSystem == 4) {
        trayThreeReady = true;
      }
      else{
        entry3 = false;
        delay(5000);
        idleState = true;
      }
    }
    else if(!entry2) {
      motion.jogArmBeltLeft();
      if(sensors.sensorActivated(2)) { // Jog until slightly the entry sensor.
          motion.motionStop();
          entry2 = true;
          itemInSystem -= 1;
      }
    }
  }
}

void fsm::flowLogic() {
  // Code flow for when object has been detected.
  if(entry1) {
    // Serial.println("In first tray");
    flowLogicFirst();
  }
  else if(entry2){
    // Serial.println("In second tray");
    flowLogicSecond();
  }
  else if(entry3){
    // Serial.println("In third tray");
    flowLogicThird();
  }
  else{
    systemMode = -1;
  }
}

//######################THIS PART RUNS IN THE LOOP######################//
void fsm::mainLogic() {
  if(!systemReady) {
    resetMachine();
    delay(5);
  }
  else if(idleState) {
    idle();
  }
  else{
    flowLogic();
  }
}
