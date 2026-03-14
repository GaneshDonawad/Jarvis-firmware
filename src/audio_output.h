#ifndef AUDIO_OUTPUT_H
#define AUDIO_OUTPUT_H

#include <Arduino.h>
#include <driver/i2s.h>
#include "config.h"

class AudioOutput {
private:
    bool initialized;
    bool soundEnabled;

    void writeTone(int frequency, int durationMs);
    void writeSweep(int freqStart, int freqEnd, int durationMs);

public:
    AudioOutput();
    void begin();
    void toggleSound();
    bool isSoundOn();

    // UI Sounds
    void playBootSound();
    void playTouchSound();
    void playDoubleTapSound();
    void playTripleTapSound();
    void playLongPressSound();
    void playMenuSound();

    // Emotion Sounds
    void playHappySound();
    void playSadSound();
    void playAngrySound();
    void playSurprisedSound();
    void playSleepSound();
    void playLoveSound();
    void playCoolSound();
    void playConfusedSound();
    void playExcitedSound();
    void playShySound();

    // System Sounds
    void playWakeUp();
    void playSoundReaction();

    // Game Sounds
    void playGameOverSound();
    void playScoreSound();
    void playWinSound();
    void playSimonTone(int index);

    // Feature Sounds
    void playMagic8Ball();
};

#endif