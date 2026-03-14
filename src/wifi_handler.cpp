#include "wifi_handler.h"

WiFiHandler::WiFiHandler(OLEDDisplay* d) : oled(d), connected(false) {
    ssid[0] = '\0';
    password[0] = '\0';
}

void WiFiHandler::setCredentials(const char* s, const char* p) {
    strncpy(ssid, s, sizeof(ssid) - 1);
    ssid[sizeof(ssid) - 1] = '\0';
    strncpy(password, p, sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0';
}

bool WiFiHandler::connect() {
    if (strlen(ssid) == 0) {
        return false;
    }

    oled->clear();
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    oled->setCursor(0, 0);
    oled->print("Connecting WiFi");
    oled->setCursor(0, 15);
    oled->print(ssid);
    oled->show();

    WiFi.begin(ssid, password);
    int attempts = 0;
    
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        attempts++;
    }
    
    connected = (WiFi.status() == WL_CONNECTED);

    oled->clear();
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    oled->setCursor(0, 20);
    
    if (connected) {
        oled->print("WiFi Connected!");
        oled->setCursor(0, 35);
        oled->print(WiFi.localIP().toString().c_str());
    } else {
        oled->print("WiFi Failed!");
    }
    
    oled->show();
    delay(1500);
    
    return connected;
}

void WiFiHandler::disconnect() {
    WiFi.disconnect();
    connected = false;
}

bool WiFiHandler::isConnected() {
    connected = (WiFi.status() == WL_CONNECTED);
    return connected;
}

void WiFiHandler::displayStatus() {
    oled->clear();
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    
    oled->setCursor(0, 0);
    oled->print("== WIFI ==");
    oled->drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
    
    oled->setCursor(0, 16);
    
    if (isConnected()) {
        oled->print("Connected:");
        oled->setCursor(0, 26);
        oled->print(ssid);
        oled->setCursor(0, 40);
        oled->print("IP:");
        oled->print(WiFi.localIP().toString().c_str());
    } else {
        oled->print("Not connected");
    }
    
    oled->setCursor(0, 56);
    oled->print("Hold:Back");
    oled->show();
}