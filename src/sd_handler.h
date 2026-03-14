#ifndef SD_HANDLER_H
#define SD_HANDLER_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "config.h"
#include "oled_display.h"

class SDHandler {
private:
    OLEDDisplay* oled;
    bool initialized;
    SPIClass* spiSD;

public:
    SDHandler(OLEDDisplay* d);
    ~SDHandler();
    bool begin();
    bool isReady();

    // File operations
    bool fileExists(const char* path);
    bool createDir(const char* path);
    bool writeFile(const char* path, const char* data);
    bool appendFile(const char* path, const char* data);
    bool readFile(const char* path, char* buffer, int maxLen);
    bool deleteFile(const char* path);

    // High score save/load
    bool saveHighScore(const char* game, int score);
    int loadHighScore(const char* game);

    // Settings save/load
    bool saveSetting(const char* key, int value);
    int loadSetting(const char* key, int defaultVal);

    // BMP image
    bool loadBMP(const char* path, uint8_t* buffer, int bufSize);

    // Image file listing
    int listImages(char fileList[][SD_MAX_PATH], int maxFiles);

    // Log
    bool logEvent(const char* event);

    // Card info
    void displayInfo();
};

#endif