#ifndef RACK_MANAGER_H
#define RACK_MANAGER_H
#include "channel.h"

class rackManager {

  public:
    int diffX(int start, int end);
    int diffY(int start, int end);
    int diffHighestY(int start);
    int diffFirstY(int start);
    int diffSecondY(int start);
    int diffThirdY(int start);

    int toWorkerChannelX(int start);
    int toWorkerChannelY(int start);
    int toRobotChannelX(int start);
    int toRobotChannelY(int start);

    void setup();
    bool inSeesaw(String label); 
    bool inStationary(String label);

    Channel* nextFree(String label);
    Channel* locateChannel(String label);

    // buffer channel hardcode
    Channel robot = Channel(0, 72);
    Channel worker = Channel(0, 38);
    Channel seesaws[16];
    Channel stationaries[6];

    int highest_y = 145;
    int third_y = 135;
    int second_y = 97;
    int first_y = 60;
};
#endif