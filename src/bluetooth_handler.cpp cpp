#include "bluetooth_handler.h"

BluetoothHandler::BluetoothHandler(OLEDDisplay* d, const char* name)
    : oled(d), connected(false) {
    strncpy(deviceName, name, sizeof(deviceName) - 1);
    deviceName[sizeof(deviceName) - 1] = '\0';
}

void BluetoothHandler::begin() {
    Serial.println("Bluetooth handler initialized");
}

bool BluetoothHandler::isConnected() {
    return connected;
}

void BluetoothHandler::displayStatus() {
    oled->clear();
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    
    oled->setCursor(0, 0);
    oled->print("== BLUETOOTH ==");
    oled->drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
    
    oled->setCursor(0, 16);
    oled->print("Name:");
    oled->setCursor(0, 26);
    oled->print(deviceName);
    
    oled->setCursor(0, 40);
    oled->print(connected ? "CONNECTED" : "WAITING...");
    
    oled->setCursor(0, 56);
    oled->print("Hold:Back");
    oled->show();
}