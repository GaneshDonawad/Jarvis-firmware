#ifndef MIC_VISUALIZER_H
#define MIC_VISUALIZER_H

#include "oled_display.h"
#include "audio_input.h"
#include "config.h"

class MicVisualizer {
private:
    OLEDDisplay* oled;
    AudioInput* mic;

public:
    MicVisualizer(OLEDDisplay* d, AudioInput* m);
    void draw();
};

#endif