#include "oled_display.h"

OLEDDisplay::OLEDDisplay() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}

bool OLEDDisplay::begin() {
    Wire.begin();
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED FAILED");
        return false;
    }
    display.clearDisplay();
    display.display();
    Serial.println("OLED OK");
    return true;
}

void OLEDDisplay::clear() {
    display.clearDisplay();
}

void OLEDDisplay::show() {
    display.display();
}

void OLEDDisplay::drawPixel(int x, int y, uint16_t c) {
    display.drawPixel(x, y, c);
}

void OLEDDisplay::fillRect(int x, int y, int w, int h, uint16_t c) {
    display.fillRect(x, y, w, h, c);
}

void OLEDDisplay::drawRect(int x, int y, int w, int h, uint16_t c) {
    display.drawRect(x, y, w, h, c);
}

void OLEDDisplay::fillCircle(int x, int y, int r, uint16_t c) {
    display.fillCircle(x, y, r, c);
}

void OLEDDisplay::drawCircle(int x, int y, int r, uint16_t c) {
    display.drawCircle(x, y, r, c);
}

void OLEDDisplay::fillRoundRect(int x, int y, int w, int h, int r, uint16_t c) {
    display.fillRoundRect(x, y, w, h, r, c);
}

void OLEDDisplay::drawRoundRect(int x, int y, int w, int h, int r, uint16_t c) {
    display.drawRoundRect(x, y, w, h, r, c);
}

void OLEDDisplay::drawLine(int x0, int y0, int x1, int y1, uint16_t c) {
    display.drawLine(x0, y0, x1, y1, c);
}

void OLEDDisplay::fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t c) {
    display.fillTriangle(x0, y0, x1, y1, x2, y2, c);
}

void OLEDDisplay::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t c) {
    display.drawTriangle(x0, y0, x1, y1, x2, y2, c);
}

void OLEDDisplay::setTextSize(uint8_t s) {
    display.setTextSize(s);
}

void OLEDDisplay::setTextColor(uint16_t c) {
    display.setTextColor(c);
}

void OLEDDisplay::setTextColor(uint16_t c, uint16_t bg) {
    display.setTextColor(c, bg);
}

void OLEDDisplay::setCursor(int x, int y) {
    display.setCursor(x, y);
}

void OLEDDisplay::print(const char* t) {
    display.print(t);
}

void OLEDDisplay::print(int n) {
    display.print(n);
}

void OLEDDisplay::print(float n, int d) {
    display.print(n, d);
}

void OLEDDisplay::println(const char* t) {
    display.println(t);
}

void OLEDDisplay::drawBitmap(int x, int y, const uint8_t* bmp, int w, int h, uint16_t c) {
    display.drawBitmap(x, y, bmp, w, h, c);
}

int OLEDDisplay::width() {
    return SCREEN_WIDTH;
}

int OLEDDisplay::height() {
    return SCREEN_HEIGHT;
}