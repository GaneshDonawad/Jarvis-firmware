#include "animations.h"

Animations::Animations(OLEDDisplay* d) : oled(d) {}

void Animations::playBootAnimation() {
    // Scan line animation
    for (int i = 0; i < SCREEN_WIDTH; i += 2) {
        oled->clear();
        oled->drawLine(i, 0, i, SCREEN_HEIGHT - 1, SSD1306_WHITE);
        oled->show();
        delay(8);
    }

    // Robot name reveal
    oled->clear();
    oled->setTextSize(2);
    oled->setTextColor(SSD1306_WHITE);
    
    const char* name = "ROBO";
    for (int i = 0; i < 4; i++) {
        oled->setCursor(25 + i * 20, 8);
        char c[2] = {name[i], '\0'};
        oled->print(c);
        oled->show();
        delay(150);
    }

    oled->setTextSize(1);
    oled->setCursor(22, 32);
    oled->print("BUDDY v2.0");
    oled->show();
    delay(400);

    // Loading bar
    for (int i = 0; i <= 100; i += 4) {
        oled->fillRect(14, 48, 100, 10, SSD1306_BLACK);
        oled->drawRect(14, 48, 100, 10, SSD1306_WHITE);
        oled->fillRect(14, 48, i, 10, SSD1306_WHITE);
        oled->show();
        delay(25);
    }
    delay(200);
}

void Animations::playWakeUpAnimation() {
    // Eyes opening
    for (int h = 2; h <= 24; h += 2) {
        oled->clear();
        int y = 25 - h / 2;
        oled->fillRoundRect(23, y, 24, h, 3, SSD1306_WHITE);
        oled->fillRoundRect(81, y, 24, h, 3, SSD1306_WHITE);
        oled->show();
        delay(35);
    }
}

void Animations::playSleepAnimation() {
    // Eyes closing
    for (int h = 24; h >= 2; h -= 2) {
        oled->clear();
        int y = 25 - h / 2;
        oled->fillRoundRect(23, y, 24, h, 3, SSD1306_WHITE);
        oled->fillRoundRect(81, y, 24, h, 3, SSD1306_WHITE);
        oled->show();
        delay(35);
    }
    
    oled->clear();
    oled->fillRect(23, 24, 24, 4, SSD1306_WHITE);
    oled->fillRect(81, 24, 24, 4, SSD1306_WHITE);
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    oled->setCursor(95, 10);
    oled->print("Z");
    oled->setCursor(103, 4);
    oled->print("z");
    oled->setCursor(110, 0);
    oled->print("z");
    oled->show();
}

void Animations::playTouchReaction() {
    // Surprised blink
    for (int i = 0; i < 3; i++) {
        oled->clear();
        oled->fillRoundRect(27, 17, 20, 16, 5, SSD1306_WHITE);
        oled->fillRoundRect(85, 17, 20, 16, 5, SSD1306_WHITE);
        oled->show();
        delay(60);
        
        oled->clear();
        oled->fillRoundRect(23, 13, 28, 28, 5, SSD1306_WHITE);
        oled->fillRoundRect(81, 13, 28, 28, 5, SSD1306_WHITE);
        oled->fillCircle(35, 25, 6, SSD1306_BLACK);
        oled->fillCircle(93, 25, 6, SSD1306_BLACK);
        oled->show();
        delay(60);
    }
}

void Animations::playTransition() {
    // Wipe left to right
    for (int i = 0; i < SCREEN_WIDTH; i += 4) {
        oled->fillRect(i, 0, 4, SCREEN_HEIGHT, SSD1306_WHITE);
        oled->show();
        delay(3);
    }
    
    // Wipe right to left
    for (int i = 0; i < SCREEN_WIDTH; i += 4) {
        oled->fillRect(i, 0, 4, SCREEN_HEIGHT, SSD1306_BLACK);
        oled->show();
        delay(3);
    }
}

void Animations::playSoundReaction() {
    // React to sound
    for (int i = 0; i < 2; i++) {
        oled->clear();
        oled->fillCircle(35, 25, 16, SSD1306_WHITE);
        oled->fillCircle(93, 25, 16, SSD1306_WHITE);
        oled->fillCircle(35, 25, 8, SSD1306_BLACK);
        oled->fillCircle(93, 25, 8, SSD1306_BLACK);
        oled->drawCircle(64, 52, 8, SSD1306_WHITE);
        oled->show();
        delay(100);
        
        oled->clear();
        oled->fillRoundRect(23, 13, 24, 24, 5, SSD1306_WHITE);
        oled->fillRoundRect(81, 13, 24, 24, 5, SSD1306_WHITE);
        oled->fillCircle(35, 25, 6, SSD1306_BLACK);
        oled->fillCircle(93, 25, 6, SSD1306_BLACK);
        oled->show();
        delay(100);
    }
}

void Animations::playDanceAnimation() {
    // Dance with bouncing motion
    for (int f = 0; f < 10; f++) {
        oled->clear();
        int b = (f % 2 == 0) ? -4 : 4;
        int ey = 25 + b;

        // Bouncing eyes
        oled->fillRoundRect(23, ey - 12, 24, 24, 5, SSD1306_WHITE);
        oled->fillRoundRect(81, ey - 12, 24, 24, 5, SSD1306_WHITE);
        oled->fillCircle(35, ey, 6, SSD1306_BLACK);
        oled->fillCircle(93, ey, 6, SSD1306_BLACK);

        // Musical notes
        int nx = 10 + (f * 14) % 110;
        oled->fillCircle(nx, 5, 2, SSD1306_WHITE);
        oled->drawLine(nx + 2, 5, nx + 2, 0, SSD1306_WHITE);
        oled->fillCircle(SCREEN_WIDTH - nx, 8, 2, SSD1306_WHITE);
        oled->drawLine(SCREEN_WIDTH - nx + 2, 8, SCREEN_WIDTH - nx + 2, 3, SSD1306_WHITE);

        // Bouncing smile
        for (int i = -15; i <= 15; i++) {
            oled->drawPixel(64 + i, 50 + (i * i) / 30 + b, SSD1306_WHITE);
        }

        oled->show();
        delay(130);
    }
}

void Animations::playEmotionTransition() {
    // Circle expanding
    for (int r = 0; r <= 70; r += 7) {
        oled->clear();
        oled->fillCircle(64, 32, r, SSD1306_WHITE);
        oled->show();
        delay(12);
    }
    
    // Circle contracting
    for (int r = 70; r >= 0; r -= 7) {
        oled->clear();
        oled->fillCircle(64, 32, r, SSD1306_WHITE);
        oled->show();
        delay(12);
    }
}