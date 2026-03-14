#include "touch_handler.h"

TouchHandler::TouchHandler(int touchPin) : pin(touchPin) {
    lastState = false;
    currentState = false;
    lastTouchTime = 0;
    touchStartTime = 0;
    tapCount = 0;
    waitingForMulti = false;
    longPressDetected = false;
    multiTapWindow = 400;
    longPressTime = 1000;
}

void TouchHandler::begin() {
    pinMode(pin, INPUT);
    Serial.print("Touch sensor initialized on GPIO ");
    Serial.println(pin);
}

TouchEvent TouchHandler::update() {
    currentState = (digitalRead(pin) == HIGH);
    unsigned long now = millis();
    TouchEvent event = TOUCH_NONE;

    // Detect press start
    if (currentState && !lastState) {
        touchStartTime = now;
        longPressDetected = false;
    }

    // Detect long press while held
    if (currentState && !longPressDetected) {
        if (now - touchStartTime > longPressTime) {
            longPressDetected = true;
            event = TOUCH_LONG;
            tapCount = 0;
            waitingForMulti = false;
        }
    }

    // Detect release (short touch)
    if (!currentState && lastState) {
        if (!longPressDetected && (now - touchStartTime > 50)) {
            tapCount++;
            lastTouchTime = now;
            waitingForMulti = true;
        }
    }

    // Evaluate tap count after time window expires
    if (waitingForMulti && (now - lastTouchTime > multiTapWindow)) {
        if (tapCount >= 3) {
            event = TOUCH_TRIPLE;
        } else if (tapCount == 2) {
            event = TOUCH_DOUBLE;
        } else if (tapCount == 1) {
            event = TOUCH_SINGLE;
        }
        tapCount = 0;
        waitingForMulti = false;
    }

    lastState = currentState;
    return event;
}

bool TouchHandler::isTouched() {
    return currentState;
}

unsigned long TouchHandler::holdDuration() {
    if (currentState) {
        return millis() - touchStartTime;
    }
    return 0;
}