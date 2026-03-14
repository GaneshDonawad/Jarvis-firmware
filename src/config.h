#ifndef CONFIG_H
#define CONFIG_H

// ========== DISPLAY ==========
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_ADDR     0x3C

// ========== PIN MAP ==========
#define PIN_MIC_SD       1    // D0  = GPIO1   INMP441 SD (mic data in)
#define PIN_SD_CS        2    // D1  = GPIO2   SD Card Chip Select
#define PIN_I2S_DOUT     3    // D2  = GPIO3   MAX98357A DIN (audio out)
#define PIN_I2S_BCLK     4    // D3  = GPIO4   Shared I2S BCLK
// D4  = GPIO5   OLED SDA (Wire default)
// D5  = GPIO6   OLED SCL (Wire default)
#define PIN_I2S_LRCLK    43   // D6  = GPIO43  Shared I2S LRCLK
#define PIN_SD_SCK       44   // D7  = GPIO44  SD Card SCK
#define PIN_SD_MOSI      7    // D8  = GPIO7   SD Card MOSI
#define PIN_SD_MISO      8    // D9  = GPIO8   SD Card MISO
#define PIN_TOUCH        9    // D10 = GPIO9   TTP223 Touch

// MAX98357A SD pin wired to 3V3 (always enabled)

// ========== I2S AUDIO ==========
#define I2S_SAMPLE_RATE  16000
#define I2S_SPK_PORT     I2S_NUM_0
#define I2S_MIC_PORT     I2S_NUM_1

// ========== MICROPHONE ==========
#define MIC_NOISE_THRESHOLD   800
#define MIC_LOUD_THRESHOLD    3000
#define MIC_CLAP_THRESHOLD    5000
#define MIC_CLAP_WINDOW       500

// ========== TIMING ==========
#define BLINK_INTERVAL   4000
#define BLINK_DURATION   150
#define IDLE_TIMEOUT     30000

// ========== SD CARD ==========
#define SD_MAX_PATH      64
#define BMP_HEADER_SIZE  62

// ========== OPERATING MODES ==========
enum RobotMode {
    MODE_FACE,
    MODE_MENU,
    MODE_GAME_DINO,
    MODE_GAME_PONG,
    MODE_GAME_SIMON,
    MODE_AI_CHAT,
    MODE_MIC_VISUALIZER,
    MODE_MAGIC_8BALL,
    MODE_IMAGE_VIEWER,
    MODE_BLUETOOTH,
    MODE_WIFI,
    MODE_SETTINGS
};

// ========== EMOTIONS (10 total) ==========
enum Emotion {
    EMOTION_HAPPY,
    EMOTION_SAD,
    EMOTION_ANGRY,
    EMOTION_SURPRISED,
    EMOTION_SLEEPY,
    EMOTION_LOVE,
    EMOTION_COOL,
    EMOTION_CONFUSED,
    EMOTION_EXCITED,
    EMOTION_SHY
};

#define TOTAL_EMOTIONS 10

#endif