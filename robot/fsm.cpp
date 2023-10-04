#include "Arduino.h"
#include "fsm.h"

void fsm::setUp() {
  sensors.sensorSetUp();
}

// Function to reset the states of the machine
void fsm::resetMachine() {
  if(!trayYReset) {
    motion.jogSliderBeltDown();
    if(sensors.sensorActivated(6)) {
      motion.motionStop();
      trayYReset = true;
    }
  }
  if(trayYReset) { // Can add more conditions to exit resetMachine function
    systemReady = true;
  }
}

//Function to call during the idle mode of the machine
void fsm::idle() {
  if(sensors.sensorActivated(0)) {
    entry1 = true;
    exit1 = false;
    itemInSystem += 1;
    systemMode = 1;
  }
  if(sensors.sensorActivated(5)) {
    // This logic
    entry3 = true;
    exit3 = false;
    itemInSystem += 1;
    systemMode = 0;
  }

  // During idle mode, if systemmode is activated, reset the slider for arm.
  if(systemMode == 1) {
    if(currTrayY != startTrayY){
      motion.jogSliderBeltDown();
      if(sensors.sensorActivated(6)) {
        motion.motionStop();
        currTrayY = startTrayY;
      }
    }
  }
  else if(systemMode == 0) {
    if(currTrayY == startTrayY){
      motion.jogSliderBeltUp();
      if(sensors.sensorActivated(6)) {
        motion.motionStop();
        currTrayY = startTrayY;
      }
    }
  }
  delay(10);
}

void fsm::flowLogicFirst(){
  if(systemMode == 1){
    if(currTrayY!= startTrayY) { // Ensure the slider is down.
      motion.jogSliderBeltDown();
      if(sensor.sensorActivated(6)) {
        currTrayY = startTrayY;
      }
    }
    else if(!exit1) { // do until object exits the tray 1.
      motion.jogArmBeltLeft();
      if(sensor.sensorActivated(1)) {
        exit1 = true;
      }
    }
    else if(!entry2) {
      motion.jogArmBeltLeft();
      if(sensor.sensorActivated(2)) {
        entry2 = true;
        entry1 = false;
        exit1 = true
      }
    }
  }
  else{
    
  }
}
void fsm::flowLogicSecond(){
  if(systemMode == 1){
    if(!pause2) {
      motion.jogArmBeltLeft();
      if(sensor.sensorActivated(3)) {
        pause2 = true;
      }
    }
  
    if(currTrayY == startTrayY) {
      motion.jogSliderBeltUp();
      if(sensor.sensorActivated(7)) {
        motion.motionStop();
        currTrayY = 1;
      }
    }
    
    if(pause2 && currTrayY != startTrayY) {
      if(!exit2) {
        motion.jogArmBeltRight();
        if(sensor.sensorActivated(2)) {
          exit2 = true;
        }
      }
      else if(!entry3) {
         motion.jogArmBeltRight();
         if(sensor.sensorActivated(4)) {
          entry3 = true;
          //Reset 2nd tray
          entry2 = false;
          exit2 = true;
          pause2 = false;
        }
      }
    }
  }
  else{
    if(!pause2) {
      motion.jogArmBeltLeft();
      if(sensor.sensorActivated(3)) {
        pause2 = true;
      }
    }
  
    if(currTrayY != startTrayY) {
      motion.jogSliderBeltDown();
      if(sensor.sensorActivated(7)) {
        motion.motionStop();
        currTrayY = startTrayY;
      }
    }

    if(pause2 && currTrayY == startTrayY) {
      if(!exit2) {
        motion.jogArmBeltRight();
        if(sensor.sensorActivated(2)) {
          exit2 = true;
        }
      }
      else if(!entry1) {
         motion.jogArmBeltRight();
         if(sensor.sensorActivated(1)) {
          entry1 = true;
          //Reset 2nd tray
          entry2 = false;
          exit2 = true;
          pause2 = false;
        }
      }
    }
    
  }
}
void fsm::flowLogicThird(){
  if(systemMode == 1){
    motion.jogArmBeltRight();
    if(!sensor.sensorActivated(4)) { // Jog until slightly pass the entry sensor.
      entry3 = false;
    }
  }
  else{
    if(!trayThreeReady) {
      motion.jogArmBeltLeft();
      if(!sensor.sensorActivated(5)) { // Jog until slightly pass the entry sensor.
          entry3 = false;
          if(itemInSystem==4) {
            trayThreeReady = true;
          }
      }
    }
    else if(!exit3) {
      motion.jogArmBeltLeft();
      if(sensor.sensorActivated(4)) { // Jog until slightly pass the exit sensor.
          exit3 = true;
      }
    }
    else if(!entry2){
      motion.jogArmBeltLeft();
      if(sensor.sensorActivated(2)) { // Jog until slightly the entry sensor.
          entry2 = true;
      }
    }
  }
}

void fsm::flowLogic() {
  // Code flow for when object has been detected.
  if(entry1) {
    Serial.println("In first tray");
    armLogicFirst();
  }
  else if(entry2){
    Serial.println("In second tray");
    armLogicSecond();
  }
  else if(entry3){
    Serial.println("In third tray");
    armLogicThird();
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
