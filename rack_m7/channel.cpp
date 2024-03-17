#include "Arduino.h"
#include "channel.h"

Channel::Channel(){}

// hole initialize.
Channel::Channel(int _xoff, int _yoff) {
  xOffset = _xoff;
  yOffset = _yoff;
  numberOfItems = 0;
}

void Channel::removeOneItem() {
  if(!empty()){
    numberOfItems -= 1;
  }
}

void Channel::addOneItem(String newLabel) {
  items[numberOfItems] = newLabel;
  numberOfItems += 1;
}

bool Channel::empty() {
  return numberOfItems == 0;
}

bool Channel::full() {
  return numberOfItems == 4;
}

void Channel::reset() {
    String label = "";
    String items[4];
}

String Channel::pop(){
  if(!empty()) {
    numberOfItems -= 1;
    if(numberOfItems == 0) {
      label = "";
    }
    return items[numberOfItems];
  }
}