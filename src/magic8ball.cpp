#include "magic8ball.h"

Magic8Ball::Magic8Ball(OLEDDisplay* d, AudioOutput* a)
    : oled(d), audio(a), active(false), startTime(0), answerIndex(0) {
    answers[0]  = "Yes!";
    answers[1]  = "No way!";
    answers[2]  = "Maybe...";
    answers[3]  = "Ask again";
    answers[4]  = "Absolutely!";
    answers[5]  = "Doubtful";
    answers[6]  = "100% yes!";
    answers[7]  = "Nope!";
    answers[8]  = "Try again";
    answers[9]  = "For sure!";
    answers[10] = "Not likely";
    answers[11] = "Go for it!";
    answers[12] = "Hmm...";
    answers[13] = "Of course!";
    answers[14] = "Never!";
    answers[15] = "Why not?";
}

void Magic8Ball::ask() {
    audio->playMagic8Ball();
    active = true;
    startTime = millis();
    answerIndex = random(0, TOTAL_ANSWERS);
}

bool Magic8Ball::isActive() {
    return active;
}

void Magic8Ball::draw() {
    oled->clear();

    // Draw magic 8-ball
    oled->drawCircle(64, 26, 25, SSD1306_WHITE);
    oled->drawCircle(64, 26, 26, SSD1306_WHITE);

    // Inner triangle
    oled->drawTriangle(50, 33, 78, 33, 64, 14, SSD1306_WHITE);

    unsigned long now = millis();
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);

    if (active && now - startTime < 2000) {
        // Show thinking animation
        oled->setCursor(35, 23);
        oled->print("Thinking");
        int dots = ((now - startTime) / 300) % 4;
        for (int i = 0; i < dots; i++) {
            oled->print(".");
        }
    } else if (active) {
        // Show answer
        oled->setCursor(10, 23);
        oled->print(answers[answerIndex]);
        active = false;
    } else {
        // Idle - show 8
        oled->setCursor(58, 23);
        oled->print("8");
    }

    oled->setCursor(0, 57);
    oled->print("Tap:Ask  Hold:Back");
    oled->show();
}