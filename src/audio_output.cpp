#include "audio_output.h"

AudioOutput::AudioOutput() : initialized(false), soundEnabled(true) {}

void AudioOutput::writeTone(int frequency, int durationMs) {
    if (!soundEnabled || !initialized) return;

    int sampleCount = (I2S_SAMPLE_RATE * durationMs) / 1000;
    size_t bytesWritten;
    float phase = 0.0;
    float phaseInc = (2.0 * PI * frequency) / I2S_SAMPLE_RATE;

    for (int i = 0; i < sampleCount; i++) {
        int16_t sample = (int16_t)(16000 * sin(phase));
        phase += phaseInc;
        if (phase > 2.0 * PI) phase -= 2.0 * PI;
        
        int16_t stereo[2] = {sample, sample};
        i2s_write(I2S_SPK_PORT, stereo, sizeof(stereo), &bytesWritten, portMAX_DELAY);
    }

    // Anti-click silence
    int16_t silence[2] = {0, 0};
    for (int i = 0; i < 300; i++) {
        i2s_write(I2S_SPK_PORT, silence, sizeof(silence), &bytesWritten, portMAX_DELAY);
    }
}

void AudioOutput::writeSweep(int freqStart, int freqEnd, int durationMs) {
    if (!soundEnabled || !initialized) return;

    int sampleCount = (I2S_SAMPLE_RATE * durationMs) / 1000;
    size_t bytesWritten;
    float phase = 0.0;

    for (int i = 0; i < sampleCount; i++) {
        float progress = (float)i / sampleCount;
        float freq = freqStart + (freqEnd - freqStart) * progress;
        phase += (2.0 * PI * freq) / I2S_SAMPLE_RATE;
        if (phase > 2.0 * PI) phase -= 2.0 * PI;
        
        int16_t sample = (int16_t)(14000 * sin(phase));
        int16_t stereo[2] = {sample, sample};
        i2s_write(I2S_SPK_PORT, stereo, sizeof(stereo), &bytesWritten, portMAX_DELAY);
    }
}

void AudioOutput::begin() {
    // MAX98357A SD pin wired directly to 3V3 - always enabled
    // No GPIO control needed
    
    i2s_config_t cfg = {};
    cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
    cfg.sample_rate = I2S_SAMPLE_RATE;
    cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    cfg.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
    cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
    cfg.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
    cfg.dma_buf_count = 8;
    cfg.dma_buf_len = 64;
    cfg.use_apll = false;
    cfg.tx_desc_auto_clear = true;

    i2s_pin_config_t pins = {};
    pins.bck_io_num = PIN_I2S_BCLK;
    pins.ws_io_num = PIN_I2S_LRCLK;
    pins.data_out_num = PIN_I2S_DOUT;
    pins.data_in_num = I2S_PIN_NO_CHANGE;

    if (i2s_driver_install(I2S_SPK_PORT, &cfg, 0, NULL) == ESP_OK &&
        i2s_set_pin(I2S_SPK_PORT, &pins) == ESP_OK) {
        initialized = true;
        Serial.println("MAX98357A Amplifier initialized (I2S, SD→3V3)");
    } else {
        Serial.println("MAX98357A Amplifier initialization FAILED");
        initialized = false;
    }
}

void AudioOutput::toggleSound() {
    soundEnabled = !soundEnabled;
    Serial.print("Sound: ");
    Serial.println(soundEnabled ? "ON" : "OFF");
}

bool AudioOutput::isSoundOn() {
    return soundEnabled;
}

// UI Sounds
void AudioOutput::playBootSound() {
    writeTone(523, 80);
    writeTone(659, 80);
    writeTone(784, 80);
    writeTone(1047, 150);
}

void AudioOutput::playTouchSound() {
    writeTone(1200, 40);
}

void AudioOutput::playDoubleTapSound() {
    writeTone(800, 40);
    writeTone(1200, 40);
}

void AudioOutput::playTripleTapSound() {
    writeTone(600, 30);
    writeTone(900, 30);
    writeTone(1200, 30);
}

void AudioOutput::playLongPressSound() {
    writeTone(600, 150);
}

void AudioOutput::playMenuSound() {
    writeTone(900, 25);
}

// Emotion Sounds
void AudioOutput::playHappySound() {
    writeTone(784, 60);
    writeTone(988, 60);
    writeTone(1175, 100);
}

void AudioOutput::playSadSound() {
    writeTone(400, 150);
    writeTone(350, 150);
    writeTone(300, 200);
}

void AudioOutput::playAngrySound() {
    writeTone(200, 100);
    writeTone(250, 100);
    writeTone(200, 150);
}

void AudioOutput::playSurprisedSound() {
    writeSweep(400, 1200, 200);
}

void AudioOutput::playSleepSound() {
    writeSweep(600, 200, 400);
}

void AudioOutput::playLoveSound() {
    writeTone(659, 80);
    writeTone(784, 80);
    writeTone(1047, 120);
    writeTone(784, 60);
    writeTone(1047, 150);
}

void AudioOutput::playCoolSound() {
    writeTone(440, 80);
    writeTone(554, 80);
    writeTone(659, 120);
}

void AudioOutput::playConfusedSound() {
    writeTone(500, 60);
    writeTone(400, 60);
    writeTone(500, 60);
    writeTone(400, 80);
}

void AudioOutput::playExcitedSound() {
    writeSweep(500, 1500, 120);
    writeSweep(500, 1500, 120);
}

void AudioOutput::playShySound() {
    writeTone(800, 40);
    writeTone(600, 60);
}

// System Sounds
void AudioOutput::playWakeUp() {
    writeSweep(200, 800, 300);
}

void AudioOutput::playSoundReaction() {
    writeTone(600, 35);
    writeTone(900, 35);
    writeTone(600, 35);
}

// Game Sounds
void AudioOutput::playGameOverSound() {
    writeTone(500, 120);
    writeTone(400, 120);
    writeTone(300, 250);
}

void AudioOutput::playScoreSound() {
    writeTone(1000, 40);
    writeTone(1500, 60);
}

void AudioOutput::playWinSound() {
    writeTone(523, 60);
    writeTone(659, 60);
    writeTone(784, 60);
    writeTone(1047, 60);
    writeTone(1319, 150);
}

void AudioOutput::playSimonTone(int index) {
    int freqs[] = {262, 330, 392, 523};
    if (index >= 0 && index < 4) {
        writeTone(freqs[index], 250);
    }
}

// Feature Sounds
void AudioOutput::playMagic8Ball() {
    writeSweep(300, 800, 400);
}