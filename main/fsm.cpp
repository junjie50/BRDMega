#include "Arduino.h"
#include "fsm.h"
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>

// ######################### COMMUNICATION SETUP ######################### //
ros::NodeHandle  nh;
String command;
bool dataIn;

void callback( const std_msgs::String& msg){
  dataIn = true;
  command = msg.data;
}

std_msgs::Int32 coneNum_msg;
std_msgs::String response_msg;
std_msgs::String feedback_msg;
ros::Publisher pubCone("wp1mag_coneNum", &coneNum_msg);
ros::Publisher pubFeed("wp1mag_feedback", &feedback_msg);
ros::Publisher pubRes("wp1mag_response", &response_msg);
ros::Subscriber<std_msgs::String> sub("wp1mag_command", callback);

void commSetUp() {
  nh.initNode();
  nh.advertise(pubCone);
  nh.advertise(pubRes);
  nh.advertise(pubFeed);
  nh.subscribe(sub);

  nh.spinOnce();
}

String nextCommand() {
  nh.spinOnce();
  if(dataIn) {
    nh.loginfo("get data");
    dataIn = false;
    if (command == "PrepareCone" || command == "StoreCone"){
      return command;
    } 
    else {
      return "";
    }
  } 
  else{
    nh.loginfo("not get data");
    return "";
  }
}

void sendResponse(const char*  message) {
  response_msg.data = message;
  pubRes.publish( &response_msg );
}

void sendCone(int number) {
  coneNum_msg.data = number;
  pubCone.publish( &coneNum_msg );
}

void sendFeedback(const char*  message) {
  feedback_msg.data = message;
  pubFeed.publish( &feedback_msg );
}
//######################### COMMUNICATION END ######################### //



// ######################### UTILITY FUNCTIONS ######################### //
void fsm::stateUpdate(){
  if(nextCmd == "StoreCone") { // Received Command.
    if(!sensors.armEntryEmpty()) {
      entry1 = true;
      idleState = false;
      itemInSystem += 1;
      systemMode = 1;
      itemType = 1;
    }
  }
  else if(nextCmd == "PrepareCone") {
    entry3 = true;
    idleState = false;
    itemInSystem -= 1;
    systemMode = 0;
    itemType = 1;
  }
  else if(nextCmd == "unload") {
    unloadState = true;
    idleState = false;

    timeStart = millis() - 1500;
  }
  else if(nextCmd == "collect") {
    collectState = true;
    idleState = false;
    itemType = 1;

    timeStart = millis() - 1500;
    systemMode = 0;
  }
}

void fsm::override(){
  if(nextCmd == "Stop") {
    motion.motionStop();
    stopState = true;
  }
  else if(nextCmd == "Reset") {
    stopState = false;
  }
}

void fsm::replyUpdate() {
  if(nextCmd == "NumCone"){
    sendCone(itemInSystem);
  }
}

void fsm::setUp() {
  commSetUp();
  sensors.sensorSetUp();
}

// Jog slider down to touch sensor, set the position control variable.
// While moving change the currTray to -1 to indicate invalid position.
void fsm::jogSliderDownUntilTouch() {
  motion.jogSliderBeltDown();
  currTrayY = -1;
  if(sensors.sensorActivated(6)) {
    motion.motionStop();
    currTrayY = startTrayY;
    motion.moveSliderBeltUp(6);
    delay(1000);
  }
}

// Jog slider up to touch sensor, set the position control variable.
// While moving change the currTray to -1 to indicate invalid position.
void fsm::jogSliderUpUntilTouch() {
  motion.jogSliderBeltUp();
  currTrayY = -1;
  if(sensors.sensorActivated(7)) {
    motion.motionStop();
    motion.moveSliderBeltDown(5);
    currTrayY = endTrayY;
    delay(1000);
  }
}

// Function to reset the states of the machine during the start of the machine.
void fsm::resetMachine() {
  if(!trayYReset) {
    motion.jogSliderBeltDown();
    if(sensors.sensorActivated(6)) {
      motion.motionStop();
      trayYReset = true;
      motion.moveSliderBeltUp(6);
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

void fsm::sendFeedbackStatus(const char* msg) {
  unsigned long curr = millis();
  if(curr - timePrevFeed > 1000) {
    timePrevFeed = curr;
    sendFeedback(msg);
  }
}

//######################### END UTILITY FUNCTIONS #########################//

//######################### START LOGIC FUNCTIONS #########################//
// Function to call during the idle mode of the machine.
// Robot should always be entering a certain systemStatus
void fsm::idle() {
  // During idle mode, if system mode is activated, reset the slider for arm.
  // systemMode != -1 means the system is busy handling work.
  if(systemMode == 0) {
    if(currTrayY != startTrayY){
      jogSliderDownUntilTouch();//sets currTrayY
    }
  }
  else if(systemMode == 1) {
    if(currTrayY != endTrayY){
      jogSliderUpUntilTouch(); //sets currTrayY
    }
  }

  // Delay to slow down the response of the machine during idle mode.
  delay(10);
}

void fsm::flowLogicFirst(){
  if(systemMode == 1) {
    if(currTrayY != endTrayY) { // Ensure the slider is Up.
      jogSliderUpUntilTouch(); //sets currTrayY
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
      idleState = true;

      sendResponse("done");
      if(itemInSystem == 0) {
        systemMode = -1;
      }
    }
  }
}

void fsm::flowLogicSecond() {
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

        // Move the item pass the last sensor.
        motion.moveArmBeltRight(PASS_SENSOR);

        // Update that we are done.
        sendResponse("done");

        // reset the systemMode of the robot when the system is full.
        if(itemInSystem == 4) {
          systemMode = -1;
        }
      }
    }
  }
  else {
    if(currTrayY != endTrayY) {
      jogSliderUpUntilTouch();
    }
    else if(!sensors.sensorActivated(0)) { // Only move when there is free space
      motion.jogArmBeltRight();
      if(sensors.sensorActivated(1)) {
        entry1 = true;
        // Reset 2nd tray
        entry2 = false;
      }
    }
  }
}

void fsm::flowLogicThird() {
  if(systemMode == 0) {
    if(currTrayY != startTrayY) {
      jogSliderDownUntilTouch(); //sets currTrayY to down
    }
    else if(!entry2) {
      motion.jogArmBeltLeft();
      if(sensors.sensorActivated(2)) { // Jog until slightly the entry sensor.
          motion.motionStop();
          entry2 = true;
          entry3 = false;
      }
    }
  }
}

void fsm::flowLogic() {
  // Code flow for when object has been detected.
  if(entry1) {
    sendFeedbackStatus("In arm belt");
    flowLogicFirst();
  }
  else if(entry2) {
    sendFeedbackStatus("In buffer belt");
    flowLogicSecond();
  }
  else if(entry3) {
    sendFeedbackStatus("In rack belt");
    flowLogicThird();
  }
}

// Function is called when it is safe to unload and given the command from the robot.
void fsm::unload() {
  if(currTrayY != startTrayY) {
    jogSliderDownUntilTouch();//sets currTrayY to down
  }
  else {
    motion.jogArmBeltRight();
    if(millis() - timeStart > 1500 && !sensors.rackEntryEmpty()) { 
      // allow the cone to go pass the current sensor.
      itemInSystem -= 1;
      timeStart = millis();
      if(itemInSystem == 0) {
        // finished depositing the cones.
        unloadState = false;
        idleState = true;
        motion.motionStop();
        // Unloading the rest of the cones.
        motion.moveArmBeltRight(UNLOAD_AMT);
        // TODO: Broadcast to the robot that we are done depositing.
      }
    }
  }
}

void fsm::collect() {
  if(currTrayY != startTrayY) {
    jogSliderDownUntilTouch();//sets currTrayY to down
  }
  else {
    motion.jogArmBeltLeft();
    if(millis() - timeStart > 1500 && !sensors.rackEntryEmpty()) {
      // allow the cone to go pass the current sensor.
      itemInSystem += 1;
      timeStart = millis(); // Reset the time start.
      if(itemInSystem == 4) {
      // finished depositing the cones
        collectState = false;
        idleState = true;
        motion.motionStop();
      }
    }
  }
}

//######################THIS PART RUNS IN THE LOOP######################//
void fsm::mainLogic() {
  //Update command at the start of the loop.
  nextCmd = nextCommand();
  //Overrides the existing states via stopState var.
  override();
  //respond to certain commands.
  replyUpdate();

  if(stopState){
    //Critical, received command to stop everything.s
    delay(5);
  }
  else if(!systemReady) {
    resetMachine();
    delay(5);
  }
  else if(idleState) {
    idle();
  }
  else{
    //Update state according to the next command.
    stateUpdate();
    flowLogic();
  }
}