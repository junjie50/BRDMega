#include "Arduino.h"

#define HIGHEST_Y = 110;

#ifndef channelClass_h
#define channelClass_h
class Channel {
  public:
    int xOffset;
    int yOffset;
    String label;
    int numberOfItems;

    Channel(int _xoff, int _yoff);
};
#endif