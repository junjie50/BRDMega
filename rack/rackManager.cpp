#include "rackManager.h"

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

int rackManager::toWorkerHoleY(int start) {
  return worker.yOffset - start;
}

int rackManager::toWorkerHoleX(int start) {
  return worker.xOffset - start;
}