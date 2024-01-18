#ifndef RACK_MANAGER_H
#define RACK_MANAGER_H

#include "channel.h"
class rackManager {
  public:
    int diffX(int start, int end);
    int diffY(int start, int end);
    int diffHighestY(int start);
    int diffSecondY(int start);
    int diffThirdY(int start);

    int toWorkerChannelX(int start);
    int toWorkerChannelY(int start);
    int toRobotChannelX(int start);
    int toRobotChannelY(int start);

    void setup();
    bool inSeeSaw(String label); 
    bool inStationary(String label); 

    Channel nextFree(String label);
    Channel locateChannel(String label);

  private:
    int highest_y = 135;
    int first_y = 135;
    int second_y = 97;
    int third_y = 60;

    // buffer channel hardcode
    Channel robot = Channel(0, 72);
    Channel worker = Channel(0, 38);
    Channel seesaws[4];
    Channel stationaries[2];
};
#endif