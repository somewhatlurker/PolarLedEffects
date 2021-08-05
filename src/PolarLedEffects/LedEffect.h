#pragma once

#include <Arduino.h>
#include <FastLED.h>

class LedEffect {
public:
    struct SingleColourData {
        CRGB colour;
    };

    struct MultiColourData {
        unsigned int nColours;
        CRGB *colours;
    };

private:
    int duration;
    unsigned int ease_factor; // 0 for linear, 1 for quad, 2 for cubic, etc
    
protected:
    int start_time;

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


    static inline byte Mix(byte one, byte two, unsigned int factor, unsigned int scale) {
        return ((uint32_t)one * (scale - factor) + (uint32_t)two * factor) / scale;
    }

    static inline byte MixCirc(byte one, byte two, unsigned int factor, unsigned int scale) {
        bool twoLarger = two > one;
        bool swap = twoLarger ? two - one > 128 : one - two > 128;
        if (swap) {
            if (twoLarger) {
                return (((uint32_t)one + 256) * (scale - factor) + (uint32_t)two * factor) / scale;
            }
            else {
                return ((uint32_t)one * (scale - factor) + ((uint32_t)two + 256) * factor) / scale;
            }
        }
        else {
            return ((uint32_t)one * (scale - factor) + (uint32_t)two * factor) / scale;
        }
    }

public:
    LedEffect(int duration_ms = 1000, unsigned int easing = 1)
        : duration{duration_ms}, ease_factor{easing} {}
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        float t = time();
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
    
    inline void MarkStartTime(int t) {
        start_time = t;
    }
    
    inline void MarkStartTime() {
        MarkStartTime(millis());
    }
};
