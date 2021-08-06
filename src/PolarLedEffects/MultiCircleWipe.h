#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "PolarLedEffect.h"

class MultiCircleWipe : public PolarLedEffect {
private:

public:
    using PolarLedEffect::PolarLedEffect;
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        MultiColourPolarData *d = (MultiColourPolarData *)data;
        
        float t = easedTime();
        unsigned int totalDegrees = (unsigned int)(t * 360.0f);

        if (!d->transparentBg)
            fillAllInArcAndDistance(leds, led_count, d->bgColour, totalDegrees, 360, d->firstRing, d->lastRing);
        for (unsigned int i = 0; i < d->nColours; i++) {
            fillAllInArcAndDistance(leds, led_count, d->colours[i], (totalDegrees * i) / d->nColours, (totalDegrees * (i+1)) / d->nColours, d->firstRing, d->lastRing);
        }
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
};
