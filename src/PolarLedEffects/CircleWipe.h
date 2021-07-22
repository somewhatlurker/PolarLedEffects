#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "PolarLedEffect.h"

class CircleWipe : public PolarLedEffect {
private:

public:
    using PolarLedEffect::PolarLedEffect;
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        SingleColourPolarData *d = (SingleColourPolarData *)data;
        
        float t = easedTime();
        unsigned int endDegrees = (unsigned int)(t * 360.0f);
        fillAllInArcAndDistance(leds, led_count, d->colour, 0, endDegrees, d->firstRing, d->lastRing);
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
};
