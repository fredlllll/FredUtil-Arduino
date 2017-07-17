#ifndef FREDTIMERTIMEOUT_H
#define FREDTIMERTIMEOUT_H
struct FredTimerTimeout {
    unsigned long duration;
    unsigned long lastOccured;

    FredTimerTimeout(uint32_t duration) :
            duration(duration), lastOccured(millis()) {
    }
    void occur() {
        lastOccured = millis();
    }
    bool isTimeout() {
        return (millis() - lastOccured) > duration;
    }
};
#endif
