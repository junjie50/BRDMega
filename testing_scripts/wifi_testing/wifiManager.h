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
    void sendMessageRack(String message);
    

  private:
    int status = WL_IDLE_STATUS; // the WiFi radio's status
    char packetBuffer[255];
    byte serialBuffer[255];
    char reply[6] = "hello";

    // Set up UDP port to listen on
    WiFiUDP udp;
    unsigned int localPort = 4210;

    // Rack IP Address and port
    unsigned int targetPort = 51748;
    IPAddress targetIP = IPAddress(172,20,10,4);
};
#endif