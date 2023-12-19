#ifndef COMMNS_MANAGER_H
#define COMMNS_MANAGER_H
#include "Arduino.h"
class commsManager {
  public:
    String nextCommand();
    void sendMessage(String message);
  private:

};


#endif