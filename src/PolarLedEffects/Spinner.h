#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "PolarLedEffect.h"

class Spinner : public PolarLedEffect {
private:

public:
    using PolarLedEffect::PolarLedEffect;
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        MultiColourPolarData *d = (MultiColourPolarData *)data;
        
        float t = easedTime();
        unsigned int startDegrees = (unsigned int)(t * 360.0f);

        for (unsigned int i = 0; i < d->nColours; i++) {
            unsigned int arcStart = startDegrees + (360 * i) / d->nColours;
            unsigned int arcEnd = startDegrees + (360 * (i+1)) / d->nColours;
            arcStart %= 360;
            arcEnd %= 360;
            fillAllInArcAndDistance(leds, led_count, d->colours[i], arcStart, arcEnd, d->firstRing, d->lastRing);
        }
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
};
