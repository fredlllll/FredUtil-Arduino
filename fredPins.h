#ifndef FREDPINS_H
#define FREDPINS_H

#include <Arduino.h>

struct DigitalPin {
    const uint8_t pin;

    inline DigitalPin(const uint8_t pin) :
            pin(pin) {
    }

    inline uint8_t getPinNumber() const {
        return pin;
    }

    inline void setInput() const {
        pinMode(pin, INPUT);
    }

    inline void setOutput() const {
        pinMode(pin, OUTPUT);
    }

    inline void setInputPullup() const {
        pinMode(pin, INPUT_PULLUP);
    }

    inline void setHigh() const {
        digitalWrite(pin, HIGH);
    }

    inline void setLow() const {
        digitalWrite(pin, LOW);
    }

    inline uint8_t digitalRead() const {
        return ::digitalRead(pin);
    }
};

struct AnalogPin {
    const uint8_t pin;

    inline AnalogPin(const uint8_t pin) :
            pin(pin) {
    }

    inline uint8_t getPinNumber() const {
        return pin;
    }

    inline int analogRead() const {
        return ::analogRead(pin);
    }
};
#endif
