#include "comms.h"


// For now use serial from computer to test the system.
String commsManager::nextCommand() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\r'); // Only for entering from computer
    while(Serial.available()) {
      Serial.read();
    }
    return msg;
  }
  else {
    return "";
  }
}

void commsManager::sendMessage(String message) {
  Serial.println(message);
}