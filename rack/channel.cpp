#include "Arduino.h"
#include "hole.h"

// hole initialize.
Channel::Channel(int _xoff, int _yoff) {
  xOffset = _xoff;
  yOffset = _yoff;
  label = "";
  numberOfItems = 0;
}