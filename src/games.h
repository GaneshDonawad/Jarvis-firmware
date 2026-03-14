#ifndef GAMES_H
#define GAMES_H

#include "oled_display.h"
#include "audio_output.h"
#include "config.h"

// ========== DINO GAME ==========
class DinoGame {
private:
    OLEDDisplay* oled;
    AudioOutput* audio;
    
    int dinoY;
    int dinoVel;
    bool jumping;
    int groundY;
    int dinoX;
    int dinoW;
    int dinoH;
    int cactX;
    int cactW;
    int cactH;
    int birdX;
    int birdY;
    bool birdOn;
    int score;
    int best;
    int spd;
    bool over;

    void drawDino();
    void drawCactus();
    void drawBird();
    void drawGround();
    bool collision();

public:
    DinoGame(OLEDDisplay* d, AudioOutput* a);
    void reset();
    bool isGameOver();
    void jump();
    void update();
};

// ========== PONG GAME ==========
class PongGame {
private:
    OLEDDisplay* oled;
    AudioOutput* audio;
    
    int padY;
    int padH;
    int padW;
    int padX;
    float bx;
    float by;
    float bsx;
    float bsy;
    int bs;
    int aiY;
    int aiX;
    int ps;
    int as_score;
    bool over;
    int maxS;

public:
    PongGame(OLEDDisplay* d, AudioOutput* a);
    void reset();
    bool isGameOver();
    void handleTouch(bool t);
    void update();
};

// ========== SIMON GAME ==========
class SimonGame {
private:
    OLEDDisplay* oled;
    AudioOutput* audio;
    
    static const int MAX_SEQ = 20;
    int seq[MAX_SEQ];
    int seqLen;
    int plIdx;
    int showIdx;
    bool plTurn;
    bool over;
    bool showing;
    int score;
    int best;
    unsigned long lastShow;

    void drawQuad(int q, bool filled);
    void drawLabels();

public:
    SimonGame(OLEDDisplay* d, AudioOutput* a);
    void reset();
    bool isGameOver();
    void handleTap();
    void update();
};

#endif