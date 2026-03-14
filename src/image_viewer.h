#ifndef IMAGE_VIEWER_H
#define IMAGE_VIEWER_H

#include "oled_display.h"
#include "sd_handler.h"
#include "config.h"

class ImageViewer {
private:
    OLEDDisplay* oled;
    SDHandler* sd;

    static const int MAX_IMAGES = 20;
    char imageFiles[MAX_IMAGES][SD_MAX_PATH];
    int imageCount;
    int currentImage;
    bool slideshowActive;
    unsigned long lastSlideTime;
    unsigned long slideInterval;

    uint8_t bmpBuffer[1024];

    void showCurrentImage();
    void showNoImages();

public:
    ImageViewer(OLEDDisplay* d, SDHandler* s);
    void begin();
    void nextImage();
    void prevImage();
    void toggleSlideshow();
    void draw();
    int getImageCount();
    int getCurrentIndex();
};

#endif