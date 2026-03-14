#include "mic_visualizer.h"

MicVisualizer::MicVisualizer(OLEDDisplay* d, AudioInput* m)
    : oled(d), mic(m) {}

void MicVisualizer::draw() {
    oled->clear();
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);

    // Current level
    oled->setCursor(0, 0);
    oled->print("MIC:");
    oled->print(mic->getLevel());

    // Peak level
    oled->setCursor(75, 0);
    oled->print("Pk:");
    oled->print(mic->getPeak());

    // VU meter bar
    int barWidth = mic->mapLevel(0, 100);
    oled->drawRect(0, 11, 102, 8, SSD1306_WHITE);
    oled->fillRect(1, 12, barWidth, 6, SSD1306_WHITE);

    // Peak marker line
    int peakX = constrain(map(mic->getPeak(), 0, 8000, 0, 100), 0, 100);
    oled->drawLine(peakX + 1, 11, peakX + 1, 18, SSD1306_WHITE);

    // Waveform history
    int historySize = mic->getHistorySize();
    int xSpacing = max(1, SCREEN_WIDTH / historySize);
    
    for (int i = 0; i < historySize - 1; i++) {
        int y1 = constrain(50 - map(mic->getHistory(i), 0, 8000, 0, 28), 22, 55);
        int y2 = constrain(50 - map(mic->getHistory(i + 1), 0, 8000, 0, 28), 22, 55);
        oled->drawLine(i * xSpacing, y1, (i + 1) * xSpacing, y2, SSD1306_WHITE);
    }

    // Center baseline
    oled->drawLine(0, 50, SCREEN_WIDTH, 50, SSD1306_WHITE);

    // Status text
    oled->setCursor(0, 57);
    if (mic->isLoud()) {
        oled->print(">>> LOUD! <<<");
    } else if (mic->hasSound()) {
        oled->print("~ Sound ~");
    } else {
        oled->print(". Quiet .");
    }

    oled->setCursor(90, 57);
    oled->print("Hld:X");
    oled->show();
}