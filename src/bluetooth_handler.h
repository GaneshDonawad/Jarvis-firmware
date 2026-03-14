#ifndef BLUETOOTH_HANDLER_H
#define BLUETOOTH_HANDLER_H

#include <Arduino.h>
#include "oled_display.h"
#include "config.h"

class BluetoothHandler {
private:
    OLEDDisplay* oled;
    bool connected;
    char deviceName[20];

public:
    BluetoothHandler(OLEDDisplay* d, const char* name = "RoboBuddy");
    void begin();
    bool isConnected();
    void displayStatus();
};

#endif