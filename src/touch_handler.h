#ifndef TOUCH_HANDLER_H
#define TOUCH_HANDLER_H

#include <Arduino.h>
#include "config.h"

enum TouchEvent {
    TOUCH_NONE,
    TOUCH_SINGLE,
    TOUCH_DOUBLE,
    TOUCH_TRIPLE,
    TOUCH_LONG
};

class TouchHandler {
private:
    int pin;
    bool lastState;
    bool currentState;
    unsigned long lastTouchTime;
    unsigned long touchStartTime;
    int tapCount;
    bool waitingForMulti;
    bool longPressDetected;
    unsigned long multiTapWindow;
    unsigned long longPressTime;

public:
    TouchHandler(int touchPin);
    void begin();
    TouchEvent update();
    bool isTouched();
    unsigned long holdDuration();
};

#endif