#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "oled_display.h"
#include "config.h"

class Animations {
private:
    OLEDDisplay* oled;

public:
    Animations(OLEDDisplay* d);
    void playBootAnimation();
    void playWakeUpAnimation();
    void playSleepAnimation();
    void playTouchReaction();
    void playTransition();
    void playSoundReaction();
    void playDanceAnimation();
    void playEmotionTransition();
};

#endif