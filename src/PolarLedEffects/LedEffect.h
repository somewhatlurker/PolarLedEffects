#pragma once

#include <Arduino.h>
#include <FastLED.h>

class LedEffect {
private:
    int duration;
    unsigned int ease_factor; // 0 for linear, 1 for quad, 2 for cubic, etc
    int start_time;
    
protected:
    float time(int millis) {
        return (float)(millis - start_time) / (float)duration;
    }
    
    float time() {
        return time(millis());
    }
    
    float easedTime(float t) {
        if (t <= 0.5f) {
           return pow(t, (float)ease_factor+1.0f) * pow(2.0f, (float)ease_factor);
        }
        else if (t < 1.0f) {
            return 1.0f - (pow(1.0f-t, (float)ease_factor+1.0f) * pow(2.0f, (float)ease_factor));
        }
        else {
            return 1.0f;
        }
    }
    
    float easedTime() {
        return easedTime(time());
    }

public:
    LedEffect(int duration_ms = 1000, unsigned int easing = 1)
        : duration{duration_ms}, ease_factor{easing} {}
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        return true;
    }
    
    inline void MarkStartTime(int t) {
        start_time = t;
    }
    
    inline void MarkStartTime() {
        MarkStartTime(millis());
    }

    struct SingleColourData {
        CHSV colour;
    };
    
    struct MultiColourData {
        unsigned int nColours;
        CHSV *colours;
    };
};
