#include "wifiManager.h"


wifiManager wifi;
void setup() {
  Serial.begin(115200); 
  wifi.setUp();

  delay(1000);
}

String nextCmd;
void loop() {
  wifi.connectWifi();
  nextCmd = wifi.nextMessage();
  if(nextCmd.length() > 0) {
    Serial.println(nextCmd);
    wifi.sendMessage("Received");
  }
  delay(10);
}