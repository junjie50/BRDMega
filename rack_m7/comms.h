/************************************************
 This is the class used to manage the communication
 with the ROS system.
 ***********************************************/
#ifndef COMMNS_MANAGER_H
#define COMMNS_MANAGER_H
#include "Arduino.h"
#include "wifiManager.h"
class commsManager {
  public:
    void clearSerial1();
    void setup();
    String nextCommand();
    void sendMessage(String message);
  private:
  wifiManager wifi;
    
};
#endif