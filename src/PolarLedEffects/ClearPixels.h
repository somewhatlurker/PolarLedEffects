#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "LedEffect.h"

class ClearPixels : public LedEffect {
private:

public:
    using LedEffect::LedEffect;
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        SingleColourData *d = (SingleColourData *)data;

        for (unsigned int i = 0; i < led_count; i++)
            leds[i] = d->colour;
        
        return true;
    }
};
