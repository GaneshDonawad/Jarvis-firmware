#ifndef ROBOT_FACE_H
#define ROBOT_FACE_H

#include "oled_display.h"
#include "config.h"

class RobotFace {
private:
    OLEDDisplay* oled;
    Emotion currentEmotion;
    bool isBlinking;
    unsigned long lastBlinkTime;
    unsigned long blinkStartTime;
    float eyeOffsetX;
    float eyeOffsetY;
    float targetOffsetX;
    float targetOffsetY;
    unsigned long lastLookTime;
    int leftEyeX;
    int rightEyeX;
    int eyeY;
    int eyeW;
    int eyeH;
    int pupilR;

    void drawSmile(int cx, int cy, int w);
    void drawFrown(int cx, int cy, int w);
    void drawHeart(int cx, int cy, int s);
    void drawStar(int cx, int cy, int s);
    void drawBlink(int lx, int ly, int rx, int ry);
    void drawEyePair(int lx, int ly, int rx, int ry, int r = 5);

    void drawHappyEyes(int lx, int ly, int rx, int ry);
    void drawSadEyes(int lx, int ly, int rx, int ry);
    void drawAngryEyes(int lx, int ly, int rx, int ry);
    void drawSurprisedEyes(int lx, int ly, int rx, int ry);
    void drawSleepyEyes(int lx, int ly, int rx, int ry);
    void drawLoveEyes(int lx, int ly, int rx, int ry);
    void drawCoolEyes(int lx, int ly, int rx, int ry);
    void drawConfusedEyes(int lx, int ly, int rx, int ry);
    void drawExcitedEyes(int lx, int ly, int rx, int ry);
    void drawShyEyes(int lx, int ly, int rx, int ry);

    void drawEyes();
    void updateLook();
    void updateBlink();

public:
    RobotFace(OLEDDisplay* d);
    void setEmotion(Emotion e);
    Emotion getEmotion();
    Emotion nextEmotion();
    void update();
    const char* getEmotionName();
};

#endif