#include "rackManager.h"

/************************************************
Movement utility functions
 ***********************************************/
int rackManager::diffX(int start, int end) {
  return end - start;
}

int rackManager::diffY(int start, int end) {
  return end - start;
}

int rackManager::diffHighestY(int start) {
  return highest_y - start;
}

int rackManager::diffSecondY(int start) {
  return second_y - start;
}

int rackManager::diffThirdY(int start) {
  return third_y - start;
}

int rackManager::toWorkerChannelY(int start) {
  return worker.yOffset - start;
}

int rackManager::toWorkerChannelX(int start) {
  return worker.xOffset - start;
}

int rackManager::toRobotChannelY(int start) {
  return robot.yOffset - start;
}

int rackManager::toRobotChannelX(int start) {
  return robot.xOffset - start;
}

/************************************************
 # Hole management
 ***********************************************/
void rackManager::setup() {
  seesaws[0] = new Channel(0, highest_y);
  seesaws[1] = new Channel(1, third_y);
  seesaws[2] = new Channel(1, second_y);
  seesaws[3] = new Channel(1, first_y);
  stationary[0] = new Channel(0, 0);
  stationary[1] = new Channel(1, 0);
}

// Checking for the label in seesaw
bool rackManager::inSeesaw(String label) {
  for(int i = 0; i < 4; i++){
    if (seesaws[i].label == label) {
      return true;
    }
  }
  return false;
}

// Checking for the label in stationary
bool rackManager::inStationary(String label) {
  for(int i = 0; i < 2; i++){
    if (seesaws[i].label == label) {
      return true;
    }
  }
  return false;
}

// Function to get the channel for storing this label.
// nextFree prioritise storing in the stationary first,
// if label is already in stationary, place it in seesaw if there is space.
Channel rackManager::nextFree(String label) {
  if (!inStationary(label)) {
    for (int i = 0; i < 2; i++) {
      if(stationaries[i].label == "") {
        return stationaries[i];
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    if(seesaws[i].label == "") {
      return seesaws[i];
    }
  }
  return nullptr;
}


// Function to get the channel containing the label.
Channel rackManager::locateChannel(String label) {
  if(inSeeSaw(label)) {
    for(int i = 0; i < 4; i++) {
      if (seesaws[i].label == label){
        return seesaws[i];
      }
    }
  }
  if(inStationary(label)){
    for(int i = 0; i < 2; i++) {
      return stationaries[i]
    }
  }
  return nullptr;
}