#include "comms.h"

void commsManager::clearSerial1(){
  while(Serial1.available()) {
    Serial1.readString();
  }
}
void commsManager::setup() {
  wifi.setUp();
}

// For now use serial from computer to test the system.
String commsManager::nextCommand() {
  wifi.connectWifi();
  return wifi.nextMessage();
}

void commsManager::sendMessage(String message) {
  wifi.connectWifi();
  wifi.sendMessageMag(message);
}