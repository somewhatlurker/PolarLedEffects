#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "PolarLedEffect.h"

class CircleZoomReversed : public PolarLedEffect {
private:

public:
    using PolarLedEffect::PolarLedEffect;
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        SingleColourPolarData *d = (SingleColourPolarData *)data;
        
        float t = easedTime();
        unsigned int endRing = d->lastRing - (unsigned int)(t * (d->lastRing - d->firstRing));

        if (!d->transparentBg)
            fillAllInDistance(leds, led_count, d->bgColour, 0, endRing-1);
        fillAllInDistance(leds, led_count, d->colour, endRing, d->lastRing);
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
};
