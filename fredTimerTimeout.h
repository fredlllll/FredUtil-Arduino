#ifndef FREDTIMERTIMEOUT
#define FREDTIMERTIMEOUT
struct TimerTimeout{
    uint32_t lastOccured;
	uint32_t duration;
    FredTimerTimeout(uint32_t duration):duration(duration){}
    void occur(){
        lastOccured = millis();
    }
    bool isTimeout(){
        return (millis()-lastOccured) > duration;
    }
};
#endif