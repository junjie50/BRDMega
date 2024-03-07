/*Adapted from code from https://docs.arduino.cc/tutorials/giga-r1-wifi/giga-wifi/*/
#include "wifiManager.h"
#include "arduino_secrets.h"

void wifiManager::setUp() {
  connectWifi();
  udp.begin(localPort);
}

void wifiManager::connectWifi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
  }

  // Always try to check if is connected.
  if(status != WL_CONNECTED){
    Serial.println("");
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(SECRET_SSID);
    // attempt to connect to WiFi network:
    while (status != WL_CONNECTED) {
      // Connect to WPA/WPA2 network:
      status = WiFi.begin(SECRET_SSID, SECRET_PASS);
      delay(100);
    }

    // you're connected now, so print out the data:
    Serial.print("You're connected to the network");
  }
}

void wifiManager::printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void wifiManager::printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void wifiManager::printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

// Check next message from the rack.
String wifiManager::nextMessage() {
  // if there's data available, read a packet
  int packetSize = udp.parsePacket();
  if (packetSize) {
    // read the packet into packetBufffer
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';
    }
    return String(packetBuffer);
  }
  return "";
}

void wifiManager::sendMessage(char message[]){
  // send a reply, to the IP address and port that sent us the packet we received
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  udp.write(message);
  udp.endPacket();
}

void wifiManager::sendMessage(String message){
  // send a reply, to the IP address and port that sent us the packet we received
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  udp.write(message.c_str());
  udp.endPacket();
}

// Send message to the rack.
void wifiManager::sendMessageRack(String message){
  // send a reply, to the IP address and port that sent us the packet we received
  udp.beginPacket(targetIP, targetPort);
  udp.write(message.c_str());
  udp.endPacket();
}