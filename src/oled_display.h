#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

class OLEDDisplay {
private:
    Adafruit_SSD1306 display;

public:
    OLEDDisplay();
    bool begin();
    void clear();
    void show();
    void drawPixel(int x, int y, uint16_t c);
    void fillRect(int x, int y, int w, int h, uint16_t c);
    void drawRect(int x, int y, int w, int h, uint16_t c);
    void fillCircle(int x, int y, int r, uint16_t c);
    void drawCircle(int x, int y, int r, uint16_t c);
    void fillRoundRect(int x, int y, int w, int h, int r, uint16_t c);
    void drawRoundRect(int x, int y, int w, int h, int r, uint16_t c);
    void drawLine(int x0, int y0, int x1, int y1, uint16_t c);
    void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t c);
    void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t c);
    void setTextSize(uint8_t s);
    void setTextColor(uint16_t c);
    void setTextColor(uint16_t c, uint16_t bg);
    void setCursor(int x, int y);
    void print(const char* t);
    void print(int n);
    void print(float n, int d = 2);
    void println(const char* t);
    void drawBitmap(int x, int y, const uint8_t* bmp, int w, int h, uint16_t c);
    int width();
    int height();
};

#endif