#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include "oled_display.h"
#include "config.h"

class WiFiHandler {
private:
    OLEDDisplay* oled;
    bool connected;
    char ssid[33];
    char password[65];

public:
    WiFiHandler(OLEDDisplay* d);
    void setCredentials(const char* s, const char* p);
    bool connect();
    void disconnect();
    bool isConnected();
    void displayStatus();
};

#endif