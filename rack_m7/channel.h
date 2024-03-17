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
    String items[4];

    
    Channel();
    Channel(int _xoff, int _yoff);
    String pop();
    void removeOneItem();
    void addOneItem(String newLabel);
    bool empty();
    void reset();
    bool full();
};
#endif