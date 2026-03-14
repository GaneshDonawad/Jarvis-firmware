#include "sd_handler.h"

SDHandler::SDHandler(OLEDDisplay* d) : oled(d), initialized(false), spiSD(nullptr) {}

SDHandler::~SDHandler() {
    if (spiSD) {
        delete spiSD;
        spiSD = nullptr;
    }
}

bool SDHandler::begin() {
    spiSD = new SPIClass(HSPI);
    spiSD->begin(PIN_SD_SCK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);

    if (!SD.begin(PIN_SD_CS, *spiSD)) {
        Serial.println("SD Card initialization FAILED");
        initialized = false;
        return false;
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD card detected");
        initialized = false;
        return false;
    }

    initialized = true;

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.print("SD Card Size: ");
    Serial.print((unsigned long)cardSize);
    Serial.println(" MB");

    // Create default directories
    createDir("/images");
    createDir("/sounds");
    createDir("/data");
    createDir("/logs");
    createDir("/games");

    Serial.println("SD Card initialized successfully");
    return true;
}

bool SDHandler::isReady() {
    return initialized;
}

bool SDHandler::fileExists(const char* path) {
    if (!initialized) return false;
    return SD.exists(path);
}

bool SDHandler::createDir(const char* path) {
    if (!initialized) return false;
    if (SD.exists(path)) return true;
    return SD.mkdir(path);
}

bool SDHandler::writeFile(const char* path, const char* data) {
    if (!initialized) return false;
    File file = SD.open(path, FILE_WRITE);
    if (!file) return false;
    file.print(data);
    file.close();
    return true;
}

bool SDHandler::appendFile(const char* path, const char* data) {
    if (!initialized) return false;
    File file = SD.open(path, FILE_APPEND);
    if (!file) return false;
    file.print(data);
    file.close();
    return true;
}

bool SDHandler::readFile(const char* path, char* buffer, int maxLen) {
    if (!initialized) return false;
    File file = SD.open(path, FILE_READ);
    if (!file) return false;
    
    int len = 0;
    while (file.available() && len < maxLen - 1) {
        buffer[len++] = file.read();
    }
    buffer[len] = '\0';
    file.close();
    return true;
}

bool SDHandler::deleteFile(const char* path) {
    if (!initialized) return false;
    return SD.remove(path);
}

bool SDHandler::saveHighScore(const char* game, int score) {
    char path[SD_MAX_PATH];
    snprintf(path, sizeof(path), "/games/%s.txt", game);
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", score);
    return writeFile(path, buf);
}

int SDHandler::loadHighScore(const char* game) {
    char path[SD_MAX_PATH];
    snprintf(path, sizeof(path), "/games/%s.txt", game);
    char buf[16];
    if (readFile(path, buf, sizeof(buf))) {
        return atoi(buf);
    }
    return 0;
}

bool SDHandler::saveSetting(const char* key, int value) {
    char path[SD_MAX_PATH];
    snprintf(path, sizeof(path), "/data/%s.txt", key);
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", value);
    return writeFile(path, buf);
}

int SDHandler::loadSetting(const char* key, int defaultVal) {
    char path[SD_MAX_PATH];
    snprintf(path, sizeof(path), "/data/%s.txt", key);
    char buf[16];
    if (readFile(path, buf, sizeof(buf))) {
        return atoi(buf);
    }
    return defaultVal;
}

bool SDHandler::loadBMP(const char* path, uint8_t* buffer, int bufSize) {
    if (!initialized) return false;
    
    File file = SD.open(path, FILE_READ);
    if (!file) {
        Serial.print("BMP file not found: ");
        Serial.println(path);
        return false;
    }

    // Read BMP header
    uint8_t header[62];
    if (file.read(header, 62) != 62) {
        file.close();
        return false;
    }

    // Verify BMP signature
    if (header[0] != 'B' || header[1] != 'M') {
        Serial.println("Invalid BMP signature");
        file.close();
        return false;
    }

    // Get image dimensions
    int32_t width = *(int32_t*)&header[18];
    int32_t height = *(int32_t*)&header[22];
    uint16_t bpp = *(uint16_t*)&header[28];

    Serial.print("BMP: ");
    Serial.print(width);
    Serial.print("x");
    Serial.print(height);
    Serial.print(" ");
    Serial.print(bpp);
    Serial.println("bpp");

    if (width != 128 || abs(height) != 64) {
        Serial.println("BMP must be 128x64");
        file.close();
        return false;
    }

    // Read pixel data
    uint32_t dataOffset = *(uint32_t*)&header[10];
    file.seek(dataOffset);

    // 128x64 mono = 1024 bytes (128 pixels / 8 bits * 64 rows)
    int rowBytes = (width + 7) / 8;
    int padding = (4 - (rowBytes % 4)) % 4;
    int neededSize = (width * abs(height) + 7) / 8;

    if (neededSize > bufSize) {
        Serial.println("Buffer too small for BMP");
        file.close();
        return false;
    }

    memset(buffer, 0, bufSize);

    // BMP stores bottom-to-top
    bool bottomUp = (height > 0);
    int absHeight = abs(height);

    for (int row = 0; row < absHeight; row++) {
        int destRow = bottomUp ? (absHeight - 1 - row) : row;

        uint8_t rowData[20];
        int readLen = rowBytes + padding;
        file.read(rowData, readLen);

        for (int col = 0; col < width; col++) {
            int srcByte = col / 8;
            int srcBit = 7 - (col % 8);
            bool pixel = (rowData[srcByte] >> srcBit) & 1;

            // For SSD1306: pages are 8 rows vertical
            int page = destRow / 8;
            int bit = destRow % 8;
            int idx = page * width + col;

            if (bpp == 1) {
                if (!pixel) {
                    buffer[idx] |= (1 << bit);
                }
            }
        }
    }

    file.close();
    Serial.println("BMP loaded successfully");
    return true;
}

int SDHandler::listImages(char fileList[][SD_MAX_PATH], int maxFiles) {
    if (!initialized) return 0;
    
    File dir = SD.open("/images");
    if (!dir || !dir.isDirectory()) return 0;

    int count = 0;
    File entry;
    
    while ((entry = dir.openNextFile()) && count < maxFiles) {
        if (!entry.isDirectory()) {
            const char* name = entry.name();
            int len = strlen(name);
            
            // Check for .bmp extension
            if (len > 4 &&
                (name[len-4] == '.' &&
                 (name[len-3] == 'b' || name[len-3] == 'B') &&
                 (name[len-2] == 'm' || name[len-2] == 'M') &&
                 (name[len-1] == 'p' || name[len-1] == 'P'))) {
                snprintf(fileList[count], SD_MAX_PATH, "/images/%s", name);
                count++;
            }
        }
        entry.close();
    }
    
    dir.close();
    return count;
}

bool SDHandler::logEvent(const char* event) {
    char entry[128];
    unsigned long t = millis();
    snprintf(entry, sizeof(entry), "[%lu] %s\n", t, event);
    return appendFile("/logs/events.txt", entry);
}

void SDHandler::displayInfo() {
    oled->clear();
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    oled->setCursor(0, 0);
    oled->print("== SD CARD ==");
    oled->drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    if (!initialized) {
        oled->setCursor(0, 20);
        oled->print("NOT DETECTED");
    } else {
        uint64_t total = SD.totalBytes() / (1024 * 1024);
        uint64_t used = SD.usedBytes() / (1024 * 1024);

        oled->setCursor(0, 14);
        oled->print("Total: ");
        oled->print((int)total);
        oled->print(" MB");

        oled->setCursor(0, 26);
        oled->print("Used:  ");
        oled->print((int)used);
        oled->print(" MB");

        oled->setCursor(0, 38);
        oled->print("Free:  ");
        oled->print((int)(total - used));
        oled->print(" MB");

        oled->setCursor(0, 50);
        if (fileExists("/images")) {
            oled->print("Images: YES");
        } else {
            oled->print("Images: NO");
        }
    }
    oled->show();
}