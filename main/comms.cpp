#include "comms.h"
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>

commsManager::commsManager() : sub("wp1mag_command", &commsManager::callback, this)
,pubCone("wp1mag_coneNum", &coneNum_msg), pubRes("wp1mag_response", &response_msg) {
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(pubCone);
  nh.advertise(pubRes);
  nh.subscribe(sub);
}

// For now use serial from computer to test the system.
String commsManager::nextCommand() {
  update(); // Get the update from the callback stack.
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

void commsManager::sendResponse(const char* message) {
  response_msg.data = message;
  pubRes.publish( &response_msg );
}

void commsManager::sendConeNumber(int count) {
  coneNum_msg.data = count;
  pubRes.publish( &coneNum_msg );
}

void commsManager::update() {
  nh.spinOnce();
}