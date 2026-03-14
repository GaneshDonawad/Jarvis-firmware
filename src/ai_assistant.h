#ifndef AI_ASSISTANT_H
#define AI_ASSISTANT_H

#include "oled_display.h"
#include "config.h"

class AIAssistant {
private:
    OLEDDisplay* oled;
    int currentItem;
    int currentPage;
    static const int PAGES = 4;
    static const int ITEMS = 8;

    const char* tips[8];
    const char* facts[8];
    const char* jokes[8];
    const char* quotes[8];
    const char* pageNames[4];

public:
    AIAssistant(OLEDDisplay* d);
    void nextItem();
    void nextPage();
    void display();
};

#endif