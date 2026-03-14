#include "ai_assistant.h"

AIAssistant::AIAssistant(OLEDDisplay* d) : oled(d), currentItem(0), currentPage(0) {
    tips[0] = "Stay hydrated\nevery day!";
    tips[1] = "Take a break\nfrom screens.";
    tips[2] = "Stretch your\nbody often!";
    tips[3] = "Smile more,\nit really helps!";
    tips[4] = "Deep breaths\nhelp you relax.";
    tips[5] = "You're doing\nreally great!";
    tips[6] = "Be kind to\nyourself today.";
    tips[7] = "Learn something\nnew each day!";

    facts[0] = "Honey never\nspoils ever!";
    facts[1] = "Octopuses have\nthree hearts!";
    facts[2] = "Bananas are\nactually berries";
    facts[3] = "Lightning is\n5x hotter than\nthe sun!";
    facts[4] = "A flamingo\ngroup is a\nflamboyance!";
    facts[5] = "Sharks are\nolder than\ntrees!";
    facts[6] = "Your nose can\nsmell 1 trillion\nscents!";
    facts[7] = "Wombat poop\nis cube-shaped!";

    jokes[0] = "Why do robots\nnever scared?\nSteel nerves!";
    jokes[1] = "Robot's fav\nmusic genre?\nHeavy metal!";
    jokes[2] = "Why was robot\nangry? People\npushed buttons!";
    jokes[3] = "How do robots\neat salsa?\nWith micro-\nchips!";
    jokes[4] = "What do you\ncall a pirate\nrobot?\nARRR-2D2!";
    jokes[5] = "Robot went on\nholiday to\nrecharge its\nbatteries!";
    jokes[6] = "Robot's fav\nexercise?\nCircuit\ntraining!";
    jokes[7] = "Why are robots\nnever lonely?\nThey have\nbytes!";

    quotes[0] = "Best way to\npredict future\nis to create\nit.";
    quotes[1] = "Do or do not.\nThere is no\ntry. -Yoda";
    quotes[2] = "Stay hungry,\nstay foolish.\n-Steve Jobs";
    quotes[3] = "Be the change\nyou wish to\nsee in the\nworld.";
    quotes[4] = "In difficulty\nlies great\nopportunity.";
    quotes[5] = "Every moment\nis a fresh\nnew beginning.";
    quotes[6] = "Believe you\ncan and you're\nhalfway there.";
    quotes[7] = "The only way\nto do great\nwork is love\nwhat you do.";

    pageNames[0] = "AI TIP";
    pageNames[1] = "FUN FACT";
    pageNames[2] = "JOKE";
    pageNames[3] = "QUOTE";
}

void AIAssistant::nextItem() {
    currentItem = (currentItem + 1) % ITEMS;
}

void AIAssistant::nextPage() {
    currentPage = (currentPage + 1) % PAGES;
    currentItem = 0;
}

void AIAssistant::display() {
    oled->clear();
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    
    oled->setCursor(0, 0);
    oled->print("== ");
    oled->print(pageNames[currentPage]);
    oled->print(" ==");
    
    oled->drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    oled->setCursor(0, 14);
    
    switch (currentPage) {
        case 0:
            oled->print(tips[currentItem]);
            break;
        case 1:
            oled->print(facts[currentItem]);
            break;
        case 2:
            oled->print(jokes[currentItem]);
            break;
        case 3:
            oled->print(quotes[currentItem]);
            break;
    }

    oled->setCursor(0, 56);
    oled->print("Tap:Next Dbl:Page");
    oled->show();
}