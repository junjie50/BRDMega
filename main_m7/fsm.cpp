#include "Arduino.h"
#include "fsm.h"
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>

// ######################### COMMUNICATION SETUP ######################### //
ros::NodeHandle  nh;
String command;
String commandRack;
String coneList;
bool dataIn;
bool dataInRack;
bool dataInConeList;

void callback( const std_msgs::String& msg){
  dataIn = true;
  command = msg.data;
}

void callbackRack( const std_msgs::String& msg){
  dataInRack = true;
  commandRack = msg.data;
}

void callbackConeList( const std_msgs::String& msg){
  dataInConeList = true;
  coneList = msg.data;
}


std_msgs::Int32 coneNum_msg;
std_msgs::String response_msg;
std_msgs::String response_msg_rack;
std_msgs::String feedback_msg;
std_msgs::String conelist_msg;
ros::Publisher pubCone("wp1mag_coneNum", &coneNum_msg);
ros::Publisher pubFeed("wp1mag_feedback", &feedback_msg);
ros::Publisher pubRes("wp1mag_response", &response_msg);
ros::Publisher pubResRack("wp1rack_response", &response_msg_rack);
ros::Subscriber<std_msgs::String> subConeList("wp1_conelist", callbackConeList);
ros::Subscriber<std_msgs::String> sub("wp1mag_command", callback);
ros::Subscriber<std_msgs::String> subRack("wp1rack_command", callbackRack);

void commSetUp() {
  nh.initNode();
  nh.advertise(pubCone);
  nh.advertise(pubRes);
  nh.advertise(pubResRack);
  nh.advertise(pubFeed);
  nh.subscribe(sub);
  nh.subscribe(subRack);

  nh.spinOnce();
}

void updateRosSerial() {
  nh.spinOnce();
}

String nextCommand() {
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
}

String nextCommandRack() {
  if(dataInRack) {
    nh.loginfo("get data rack");
    dataInRack = false;
    return commandRack;
  }
}

String nextConeList() {
  if(dataInConeList) {
    nh.loginfo("get data cone list");
    dataInConeList = false;
    return coneList;
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

void sendRackResponse(const char*  message) {
  response_msg_rack.data = message;
  pubResRack.publish(&response_msg_rack);
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
  else if(nextCmd == "EmptyMag") {
    if(itemInSystem > 0){
      unloadState = true;
      idleState = false;
      unloadStartTime = millis()-TIME_TO_PASS;
    }
    else{
      sendResponse("done");
    }
  }
  else if(nextCmd == "GetConeMag") {
    if(fsmConeListCount > 0) {
      collectState = true;
      idleState = false;
      systemMode = 0;
      collectStartTime = millis()-TIME_TO_PASS;
    }
    else {
      sendResponse("done");
    }
  }
}

void fsm::override() {
  if(nextCmd == "Stop") {
    motion.motionStop();
    stopState = true;
  }
  else if(nextCmd == "Continue") {
    stopState = false;
  }
  else if(nextCmd == "Reset") {
    restart();
  }
}

// replying to the updates of the cone.
// send feedback to the main controller.
void fsm::replyUpdate() {
  if(nextCmd == "NumCone"){
    sendCone(itemInSystem);
  }
  
  if(!idleState) {
    if(entry1) {
      sendFeedbackStatus("In arm belt");
    }
    else if(entry2) {
      sendFeedbackStatus("In moving belt");
    }
    else if(entry3) {
      sendFeedbackStatus("In rack belt");
    }
  }
}

void fsm::setUp() {
  commSetUp();
  sensors.sensorSetUp();
  motion.motionSetup();
  wifi.setUp();
}

// Jog slider down to touch sensor, set the position control variable.
// While moving change the currTray to -1 to indicate invalid position.
void fsm::jogSliderDownUntilTouch() {
  if(sensors.sensorActivated(6)) {
    motion.motionStop();
    currTrayY = startTrayY;
    motion.moveSliderBeltUp(15);
    delay(1000);
  }
  else {
    motion.jogSliderBeltDown();
    currTrayY = -1;
  }
}

// Jog slider up to touch sensor, set the position control variable.
// While moving change the currTray to -1 to indicate invalid position.
void fsm::jogSliderUpUntilTouch() {
  if(sensors.sensorActivated(7)) {
    motion.motionStop();
    motion.moveSliderBeltDown(5);
    currTrayY = endTrayY;
    delay(1000);
  }
  else {
    motion.jogSliderBeltUp();
    currTrayY = -1;
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

void fsm::restart() {
    // Flags for resetting system at the starting of robot.
    // Reset the trayY slider.
    trayYReset = false;

    // Flags used to indicate that the system is ready for main logic loop.
    systemReady = false;

    // Prepare cone for the arm.
    prepareState = false;

    // For error handling
    errorState = false;
    

    // Logical Flags
    systemMode = -1; // 1 means taking item from arm 0 means taking item from rack.
    idleState = true;
    entry1 = false;
    exit1 = true;
    entry2 = false;
    entry3 = false;
    exit3 = true;
    itemInSystem = 0;
    itemType = 0;

    // For collection
    collectState = false;

    // For unloading to the rack
    unloadState = false;

    // For storage
    trayThreeReady = false;
    
    // Int side = 0 or 1 for y tray
    // bottomtray is y = 0 while upper tray with the arm is y = 1.
    currTrayY = 0;
    startTrayY = 0;
    endTrayY = 1;

    // For feedback time
    timePrevFeed = 0;

    // STOP MODE
    stopState = false;
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
  if(collectState) {
    collect();
  }
  else if(unloadState) {
    unload();
  }
  else if(entry1) {
    flowLogicFirst();
  }
  else if(entry2) {
    flowLogicSecond();
  }
  else if(entry3) {
    flowLogicThird();
  }
}

// Function is called when it is safe to unload and given the command from the robot.
void fsm::unload() {
  if(currTrayY != startTrayY) {
    jogSliderDownUntilTouch(); //sets currTrayY to down
  }
  else {
    motion.jogArmBeltRight();
    if(millis() - unloadStartTime > TIME_TO_PASS) {
      if(!sensors.rackEntryEmpty()) {
        unloadStartTime = millis();
        itemInSystem -= 1;
        if(itemInSystem == 0) {
          motion.motionStop();
          motion.moveArmBeltRight(UNLOAD_AMT);
          unloadState = false;
          idleState = true;
          sendResponse("done");
        }
      }
    }
  }
}

void fsm::collect() {
  if(currTrayY != startTrayY) {
    jogSliderDownUntilTouch(); //sets currTrayY to down
  }
  else {
    if(millis() - collectStartTime > TIME_TO_PASS) {
      if(!sensors.rackEntryEmpty()) {
        collectStartTime = millis();
        motion.moveArmBeltLeft(PASS_SENSOR);
        itemInSystem += 1;
        if(itemInSystem == fsmConeListCount) {
          collectState = false;
          idleState = true;
          sendResponse("done");
        }
      }
    }
  }
}

// wififorwarder forwards the packages between robot and the rack
// Peek into package to obtain information for the magazine as well.
void fsm::wifiForwarder() {
  // Ensure connection at the start of each loop.
  wifi.connectWifi(); 

  // Command from main robot to rack.
  nextCmdRack = nextCommandRack();
  nextConeListStr = nextConeList();
  if(nextCmdRack.length() > 0) {
    wifi.sendMessageRack(nextCmdRack+nextConeListStr);
  }

  if(nextConeListStr.length() > 0) {
    int start = 0;
    int next = nextConeListStr.indexOf(',');
    fsmConeListCount = 0;
    while(next != -1) {
      fsmConeList[fsmConeListCount] = nextConeListStr.substring(next + 1, next + 4);
      fsmConeListCount += 1;
      start = next + 1;
      next = nextConeListStr.indexOf(',');
    }
  }

  // Response from rack to main robot.
  String response = wifi.nextMessage();
  if(response.length() > 0){
    sendRackResponse(response.c_str());
  }
}

//######################THIS PART RUNS IN THE LOOP######################//
void fsm::mainLogic() {
  // Update the ros serial.
  updateRosSerial();

  // Forward the data to the rack if need to.
  wifiForwarder();

  //Update command.
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