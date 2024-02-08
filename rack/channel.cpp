#include "Arduino.h"
#include "channel.h"

Channel::Channel(){}

// hole initialize.
Channel::Channel(int _xoff, int _yoff) {
  xOffset = _xoff;
  yOffset = _yoff;
  label = "";
  numberOfItems = 0;
}

void Channel::removeOneItem() {
  numberOfItems -= 1;
}

void Channel::addOneItem() {
  numberOfItems += 1;
}

bool Channel::empty() {
  return numberOfItems == 1;
}