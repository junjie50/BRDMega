#include "Arduino.h"

#define HIGHEST_Y = 110;

#ifndef holeClass_h
#define holeClass_h
class Hole {
  public:
    int xOffset;
    int yOffset;
    String label;

    Hole(int _xoff, int _yoff);

  private:
    int numberOfItems;
};
#endif