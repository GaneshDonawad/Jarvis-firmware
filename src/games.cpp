#include "games.h"

// ============================================================
// DINO GAME - COMPLETE IMPLEMENTATION
// ============================================================

DinoGame::DinoGame(OLEDDisplay* d, AudioOutput* a) : oled(d), audio(a) {
    groundY = 54;
    dinoX = 15;
    dinoW = 10;
    dinoH = 12;
    cactW = 6;
    cactH = 12;
    best = 0;
    reset();
}

void DinoGame::reset() {
    dinoY = groundY;
    dinoVel = 0;
    jumping = false;
    cactX = SCREEN_WIDTH;
    score = 0;
    spd = 3;
    over = false;
    birdX = SCREEN_WIDTH + 120;
    birdY = 30;
    birdOn = false;
}

bool DinoGame::isGameOver() {
    return over;
}

void DinoGame::jump() {
    if (!jumping && !over) {
        jumping = true;
        dinoVel = -8;
        audio->playTouchSound();
    }
    if (over) {
        reset();
    }
}

void DinoGame::drawDino() {
    // Draw dinosaur body
    oled->fillRect(dinoX, dinoY - dinoH, dinoW, dinoH, SSD1306_WHITE);
    
    // Draw eye
    oled->drawPixel(dinoX + 7, dinoY - dinoH + 2, SSD1306_BLACK);
    
    // Draw legs (animated)
    if (!jumping) {
        int l = (millis() / 100) % 2;
        oled->drawLine(dinoX + 2, dinoY, dinoX + 2, dinoY + 3 - l, SSD1306_WHITE);
        oled->drawLine(dinoX + 7, dinoY, dinoX + 7, dinoY + 2 + l, SSD1306_WHITE);
    }
}

void DinoGame::drawCactus() {
    // Main cactus body
    oled->fillRect(cactX, groundY - cactH, cactW, cactH, SSD1306_WHITE);
    
    // Left spike
    oled->fillRect(cactX - 2, groundY - cactH + 3, 2, 5, SSD1306_WHITE);
    
    // Right spike
    oled->fillRect(cactX + cactW, groundY - cactH + 5, 2, 4, SSD1306_WHITE);
}

void DinoGame::drawBird() {
    if (!birdOn) return;
    
    int wy = ((millis() / 150) % 2 == 0) ? -3 : 3;
    
    // Bird body
    oled->fillRect(birdX, birdY, 8, 3, SSD1306_WHITE);
    
    // Wing
    oled->drawLine(birdX + 2, birdY + wy, birdX + 6, birdY + wy, SSD1306_WHITE);
}

void DinoGame::drawGround() {
    // Ground line
    oled->drawLine(0, groundY + 4, SCREEN_WIDTH, groundY + 4, SSD1306_WHITE);
    
    // Ground dashes (moving)
    int off = (millis() / 50) % 20;
    for (int x = -off; x < SCREEN_WIDTH; x += 20) {
        oled->drawLine(x, groundY + 6, x + 5, groundY + 6, SSD1306_WHITE);
    }
}

bool DinoGame::collision() {
    // Collision with cactus
    if (dinoX + dinoW > cactX && dinoX < cactX + cactW && dinoY > groundY - cactH) {
        return true;
    }
    
    // Collision with bird
    if (birdOn && dinoX + dinoW > birdX && dinoX < birdX + 8 &&
        dinoY - dinoH < birdY + 3 && dinoY > birdY) {
        return true;
    }
    
    return false;
}

void DinoGame::update() {
    if (over) {
        oled->clear();
        oled->setTextSize(2);
        oled->setTextColor(SSD1306_WHITE);
        oled->setCursor(10, 2);
        oled->print("GAME");
        oled->setCursor(10, 20);
        oled->print("OVER!");
        
        oled->setTextSize(1);
        oled->setCursor(10, 42);
        oled->print("Score:");
        oled->print(score);
        oled->setCursor(10, 52);
        oled->print("Best: ");
        oled->print(best);
        
        oled->setCursor(5, 60);
        oled->print("Tap:Play  Hold:Exit");
        oled->show();
        return;
    }

    // Physics - jumping
    if (jumping) {
        dinoVel++;
        dinoY += dinoVel;
        if (dinoY >= groundY) {
            dinoY = groundY;
            jumping = false;
            dinoVel = 0;
        }
    }

    // Cactus movement
    cactX -= spd;
    if (cactX < -cactW) {
        cactX = SCREEN_WIDTH + random(30, 80);
        score++;
        audio->playScoreSound();
        
        // Increase difficulty
        if (score % 5 == 0 && spd < 8) {
            spd++;
        }
    }

    // Bird spawning
    if (score > 8 && !birdOn && random(100) < 2) {
        birdOn = true;
        birdX = SCREEN_WIDTH;
        birdY = 25 + random(15);
    }
    
    // Bird movement
    if (birdOn) {
        birdX -= spd + 1;
        if (birdX < -10) {
            birdOn = false;
        }
    }

    // Collision detection
    if (collision()) {
        over = true;
        if (score > best) {
            best = score;
        }
        audio->playGameOverSound();
        return;
    }

    // Render
    oled->clear();
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    oled->setCursor(85, 0);
    oled->print("S:");
    oled->print(score);
    
    drawGround();
    drawDino();
    drawCactus();
    drawBird();
    
    oled->show();
}

// ============================================================
// PONG GAME - COMPLETE IMPLEMENTATION
// ============================================================

PongGame::PongGame(OLEDDisplay* d, AudioOutput* a) : oled(d), audio(a) {
    padH = 16;
    padW = 3;
    padX = 5;
    bs = 3;
    aiX = 120;
    maxS = 5;
    reset();
}

void PongGame::reset() {
    padY = aiY = SCREEN_HEIGHT / 2 - padH / 2;
    bx = SCREEN_WIDTH / 2;
    by = SCREEN_HEIGHT / 2;
    bsx = 2;
    bsy = 1;
    ps = 0;
    as_score = 0;
    over = false;
}

bool PongGame::isGameOver() {
    return over;
}

void PongGame::handleTouch(bool t) {
    if (over && t) {
        reset();
        return;
    }
    
    if (t) {
        padY -= 4;
        if (padY < 0) padY = 0;
    } else {
        padY += 2;
        if (padY > SCREEN_HEIGHT - padH) padY = SCREEN_HEIGHT - padH;
    }
}

void PongGame::update() {
    if (over) {
        oled->clear();
        oled->setTextSize(2);
        oled->setTextColor(SSD1306_WHITE);
        oled->setCursor(10, 5);
        oled->print(ps >= maxS ? "YOU WIN!" : "YOU LOSE");
        
        oled->setTextSize(1);
        oled->setCursor(40, 30);
        oled->print(ps);
        oled->print(" - ");
        oled->print(as_score);
        
        oled->setCursor(10, 50);
        oled->print("Tap:Play  Hold:Exit");
        oled->show();
        return;
    }

    // Ball movement
    bx += bsx;
    by += bsy;

    // Wall collision (top/bottom)
    if (by <= 0 || by >= SCREEN_HEIGHT - bs) {
        bsy = -bsy;
        audio->playTouchSound();
    }

    // Player paddle collision
    if (bx <= padX + padW && by + bs >= padY && by <= padY + padH) {
        bsx = abs(bsx) * 1.05f;
        bsy = ((by - padY) / (float)padH - 0.5f) * 4;
        audio->playScoreSound();
    }

    // AI paddle collision
    if (bx + bs >= aiX && by + bs >= aiY && by <= aiY + padH) {
        bsx = -abs(bsx);
        audio->playTouchSound();
    }

    // AI movement
    int ac = aiY + padH / 2;
    if (ac < by - 2) {
        aiY += 2;
    }
    if (ac > by + 2) {
        aiY -= 2;
    }
    aiY = constrain(aiY, 0, SCREEN_HEIGHT - padH);

    // Scoring - player scores
    if (bx < 0) {
        as_score++;
        audio->playGameOverSound();
        bx = SCREEN_WIDTH / 2;
        by = SCREEN_HEIGHT / 2;
        bsx = 2;
        bsy = random(-2, 3);
    }

    // Scoring - AI scores
    if (bx > SCREEN_WIDTH) {
        ps++;
        audio->playScoreSound();
        bx = SCREEN_WIDTH / 2;
        by = SCREEN_HEIGHT / 2;
        bsx = -2;
        bsy = random(-2, 3);
    }

    // Check game end
    if (ps >= maxS || as_score >= maxS) {
        over = true;
        if (ps >= maxS) {
            audio->playWinSound();
        } else {
            audio->playGameOverSound();
        }
    }

    // Render
    oled->clear();
    
    // Center line (dashed)
    for (int y = 0; y < SCREEN_HEIGHT; y += 4) {
        oled->drawPixel(SCREEN_WIDTH / 2, y, SSD1306_WHITE);
    }
    
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    oled->setCursor(SCREEN_WIDTH / 4, 2);
    oled->print(ps);
    oled->setCursor(3 * SCREEN_WIDTH / 4, 2);
    oled->print(as_score);
    
    // Draw paddles
    oled->fillRect(padX, padY, padW, padH, SSD1306_WHITE);
    oled->fillRect(aiX, aiY, padW, padH, SSD1306_WHITE);
    
    // Draw ball
    oled->fillRect((int)bx, (int)by, bs, bs, SSD1306_WHITE);
    
    oled->show();
}

// ============================================================
// SIMON GAME - COMPLETE IMPLEMENTATION
// ============================================================

SimonGame::SimonGame(OLEDDisplay* d, AudioOutput* a) : oled(d), audio(a) {
    best = 0;
    reset();
}

void SimonGame::reset() {
    seqLen = 1;
    plIdx = 0;
    over = false;
    score = 0;
    plTurn = false;
    showing = true;
    showIdx = 0;
    lastShow = millis();
    
    for (int i = 0; i < MAX_SEQ; i++) {
        seq[i] = random(0, 4);
    }
}

bool SimonGame::isGameOver() {
    return over;
}

void SimonGame::drawQuad(int q, bool filled) {
    int x = (q % 2 == 0) ? 10 : 68;
    int y = (q < 2) ? 5 : 35;
    
    if (filled) {
        oled->fillRoundRect(x, y, 50, 24, 4, SSD1306_WHITE);
    } else {
        oled->drawRoundRect(x, y, 50, 24, 4, SSD1306_WHITE);
    }
}

void SimonGame::drawLabels() {
    oled->setTextSize(1);
    oled->setTextColor(SSD1306_WHITE);
    oled->setCursor(28, 14);
    oled->print("1");
    oled->setCursor(88, 14);
    oled->print("2");
    oled->setCursor(28, 44);
    oled->print("3");
    oled->setCursor(88, 44);
    oled->print("4");
}

void SimonGame::handleTap() {
    if (over) {
        reset();
        return;
    }
    
    if (!plTurn) {
        return;
    }

    int expected = seq[plIdx];
    audio->playSimonTone(expected);

    // Flash the correct quadrant
    oled->clear();
    for (int i = 0; i < 4; i++) {
        drawQuad(i, i == expected);
    }
    drawLabels();
    oled->show();
    delay(250);

    plIdx++;
    
    if (plIdx >= seqLen) {
        score++;
        if (score > best) {
            best = score;
        }
        seqLen++;
        
        if (seqLen > MAX_SEQ) {
            over = true;
            audio->playWinSound();
            return;
        }
        
        plIdx = 0;
        plTurn = false;
        showing = true;
        showIdx = 0;
        lastShow = millis() + 500;
    }
}

void SimonGame::update() {
    if (over) {
        oled->clear();
        oled->setTextSize(2);
        oled->setTextColor(SSD1306_WHITE);
        oled->setCursor(10, 2);
        oled->print("SIMON!");
        
        oled->setTextSize(1);
        oled->setCursor(10, 28);
        oled->print("Score: ");
        oled->print(score);
        oled->setCursor(10, 40);
        oled->print("Best:  ");
        oled->print(best);
        
        oled->setCursor(5, 56);
        oled->print("Tap:Play  Hold:Exit");
        oled->show();
        return;
    }

    unsigned long now = millis();

    if (showing) {
        if (now - lastShow > 550) {
            oled->clear();
            
            if (showIdx < seqLen) {
                int q = seq[showIdx];
                for (int i = 0; i < 4; i++) {
                    drawQuad(i, i == q);
                }
                drawLabels();
                audio->playSimonTone(q);
                showIdx++;
            } else {
                for (int i = 0; i < 4; i++) {
                    drawQuad(i, false);
                }
                drawLabels();
                showing = false;
                plTurn = true;
                plIdx = 0;
            }
            
            oled->setTextSize(1);
            oled->setTextColor(SSD1306_WHITE);
            oled->setCursor(45, 62);
            oled->print("Lvl:");
            oled->print(seqLen);
            
            oled->show();
            lastShow = now;
        }
    } else if (plTurn) {
        oled->clear();
        for (int i = 0; i < 4; i++) {
            drawQuad(i, false);
        }
        drawLabels();
        
        oled->setTextSize(1);
        oled->setTextColor(SSD1306_WHITE);
        oled->setCursor(5, 62);
        oled->print("Tap! ");
        oled->print(plIdx + 1);
        oled->print("/");
        oled->print(seqLen);
        
        oled->show();
    }
}