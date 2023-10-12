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
  }
}

// Jog slider up to touch sensor, set the position control variable.
void fsm::jogSliderUpUntilTouch() {
  motion.jogSliderBeltUp();
  if(sensors.sensorActivated(7)) {
    motion.motionStop();
    currTrayY = endTrayY;
  }
}

// Function to reset the states of the machine
void fsm::resetMachine() {
  if(!trayYReset) {
    motion.jogSliderBeltDown();
    if(sensors.sensorActivated(6)) {
      motion.motionStop();
      trayYReset = true;
      motion.moveSliderBeltUp(10);
    }
  }
  if(trayYReset) { // Can add more conditions to exit resetMachine function
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
  if(sensors.sensorActivated(0)) {
    entry1 = true;
    itemInSystem += 1;
    systemMode = 1;
  }
  
  if(sensors.sensorActivated(4)) {
    // This logic
    entry3 = true;
    itemInSystem += 1;
    systemMode = 0;
  }

  // During idle mode, if system mode is activated, reset the slider for arm.
  // systemMode != 1 means the system is busy handling work.
  if(systemMode == 1) {
    if(currTrayY != startTrayY){
      jogSliderDownUntilTouch();//sets currTrayY
    }
  }
  else if(systemMode == 0) {
    if(currTrayY == startTrayY){
      jogSliderUpUntilTouch(); //sets currTrayY
    }
  }
  delay(10);
}

void fsm::flowLogicFirst(){
  if(systemMode == 1){
    if(currTrayY != startTrayY) { // Ensure the slider is down.
      jogSliderDownUntilTouch();//sets currTrayY
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
    if(currTrayY == startTrayY) {
      jogSliderUpUntilTouch();//sets currTrayY
    }
    else {
      motion.jogArmBeltRight();
      if(sensors.sensorActivated(3)) {
        //Reset 2nd tray
        resetTrayTwo();
      }
    }
  }
  else {
    if(currTrayY != startTrayY) {
      jogSliderDownUntilTouch();
    }
    else {
       motion.jogArmBeltRight();
       if(sensors.sensorActivated(1)) {
        entry1 = true;
        //Reset 2nd tray
        entry2 = false;
      }
    }
  }
}


void fsm::flowLogicThird() {
  if(systemMode == 0) {
    if(!trayThreeReady) {
      motion.moveArmBeltLeft(LEFT_AMT); // move a fixed amount to give space for the next object
      if(itemInSystem==4) {
        trayThreeReady = true;
      }
      else{
        entry3 = false;
      }
    }
    else if(!entry2){
      motion.jogArmBeltLeft();
      if(sensors.sensorActivated(2)) { // Jog until slightly the entry sensor.
          entry2 = true;
          itemInSystem -= 1;
      }
    }
  }
}

void fsm::flowLogic() {
  // Code flow for when object has been detected.
  if(entry1) {
    Serial.println("In first tray");
    flowLogicFirst();
  }
  else if(entry2){
    Serial.println("In second tray");
    flowLogicSecond();
  }
  else if(entry3){
    Serial.println("In third tray");
    flowLogicThird();
  }
  else{
    idleState = true;
  }
}


//######################THIS PART RUNS IN THE LOOP######################//
void fsm::mainLogic() {
  if(!systemReady) {
    resetMachine();
  }
  else if(idleState) {
    idle();
  }
  else{
    flowLogic();
  }
}
