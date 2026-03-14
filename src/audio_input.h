#ifndef AUDIO_INPUT_H
#define AUDIO_INPUT_H

#include <Arduino.h>
#include <driver/i2s.h>
#include "config.h"

class AudioInput {
private:
    int currentLevel;
    int peakLevel;
    bool initialized;

    unsigned long lastClapTime;
    int clapCount;
    bool clapDetected;

    static const int HISTORY_SIZE = 32;
    int history[HISTORY_SIZE];
    int historyIndex;

    static const int BUF_LEN = 256;
    int16_t readBuf[BUF_LEN];

public:
    AudioInput();
    void begin();
    void update();

    int getLevel();
    int getPeak();
    bool isLoud();
    bool hasSound();
    bool wasClap();
    int getClapCount();
    int getHistory(int index);
    int getHistorySize();
    int mapLevel(int minOut, int maxOut);
};

#endif