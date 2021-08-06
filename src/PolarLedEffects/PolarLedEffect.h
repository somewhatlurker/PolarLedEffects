#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "LedEffect.h"

#ifndef POLAR_AA_SAMPLES
#define POLAR_AA_SAMPLES 1
#endif

class PolarLedEffect : public LedEffect {
public:
    struct SingleColourPolarData {
        CRGB colour;
        CRGB bgColour;
        bool transparentBg;
        unsigned int firstRing;
        unsigned int lastRing;
    };

    struct MultiColourPolarData {
        unsigned int nColours;
        CRGB *colours;
        CRGB bgColour;
        bool transparentBg;
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
    
    inline bool isAngleInArc(unsigned int angle, unsigned int degStart, unsigned int degEnd) {        
        if (degEnd >= degStart)
            return (angle >= degStart) && (angle <= degEnd);
        else
            return (angle >= degStart) || (angle <= degEnd);
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
#if POLAR_AA_SAMPLES <= 1
                unsigned int ledDegrees = 360 * i / ring_size;

                if (isAngleInArc(ledDegrees, degStart, degEnd)) {
                    leds[led_n] = sampler(ledDegrees, ring_n, leds[led_n], userdata);
                }
#else
                unsigned int angleStep = (360 / POLAR_AA_SAMPLES) / ring_size;

                int ledDegrees = 360 * i / ring_size - angleStep / POLAR_AA_SAMPLES;

                unsigned int r_total = 0;
                unsigned int g_total = 0;
                unsigned int b_total = 0;

                for (int j = 0; j < POLAR_AA_SAMPLES; j++) {
                  if (isAngleInArc(ledDegrees + j*angleStep, degStart, degEnd)) {
                      int splDegrees = ledDegrees + j*angleStep;
                      if (splDegrees < 0) splDegrees += 360;

                      CRGB res = sampler(splDegrees, ring_n, leds[led_n], userdata);
                      r_total += res.r;
                      g_total += res.g;
                      b_total += res.b;
                  }
                  else {
                      r_total += leds[led_n].r;
                      g_total += leds[led_n].g;
                      b_total += leds[led_n].b;
                  }
                }

                leds[led_n] = CRGB(r_total/POLAR_AA_SAMPLES, g_total/POLAR_AA_SAMPLES, b_total/POLAR_AA_SAMPLES);
#endif
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
