#include <Arduino.h>
#include "config.h"
#include "oled_display.h"
#include "robot_face.h"
#include "animations.h"
#include "touch_handler.h"
#include "audio_input.h"
#include "audio_output.h"
#include "games.h"
#include "ai_assistant.h"
#include "bluetooth_handler.h"
#include "wifi_handler.h"
#include "magic8ball.h"
#include "mic_visualizer.h"
#include "sd_handler.h"
#include "image_viewer.h"

// ========== GLOBAL OBJECTS ==========
OLEDDisplay        oled;
RobotFace          face(&oled);
Animations         anim(&oled);
TouchHandler       touch(PIN_TOUCH);
AudioInput         micInput;
AudioOutput        audioOut;
SDHandler          sdCard(&oled);
DinoGame           dino(&oled, &audioOut);
PongGame           pong(&oled, &audioOut);
SimonGame          simon(&oled, &audioOut);
AIAssistant        ai(&oled);
BluetoothHandler   bt(&oled);
WiFiHandler        wifi(&oled);
Magic8Ball         magic8(&oled, &audioOut);
MicVisualizer      micVis(&oled, &micInput);
ImageViewer        imgView(&oled, &sdCard);

// ========== GLOBAL STATE ==========
RobotMode      currentMode = MODE_FACE;
unsigned long  lastInteraction = 0;
bool           sleeping = false;
int            menuSel = 0;
const int      totalMenu = 9;
unsigned long  soundCooldown = 0;

const char* menuItems[] = {
    "Robot Face",
    "Dino Game",
    "Pong Game",
    "Simon Says",
    "AI Assistant",
    "Mic Visualizer",
    "Image Viewer",
    "Bluetooth",
    "WiFi"
};

// ========== HELPER FUNCTIONS ==========

void playEmotionSound(Emotion e) {
    switch (e) {
        case EMOTION_HAPPY:
            audioOut.playHappySound();
            break;
        case EMOTION_SAD:
            audioOut.playSadSound();
            break;
        case EMOTION_ANGRY:
            audioOut.playAngrySound();
            break;
        case EMOTION_SURPRISED:
            audioOut.playSurprisedSound();
            break;
        case EMOTION_SLEEPY:
            audioOut.playSleepSound();
            break;
        case EMOTION_LOVE:
            audioOut.playLoveSound();
            break;
        case EMOTION_COOL:
            audioOut.playCoolSound();
            break;
        case EMOTION_CONFUSED:
            audioOut.playConfusedSound();
            break;
        case EMOTION_EXCITED:
            audioOut.playExcitedSound();
            break;
        case EMOTION_SHY:
            audioOut.playShySound();
            break;
    }
}

void drawMenu() {
    oled.clear();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(20, 0);
    oled.print("== MENU ==");
    oled.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    int startItem = 0;
    if (menuSel > 3) {
        startItem = menuSel - 3;
    }
    int endItem = min(startItem + 5, totalMenu);

    for (int i = startItem; i < endItem; i++) {
        int y = 14 + (i - startItem) * 10;
        if (i == menuSel) {
            oled.fillRect(0, y - 1, SCREEN_WIDTH, 10, SSD1306_WHITE);
            oled.setTextColor(SSD1306_BLACK);
        } else {
            oled.setTextColor(SSD1306_WHITE);
        }
        oled.setCursor(5, y);
        oled.print(menuItems[i]);
    }

    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 56);
    oled.print("Tap:Next Dbl:Select");
    oled.show();
}

void enterMode(RobotMode m) {
    anim.playTransition();
    currentMode = m;
    if (m == MODE_GAME_DINO) {
        dino.reset();
    }
    if (m == MODE_GAME_PONG) {
        pong.reset();
    }
    if (m == MODE_GAME_SIMON) {
        simon.reset();
    }
    if (m == MODE_IMAGE_VIEWER) {
        imgView.begin();
    }
    sdCard.logEvent(menuItems[menuSel]);
}

// ========== SETUP ==========

void setup() {
    Serial.begin(115200);
    delay(3000);
    Serial.println("=== ROBO BUDDY v2.0 + SD CARD ===");
    Serial.println("Initializing all components...");

    // Initialize OLED
    if (!oled.begin()) {
        Serial.println("OLED initialization FAILED! Halting.");
        while (true) {
            delay(1000);
        }
    }

    // Initialize touch
    touch.begin();

    // Initialize audio
    audioOut.begin();
    micInput.begin();

    // Initialize Bluetooth
    bt.begin();

    // Initialize SD Card
    bool sdReady = sdCard.begin();
    if (sdReady) {
        Serial.println("SD Card initialized successfully");
        sdCard.logEvent("BOOT");
        
        // Load saved settings
        int soundSetting = sdCard.loadSetting("sound", 1);
        if (!soundSetting) {
            audioOut.toggleSound();
        }
    } else {
        Serial.println("SD Card not available - continuing without storage");
    }

    // Boot animations and sounds
    audioOut.playBootSound();
    anim.playBootAnimation();
    anim.playWakeUpAnimation();

    lastInteraction = millis();
    Serial.println("=== ALL SYSTEMS READY ===");
    Serial.println("Touch sensor: Single/Double/Triple tap or Long press");
    Serial.println("Current mode: Robot Face");
}

// ========== MAIN LOOP ==========

void loop() {
    TouchEvent event = touch.update();
    unsigned long now = millis();
    micInput.update();

    // ===== SLEEPING MODE =====
    if (sleeping) {
        if (event != TOUCH_NONE || micInput.isLoud()) {
            sleeping = false;
            anim.playWakeUpAnimation();
            audioOut.playWakeUp();
            lastInteraction = now;
            face.setEmotion(EMOTION_HAPPY);
            sdCard.logEvent("WAKE");
        }
        return;
    }

    // Update last interaction time
    if (event != TOUCH_NONE) {
        lastInteraction = now;
    }

    // ===== SOUND REACTION (Face mode only) =====
    if (currentMode == MODE_FACE && micInput.isLoud() && now - soundCooldown > 2000) {
        soundCooldown = now;
        lastInteraction = now;
        Emotion prev = face.getEmotion();
        face.setEmotion(EMOTION_SURPRISED);
        anim.playSoundReaction();
        audioOut.playSurprisedSound();
        delay(300);
        face.setEmotion(prev);
        sdCard.logEvent("SOUND_REACT");
    }

    // ===== DOUBLE CLAP = DANCE =====
    if (currentMode == MODE_FACE && micInput.getClapCount() >= 2 && now - soundCooldown > 3000) {
        soundCooldown = now;
        lastInteraction = now;
        face.setEmotion(EMOTION_EXCITED);
        anim.playDanceAnimation();
        audioOut.playExcitedSound();
        face.setEmotion(EMOTION_HAPPY);
        sdCard.logEvent("DANCE");
    }

    // ===== IDLE TIMEOUT → SLEEP =====
    if (now - lastInteraction > IDLE_TIMEOUT && currentMode == MODE_FACE) {
        sleeping = true;
        face.setEmotion(EMOTION_SLEEPY);
        face.update();
        delay(800);
        anim.playSleepAnimation();
        audioOut.playSleepSound();
        sdCard.logEvent("SLEEP");
        return;
    }

    // ========== MODE STATE MACHINE ==========

    switch (currentMode) {

        // ---------- FACE MODE ----------
        case MODE_FACE: {
            if (event == TOUCH_SINGLE) {
                audioOut.playTouchSound();
                anim.playTouchReaction();
                Emotion e = face.nextEmotion();
                playEmotionSound(e);
                Serial.print("Emotion changed to: ");
                Serial.println(face.getEmotionName());
            }
            else if (event == TOUCH_DOUBLE) {
                audioOut.playDoubleTapSound();
                currentMode = MODE_MENU;
                menuSel = 0;
                drawMenu();
                sdCard.logEvent("MENU_OPEN");
                return;
            }
            else if (event == TOUCH_TRIPLE) {
                audioOut.playTripleTapSound();
                currentMode = MODE_MAGIC_8BALL;
                sdCard.logEvent("MAGIC8_OPEN");
                return;
            }
            else if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                audioOut.toggleSound();
                sdCard.saveSetting("sound", audioOut.isSoundOn() ? 1 : 0);
                
                oled.clear();
                oled.setTextSize(2);
                oled.setTextColor(SSD1306_WHITE);
                oled.setCursor(15, 15);
                oled.print("Sound:");
                oled.setCursor(30, 38);
                oled.print(audioOut.isSoundOn() ? "ON" : "OFF");
                oled.show();
                
                delay(1000);
                
                Serial.print("Sound toggled: ");
                Serial.println(audioOut.isSoundOn() ? "ON" : "OFF");
            }
            
            face.update();
            break;
        }

        // ---------- MENU MODE ----------
        case MODE_MENU: {
            if (event == TOUCH_SINGLE) {
                audioOut.playMenuSound();
                menuSel = (menuSel + 1) % totalMenu;
                drawMenu();
            }
            else if (event == TOUCH_DOUBLE) {
                audioOut.playDoubleTapSound();
                Serial.print("Selected: ");
                Serial.println(menuItems[menuSel]);
                
                switch (menuSel) {
                    case 0:
                        enterMode(MODE_FACE);
                        break;
                    case 1:
                        enterMode(MODE_GAME_DINO);
                        break;
                    case 2:
                        enterMode(MODE_GAME_PONG);
                        break;
                    case 3:
                        enterMode(MODE_GAME_SIMON);
                        break;
                    case 4:
                        enterMode(MODE_AI_CHAT);
                        break;
                    case 5:
                        enterMode(MODE_MIC_VISUALIZER);
                        break;
                    case 6:
                        enterMode(MODE_IMAGE_VIEWER);
                        break;
                    case 7:
                        currentMode = MODE_BLUETOOTH;
                        bt.displayStatus();
                        break;
                    case 8:
                        currentMode = MODE_WIFI;
                        wifi.displayStatus();
                        break;
                }
            }
            else if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_FACE;
                sdCard.logEvent("MENU_CLOSE");
            }
            break;
        }

        // ---------- DINO GAME ----------
        case MODE_GAME_DINO: {
            if (event == TOUCH_SINGLE || event == TOUCH_DOUBLE) {
                dino.jump();
            }
            else if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_FACE;
                int score = dino.isGameOver() ? 0 : 0;
                sdCard.logEvent("DINO_EXIT");
                break;
            }
            
            // Voice jump
            if (micInput.isLoud()) {
                dino.jump();
            }
            
            dino.update();
            delay(30);
            break;
        }

        // ---------- PONG GAME ----------
        case MODE_GAME_PONG: {
            pong.handleTouch(touch.isTouched());
            
            if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_FACE;
                sdCard.logEvent("PONG_EXIT");
                break;
            }
            
            pong.update();
            delay(30);
            break;
        }

        // ---------- SIMON GAME ----------
        case MODE_GAME_SIMON: {
            if (event == TOUCH_SINGLE) {
                simon.handleTap();
            }
            else if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_FACE;
                sdCard.logEvent("SIMON_EXIT");
                break;
            }
            
            simon.update();
            break;
        }

        // ---------- AI ASSISTANT ----------
        case MODE_AI_CHAT: {
            if (event == TOUCH_SINGLE) {
                audioOut.playTouchSound();
                ai.nextItem();
            }
            else if (event == TOUCH_DOUBLE) {
                audioOut.playDoubleTapSound();
                ai.nextPage();
            }
            else if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_FACE;
                sdCard.logEvent("AI_EXIT");
                break;
            }
            
            ai.display();
            break;
        }

        // ---------- MIC VISUALIZER ----------
        case MODE_MIC_VISUALIZER: {
            if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_FACE;
                sdCard.logEvent("MIC_VIS_EXIT");
                break;
            }
            
            micVis.draw();
            break;
        }

        // ---------- MAGIC 8-BALL ----------
        case MODE_MAGIC_8BALL: {
            if (event == TOUCH_SINGLE) {
                magic8.ask();
            }
            else if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_FACE;
                sdCard.logEvent("MAGIC8_EXIT");
                break;
            }
            
            magic8.draw();
            break;
        }

        // ---------- IMAGE VIEWER ----------
        case MODE_IMAGE_VIEWER: {
            if (event == TOUCH_SINGLE) {
                imgView.nextImage();
                audioOut.playTouchSound();
            }
            else if (event == TOUCH_DOUBLE) {
                imgView.toggleSlideshow();
                audioOut.playDoubleTapSound();
            }
            else if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_FACE;
                sdCard.logEvent("IMAGES_EXIT");
                break;
            }
            
            imgView.draw();
            break;
        }

        // ---------- BLUETOOTH ----------
        case MODE_BLUETOOTH: {
            if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_MENU;
                menuSel = 7;
                drawMenu();
            }
            break;
        }

        // ---------- WIFI ----------
        case MODE_WIFI: {
            if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_MENU;
                menuSel = 8;
                drawMenu();
            }
            break;
        }

        // ---------- SETTINGS ----------
        case MODE_SETTINGS: {
            if (event == TOUCH_LONG) {
                audioOut.playLongPressSound();
                currentMode = MODE_MENU;
                drawMenu();
            }
            break;
        }

        default:
            currentMode = MODE_FACE;
            break;
    }
}