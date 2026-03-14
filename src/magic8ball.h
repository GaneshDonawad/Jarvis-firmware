#ifndef MAGIC8BALL_H
#define MAGIC8BALL_H

#include "oled_display.h"
#include "audio_output.h"
#include "config.h"

class Magic8Ball {
private:
    OLEDDisplay* oled;
    AudioOutput* audio;
    bool active;
    unsigned long startTime;
    int answerIndex;

    static const int TOTAL_ANSWERS = 16;
    const char* answers[TOTAL_ANSWERS];

public:
    Magic8Ball(OLEDDisplay* d, AudioOutput* a);
    void ask();
    void draw();
    bool isActive();
};

#endif