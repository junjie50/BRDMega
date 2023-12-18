#ifndef RACK_MANAGER_H
#define RACK_MANAGER_H
#include "hole.h"
class rackManager {
  public:
    int diffX(int start, int end);
    int diffY(int start, int end);
    int diffHighestY(int start);
    int diffSecondY(int start);
    int diffThirdY(int start);

    int toWorkerHoleX(int start);
    int toWorkerHoleY(int start);

  private:
    int highest_y = 135;
    int second_y = 97;
    int third_y = 60;

    //hole hardcode
    Hole robot = Hole(0, 72);
    Hole worker = Hole(0, 38);
};


#endif