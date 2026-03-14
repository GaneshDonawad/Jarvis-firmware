#include "robot_face.h"

RobotFace::RobotFace(OLEDDisplay* d) : oled(d) {
    currentEmotion = EMOTION_HAPPY;
    isBlinking = false;
    lastBlinkTime = 0;
    blinkStartTime = 0;
    eyeOffsetX = 0;
    eyeOffsetY = 0;
    targetOffsetX = 0;
    targetOffsetY = 0;
    lastLookTime = 0;
    leftEyeX = 35;
    rightEyeX = 93;
    eyeY = 25;
    eyeW = 24;
    eyeH = 24;
    pupilR = 6;
}

void RobotFace::drawSmile(int cx, int cy, int w) {
    for (int i = -w; i <= w; i++) {
        int y = (i * i) / (w * 2);
        oled->drawPixel(cx + i, cy + y, SSD1306_WHITE);
    }
}

void RobotFace::drawFrown(int cx, int cy, int w) {
    for (int i = -w; i <= w; i++) {
        int y = -(i * i) / (w * 2) + 4;
        oled->drawPixel(cx + i, cy + y, SSD1306_WHITE);
    }
}

void RobotFace::drawHeart(int cx, int cy, int s) {
    int h = s / 2;
    oled->fillCircle(cx - h / 2, cy - h / 3, h / 2, SSD1306_WHITE);
    oled->fillCircle(cx + h / 2, cy - h / 3, h / 2, SSD1306_WHITE);
    oled->fillTriangle(cx - s / 2 - 1, cy, cx + s / 2 + 1, cy, cx, cy + h + 2, SSD1306_WHITE);
}

void RobotFace::drawStar(int cx, int cy, int s) {
    oled->drawLine(cx, cy - s, cx - 3, cy + 2, SSD1306_WHITE);
    oled->drawLine(cx - 3, cy + 2, cx - s, cy - 1, SSD1306_WHITE);
    oled->drawLine(cx - s, cy - 1, cx - 1, cy + s / 2, SSD1306_WHITE);
    oled->drawLine(cx - 1, cy + s / 2, cx + 1, cy + s / 2, SSD1306_WHITE);
    oled->drawLine(cx + 1, cy + s / 2, cx + s, cy - 1, SSD1306_WHITE);
    oled->drawLine(cx + s, cy - 1, cx + 3, cy + 2, SSD1306_WHITE);
    oled->drawLine(cx + 3, cy + 2, cx, cy - s, SSD1306_WHITE);
}

void RobotFace::drawBlink(int lx, int ly, int rx, int ry) {
    oled->fillRect(lx - eyeW / 2, ly, eyeW, 3, SSD1306_WHITE);
    oled->fillRect(rx - eyeW / 2, ry, eyeW, 3, SSD1306_WHITE);
}

void RobotFace::drawEyePair(int lx, int ly, int rx, int ry, int r) {
    oled->fillRoundRect(lx - eyeW / 2, ly - eyeH / 2, eyeW, eyeH, r, SSD1306_WHITE);
    oled->fillRoundRect(rx - eyeW / 2, ry - eyeH / 2, eyeW, eyeH, r, SSD1306_WHITE);
    int px = (int)(eyeOffsetX * 0.3);
    int py = (int)(eyeOffsetY * 0.3);
    oled->fillCircle(lx + px, ly + py, pupilR, SSD1306_BLACK);
    oled->fillCircle(rx + px, ry + py, pupilR, SSD1306_BLACK);
}

void RobotFace::drawHappyEyes(int lx, int ly, int rx, int ry) {
    if (isBlinking) {
        drawBlink(lx, ly, rx, ry);
    } else {
        drawEyePair(lx, ly, rx, ry);
    }
    drawSmile(64, 50, 20);
}

void RobotFace::drawSadEyes(int lx, int ly, int rx, int ry) {
    if (isBlinking) {
        drawBlink(lx, ly, rx, ry);
    } else {
        drawEyePair(lx, ly, rx, ry);
        oled->drawLine(lx - 12, ly - 16, lx + 12, ly - 20, SSD1306_WHITE);
        oled->drawLine(rx - 12, ry - 20, rx + 12, ry - 16, SSD1306_WHITE);
    }
    drawFrown(64, 52, 15);
}

void RobotFace::drawAngryEyes(int lx, int ly, int rx, int ry) {
    drawEyePair(lx, ly, rx, ry, 3);
    oled->drawLine(lx - 14, ly - 14, lx + 14, ly - 20, SSD1306_WHITE);
    oled->drawLine(lx - 14, ly - 13, lx + 14, ly - 19, SSD1306_WHITE);
    oled->drawLine(rx - 14, ry - 20, rx + 14, ry - 14, SSD1306_WHITE);
    oled->drawLine(rx - 14, ry - 19, rx + 14, ry - 13, SSD1306_WHITE);
    oled->fillRect(50, 52, 28, 4, SSD1306_WHITE);
}

void RobotFace::drawSurprisedEyes(int lx, int ly, int rx, int ry) {
    oled->fillCircle(lx, ly, 14, SSD1306_WHITE);
    oled->fillCircle(rx, ry, 14, SSD1306_WHITE);
    oled->fillCircle(lx, ly, 7, SSD1306_BLACK);
    oled->fillCircle(rx, ry, 7, SSD1306_BLACK);
    oled->fillCircle(lx, ly, 3, SSD1306_WHITE);
    oled->fillCircle(rx, ry, 3, SSD1306_WHITE);
    oled->drawCircle(64, 52, 6, SSD1306_WHITE);
}

void RobotFace::drawSleepyEyes(int lx, int ly, int rx, int ry) {
    oled->fillRect(lx - eyeW / 2, ly - 2, eyeW, 6, SSD1306_WHITE);
    oled->fillRect(rx - eyeW / 2, ry - 2, eyeW, 6, SSD1306_WHITE);
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    oled->setCursor(100, 5);
    oled->print("Z");
    oled->setCursor(108, 0);
    oled->print("z");
}

void RobotFace::drawLoveEyes(int lx, int ly, int rx, int ry) {
    drawHeart(lx, ly, 10);
    drawHeart(rx, ry, 10);
    drawSmile(64, 50, 15);
}

void RobotFace::drawCoolEyes(int lx, int ly, int rx, int ry) {
    oled->fillRoundRect(lx - 16, ly - 8, 32, 16, 3, SSD1306_WHITE);
    oled->fillRoundRect(rx - 16, ry - 8, 32, 16, 3, SSD1306_WHITE);
    oled->fillRoundRect(lx - 13, ly - 5, 26, 10, 2, SSD1306_BLACK);
    oled->fillRoundRect(rx - 13, ry - 5, 26, 10, 2, SSD1306_BLACK);
    oled->drawLine(lx + 16, ly, rx - 16, ry, SSD1306_WHITE);
    oled->drawLine(52, 52, 76, 52, SSD1306_WHITE);
    oled->drawLine(76, 52, 72, 48, SSD1306_WHITE);
}

void RobotFace::drawConfusedEyes(int lx, int ly, int rx, int ry) {
    oled->fillRoundRect(lx - 14, ly - 14, 28, 28, 5, SSD1306_WHITE);
    oled->fillCircle(lx, ly, pupilR, SSD1306_BLACK);
    oled->fillRoundRect(rx - 8, ry - 8, 16, 16, 3, SSD1306_WHITE);
    oled->fillCircle(rx, ry, 3, SSD1306_BLACK);
    oled->drawLine(rx - 10, ry - 14, rx + 10, ry - 18, SSD1306_WHITE);
    oled->drawLine(48, 52, 56, 48, SSD1306_WHITE);
    oled->drawLine(56, 48, 64, 54, SSD1306_WHITE);
    oled->drawLine(64, 54, 72, 48, SSD1306_WHITE);
    oled->drawLine(72, 48, 80, 52, SSD1306_WHITE);
}

void RobotFace::drawExcitedEyes(int lx, int ly, int rx, int ry) {
    drawStar(lx, ly, 12);
    drawStar(rx, ry, 12);
    oled->drawLine(44, 48, 54, 55, SSD1306_WHITE);
    oled->drawLine(54, 55, 74, 55, SSD1306_WHITE);
    oled->drawLine(74, 55, 84, 48, SSD1306_WHITE);
}

void RobotFace::drawShyEyes(int lx, int ly, int rx, int ry) {
    oled->fillRoundRect(lx - eyeW / 2, ly - 4, eyeW, 12, 3, SSD1306_WHITE);
    oled->fillRoundRect(rx - eyeW / 2, ry - 4, eyeW, 12, 3, SSD1306_WHITE);
    oled->fillCircle(lx - 4, ly + 1, 3, SSD1306_BLACK);
    oled->fillCircle(rx - 4, ry + 1, 3, SSD1306_BLACK);
    oled->drawCircle(20, 45, 5, SSD1306_WHITE);
    oled->drawCircle(108, 45, 5, SSD1306_WHITE);
    drawSmile(64, 52, 8);
}

void RobotFace::drawEyes() {
    int lx = leftEyeX + (int)eyeOffsetX;
    int ly = eyeY + (int)eyeOffsetY;
    int rx = rightEyeX + (int)eyeOffsetX;
    int ry = eyeY + (int)eyeOffsetY;

    switch (currentEmotion) {
        case EMOTION_HAPPY:
            drawHappyEyes(lx, ly, rx, ry);
            break;
        case EMOTION_SAD:
            drawSadEyes(lx, ly, rx, ry);
            break;
        case EMOTION_ANGRY:
            drawAngryEyes(lx, ly, rx, ry);
            break;
        case EMOTION_SURPRISED:
            drawSurprisedEyes(lx, ly, rx, ry);
            break;
        case EMOTION_SLEEPY:
            drawSleepyEyes(lx, ly, rx, ry);
            break;
        case EMOTION_LOVE:
            drawLoveEyes(lx, ly, rx, ry);
            break;
        case EMOTION_COOL:
            drawCoolEyes(lx, ly, rx, ry);
            break;
        case EMOTION_CONFUSED:
            drawConfusedEyes(lx, ly, rx, ry);
            break;
        case EMOTION_EXCITED:
            drawExcitedEyes(lx, ly, rx, ry);
            break;
        case EMOTION_SHY:
            drawShyEyes(lx, ly, rx, ry);
            break;
    }
}

void RobotFace::updateLook() {
    unsigned long now = millis();
    if (now - lastLookTime > 2000 + random(3000)) {
        targetOffsetX = random(-5, 6);
        targetOffsetY = random(-3, 4);
        lastLookTime = now;
    }
    eyeOffsetX += (targetOffsetX - eyeOffsetX) * 0.1f;
    eyeOffsetY += (targetOffsetY - eyeOffsetY) * 0.1f;
}

void RobotFace::updateBlink() {
    unsigned long now = millis();
    if (!isBlinking && now - lastBlinkTime > BLINK_INTERVAL + random(2000)) {
        isBlinking = true;
        blinkStartTime = now;
        lastBlinkTime = now;
    }
    if (isBlinking && now - blinkStartTime > BLINK_DURATION) {
        isBlinking = false;
    }
}

void RobotFace::setEmotion(Emotion e) {
    currentEmotion = e;
}

Emotion RobotFace::getEmotion() {
    return currentEmotion;
}

Emotion RobotFace::nextEmotion() {
    currentEmotion = (Emotion)(((int)currentEmotion + 1) % TOTAL_EMOTIONS);
    return currentEmotion;
}

void RobotFace::update() {
    updateBlink();
    updateLook();
    oled->clear();
    drawEyes();
    oled->show();
}

const char* RobotFace::getEmotionName() {
    const char* names[] = {
        "Happy", "Sad", "Angry", "Surprised", "Sleepy",
        "Love", "Cool", "Confused", "Excited", "Shy"
    };
    return names[(int)currentEmotion];
}