#include "audio_input.h"

AudioInput::AudioInput() {
    currentLevel = 0;
    peakLevel = 0;
    initialized = false;
    lastClapTime = 0;
    clapCount = 0;
    clapDetected = false;
    historyIndex = 0;
    memset(history, 0, sizeof(history));
}

void AudioInput::begin() {
    i2s_config_t cfg = {};
    cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX);
    cfg.sample_rate = I2S_SAMPLE_RATE;
    cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    cfg.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
    cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
    cfg.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
    cfg.dma_buf_count = 4;
    cfg.dma_buf_len = 64;
    cfg.use_apll = false;
    cfg.tx_desc_auto_clear = false;

    i2s_pin_config_t pins = {};
    pins.bck_io_num = PIN_I2S_BCLK;
    pins.ws_io_num = PIN_I2S_LRCLK;
    pins.data_out_num = I2S_PIN_NO_CHANGE;
    pins.data_in_num = PIN_MIC_SD;

    if (i2s_driver_install(I2S_MIC_PORT, &cfg, 0, NULL) == ESP_OK &&
        i2s_set_pin(I2S_MIC_PORT, &pins) == ESP_OK) {
        initialized = true;
        Serial.println("INMP441 Microphone initialized (I2S)");
    } else {
        Serial.println("INMP441 Microphone initialization FAILED");
        initialized = false;
    }
}

void AudioInput::update() {
    if (!initialized) return;

    size_t bytesRead = 0;
    i2s_read(I2S_MIC_PORT, readBuf, sizeof(readBuf), &bytesRead, 10);

    if (bytesRead > 0) {
        int samples = bytesRead / sizeof(int16_t);
        long sum = 0;

        for (int i = 0; i < samples; i++) {
            sum += abs(readBuf[i]);
        }

        currentLevel = sum / samples;

        // Store in history
        history[historyIndex] = currentLevel;
        historyIndex = (historyIndex + 1) % HISTORY_SIZE;

        // Update peak
        if (currentLevel > peakLevel) {
            peakLevel = currentLevel;
        }
        peakLevel = (int)(peakLevel * 0.995);  // Decay peak

        // Clap detection
        unsigned long now = millis();
        clapDetected = false;

        if (currentLevel > MIC_CLAP_THRESHOLD) {
            if (now - lastClapTime > 150) {
                clapCount++;
                lastClapTime = now;
                clapDetected = true;
            }
        }

        if (now - lastClapTime > MIC_CLAP_WINDOW) {
            clapCount = 0;
        }
    }
}

int AudioInput::getLevel() {
    return currentLevel;
}

int AudioInput::getPeak() {
    return peakLevel;
}

bool AudioInput::isLoud() {
    return currentLevel > MIC_LOUD_THRESHOLD;
}

bool AudioInput::hasSound() {
    return currentLevel > MIC_NOISE_THRESHOLD;
}

bool AudioInput::wasClap() {
    return clapDetected;
}

int AudioInput::getClapCount() {
    return clapCount;
}

int AudioInput::getHistory(int index) {
    int i = (historyIndex - 1 - index + HISTORY_SIZE) % HISTORY_SIZE;
    return history[i];
}

int AudioInput::getHistorySize() {
    return HISTORY_SIZE;
}

int AudioInput::mapLevel(int minOut, int maxOut) {
    return constrain(map(currentLevel, 0, 8000, minOut, maxOut), minOut, maxOut);
}