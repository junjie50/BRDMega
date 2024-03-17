#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Arduino.h"
#include "Ethernet.h"

#ifndef WIFICOMM_H
#define WIFICOMM_H

class wifiManager {

  public:
    void setUp();
    void connectWifi();
    void printWifiData();
    void printCurrentNet();
    void printMacAddress(byte mac[]);

    // UserFunctions
    String nextMessage();
    void sendMessage(char message[]);
    void sendMessage(String message);
    void sendMessageMag(String message);
    

  private:
    int status = WL_IDLE_STATUS; // the WiFi radio's status
    char packetBuffer[255];
    byte serialBuffer[255];
    char reply[6] = "hello";

    // Set up UDP port to listen on
    WiFiUDP udp;
    IPAddress selfIP = IPAddress(169,254,78,214);
    unsigned int localPort = 4210;

    IPAddress targetIP = IPAddress(169,254,78,215);
    unsigned int targetPort = 4210;
};
#endif