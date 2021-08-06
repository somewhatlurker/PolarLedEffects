#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "PolarLedEffect.h"

class CircleZoom : public PolarLedEffect {
private:

public:
    using PolarLedEffect::PolarLedEffect;
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        SingleColourPolarData *d = (SingleColourPolarData *)data;
        
        float t = easedTime();
        unsigned int endRing = d->firstRing + (unsigned int)(t * (d->lastRing - d->firstRing));

        if (!d->transparentBg)
            fillAllInDistance(leds, led_count, d->bgColour, endRing+1, d->lastRing);
        fillAllInDistance(leds, led_count, d->colour, d->firstRing, endRing);
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
};
