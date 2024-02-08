#ifndef COMMNS_MANAGER_H
#define COMMNS_MANAGER_H
#include "Arduino.h"
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
class commsManager {
  public:
    commsManager();
    void update();
    String nextCommand();
    void sendResponse(const char* message);
    void sendConeNumber(int count);

    void callback( const std_msgs::String& msg){
      dataIn = true;
      command = msg.data;
    }

  private:
    ros::NodeHandle  nh;
    std_msgs::Int32 coneNum_msg;
    std_msgs::String response_msg;
    String command;
    bool dataIn;
    
    ros::Publisher pubCone;
    ros::Publisher pubRes;
    ros::Subscriber<std_msgs::String, commsManager> sub;
};
#endif