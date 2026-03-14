#include "image_viewer.h"

ImageViewer::ImageViewer(OLEDDisplay* d, SDHandler* s)
    : oled(d), sd(s), imageCount(0), currentImage(0),
      slideshowActive(false), lastSlideTime(0), slideInterval(3000) {}

void ImageViewer::begin() {
    if (!sd->isReady()) {
        imageCount = 0;
        Serial.println("SD Card not ready for image viewer");
        return;
    }
    
    imageCount = sd->listImages(imageFiles, MAX_IMAGES);
    currentImage = 0;
    
    Serial.print("Found ");
    Serial.print(imageCount);
    Serial.println(" BMP images");
}

void ImageViewer::nextImage() {
    if (imageCount == 0) return;
    currentImage = (currentImage + 1) % imageCount;
}

void ImageViewer::prevImage() {
    if (imageCount == 0) return;
    currentImage = (currentImage - 1 + imageCount) % imageCount;
}

void ImageViewer::toggleSlideshow() {
    slideshowActive = !slideshowActive;
    lastSlideTime = millis();
}

int ImageViewer::getImageCount() {
    return imageCount;
}

int ImageViewer::getCurrentIndex() {
    return currentImage;
}

void ImageViewer::showNoImages() {
    oled->clear();
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    oled->setCursor(0, 0);
    oled->print("== IMAGES ==");
    oled->drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    if (!sd->isReady()) {
        oled->setCursor(10, 25);
        oled->print("No SD Card!");
    } else {
        oled->setCursor(5, 20);
        oled->print("No BMP images");
        oled->setCursor(5, 32);
        oled->print("found in");
        oled->setCursor(5, 42);
        oled->print("/images/ folder");
    }

    oled->setCursor(0, 56);
    oled->print("Hold:Back");
    oled->show();
}

void ImageViewer::showCurrentImage() {
    if (sd->loadBMP(imageFiles[currentImage], bmpBuffer, sizeof(bmpBuffer))) {
        oled->clear();
        oled->drawBitmap(0, 0, bmpBuffer, 128, 64, SSD1306_WHITE);
        oled->show();
    } else {
        oled->clear();
        oled->setTextSize(1);
        oled->setTextColor(SSD1306_WHITE);
        oled->setCursor(0, 0);
        oled->print("Load failed:");
        oled->setCursor(0, 12);

        const char* name = imageFiles[currentImage];
        const char* slash = strrchr(name, '/');
        if (slash) name = slash + 1;
        oled->print(name);

        oled->setCursor(0, 30);
        oled->print("Must be 128x64");
        oled->setCursor(0, 40);
        oled->print("1-bit BMP");
        oled->setCursor(0, 56);
        oled->print("Tap:Next Hold:Back");
        oled->show();
    }
}

void ImageViewer::draw() {
    if (imageCount == 0) {
        showNoImages();
        return;
    }

    // Slideshow auto-advance
    if (slideshowActive) {
        unsigned long now = millis();
        if (now - lastSlideTime > slideInterval) {
            nextImage();
            lastSlideTime = now;
        }
    }

    showCurrentImage();

    // Overlay info bar at bottom
    oled->fillRect(0, 56, SCREEN_WIDTH, 8, SSD1306_BLACK);
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    oled->setCursor(0, 56);
    oled->print(currentImage + 1);
    oled->print("/");
    oled->print(imageCount);

    if (slideshowActive) {
        oled->setCursor(40, 56);
        oled->print("[AUTO]");
    }

    oled->setCursor(80, 56);
    oled->print("Hld:X");
    oled->show();
}