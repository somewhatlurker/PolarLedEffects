#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "LedEffect.h"

class PolarLedEffect : public LedEffect {
public:
    struct SingleColourPolarData {
        CRGB colour;
        unsigned int firstRing;
        unsigned int lastRing;
    };

    struct MultiColourPolarData {
        unsigned int nColours;
        CRGB *colours;
        unsigned int firstRing;
        unsigned int lastRing;
    };

protected:
    unsigned int led_ring_lengths[9] = {1, 8, 12, 16, 24, 32, 40, 48, 60};

    static inline int* polar_to_xy(unsigned int deg, int r) {
        static int out[2];
        out[0] = (int)round(sin16((uint32_t)deg * 65535 / 360) / 32767.f * r);
        out[1] = (int)round(0 - cos16((uint32_t)deg * 65535 / 360) / 32767.f * r);
        return out;
    }
    
    inline bool isLedInArc(unsigned int ledPos, unsigned int ledSegments, unsigned int degStart, unsigned int degEnd) {
        unsigned int ledDegrees = 360 * ledPos / ledSegments;
        
        if (degEnd >= degStart)
            return (ledDegrees >= degStart) && (ledDegrees <= degEnd);
        else
            return (ledDegrees >= degStart) || (ledDegrees <= degEnd);
    }

    typedef CRGB (*PolarSampler)(unsigned int deg, unsigned int ring, CRGB old_colour, void *data);

    // note: led_count should contain a complete ring
    inline void sampleInArcAndDistance(CRGB *leds, unsigned int led_count, PolarSampler sampler, void *userdata, 
            unsigned int degStart, unsigned int degEnd, unsigned int ringStart, unsigned int ringEnd) {
        unsigned int led_n = 0;
        for (unsigned int ring_n = FIRST_POLAR_RING; ring_n <= ringEnd; ring_n++) {
            if (led_n >= led_count) break;
            if (ring_n > LAST_POLAR_RING) break;
            if (ring_n >= sizeof(led_ring_lengths) / sizeof(led_ring_lengths[0])) break;
            
            unsigned int ring_size = led_ring_lengths[ring_n];
            
            // skip over rings until the effect starts
            if (ring_n < ringStart) {
                led_n += ring_size;
                continue;
            }
            
            // we are in the distance to be filled
            for (unsigned int i = 0; i < ring_size; i++) {
                if (isLedInArc(i, ring_size, degStart, degEnd)) {
                    leds[led_n] = sampler(360 * i / ring_size, ring_n, leds[led_n], userdata);
                }
                led_n++;
            }
        }
    }
    
    inline void sampleInArc(CRGB *leds, unsigned int led_count, PolarSampler sampler, void *userdata, 
            unsigned int degStart, unsigned int degEnd) {
        sampleInArcAndDistance(leds, led_count, sampler, userdata, degStart, degEnd, 0, 999);
    }
    
    inline void sampleInDistance(CRGB *leds, unsigned int led_count, PolarSampler sampler, void *userdata, 
            unsigned int ringStart, unsigned int ringEnd) {
        sampleInArcAndDistance(leds, led_count, sampler, userdata, 0, 360, ringStart, ringEnd);
    }


    static CRGB fillSampler(unsigned int deg, unsigned int ring, CRGB old_colour, void *data) {
        return *(CRGB*)data;
    }

    // note: led_count should contain a complete ring
    void fillAllInArcAndDistance(CRGB *leds, unsigned int led_count, CRGB colour, 
            unsigned int degStart, unsigned int degEnd, unsigned int ringStart, unsigned int ringEnd) {
        sampleInArcAndDistance(leds, led_count, fillSampler, &colour, degStart, degEnd, ringStart, ringEnd);
    }
    
    inline void fillAllInArc(CRGB *leds, unsigned int led_count, CRGB colour, 
            unsigned int degStart, unsigned int degEnd) {
        fillAllInArcAndDistance(leds, led_count, colour, degStart, degEnd, 0, 999);
    }
    
    inline void fillAllInDistance(CRGB *leds, unsigned int led_count, CRGB colour, 
            unsigned int ringStart, unsigned int ringEnd) {
        fillAllInArcAndDistance(leds, led_count, colour, 0, 360, ringStart, ringEnd);
    }

public:
    using LedEffect::LedEffect;
};
