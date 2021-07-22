#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "PolarLedEffect.h"

class Gradient : public PolarLedEffect {
private:
    struct SamplerData {
        unsigned int nPoles;
        // [0..nPoles-1] outer hues
        // [nPoles..2*nPoles-1] inner hues
        CHSV *cols;
        unsigned int firstRing;
        unsigned int lastRing;
        unsigned int startDegrees;
    };

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

    static CRGB GradientSampler(unsigned int deg, unsigned int ring, void *data) {
        SamplerData *d = (SamplerData*)data;

        deg = (deg - d->startDegrees) % 360;
        //while ((int)deg < 0) deg += 360;

        unsigned int poleSize = 360 / (d->nPoles - 1);
        unsigned int pole = deg / poleSize;
        if (pole >= d->nPoles) pole = d->nPoles - 1;
        unsigned int poleDeg = deg - (pole * poleSize);

        byte hue_outer = MixCirc(d->cols[0].hue, d->cols[1].hue, poleDeg, poleSize);
        byte sat_outer = Mix(d->cols[0].sat, d->cols[1].sat, poleDeg, poleSize);
        byte val_outer = Mix(d->cols[0].val, d->cols[1].val, poleDeg, poleSize);
        
        byte hue_inner = MixCirc(d->cols[2].hue, d->cols[3].hue, poleDeg, poleSize);
        byte sat_inner = Mix(d->cols[2].sat, d->cols[3].sat, poleDeg, poleSize);
        byte val_inner = Mix(d->cols[2].val, d->cols[3].val, poleDeg, poleSize);

        CHSV out;
        out.hue = MixCirc(hue_inner, hue_outer, ring - d->firstRing, d->lastRing - d->firstRing);
        out.sat = Mix(sat_inner, sat_outer, ring - d->firstRing, d->lastRing - d->firstRing);
        out.val = Mix(val_inner, val_outer, ring - d->firstRing, d->lastRing - d->firstRing);

        return out;
    }

public:
    using PolarLedEffect::PolarLedEffect;

    struct GradientPolarData {
        unsigned int nPoles;
        unsigned int nColours;
        CHSV *colours;
        unsigned int firstRing;
        unsigned int lastRing;
    };
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        GradientPolarData *d = (GradientPolarData *)data;
        
        float t = easedTime();
        unsigned int startDegrees = (unsigned int)(t * 360.0f);

        if (d->nColours < d->nPoles) return true;

        SamplerData s;

        s.nPoles = d->nPoles;
        s.cols = (CHSV*)malloc(s.nPoles * 2 * sizeof(CHSV));
        if (s.cols == nullptr) return true;

        for (unsigned int i = 0; i < s.nPoles; i++) {
            s.cols[i] = d->colours[i];
        }
        if (d->nColours >= s.nPoles * 2) {
            for (unsigned int i = 0; i < s.nPoles; i++) {
                s.cols[s.nPoles+i] = d->colours[s.nPoles+i];
            }
        }
        else if (d->nColours > s.nPoles) {
            for (unsigned int i = 0; i < s.nPoles; i++) {
                s.cols[s.nPoles+i] = d->colours[s.nPoles];
            }
        }
        else {
            for (unsigned int i = 0; i < s.nPoles; i++) {
                s.cols[s.nPoles+i] = d->colours[i];
            }
        }
        s.firstRing = d->firstRing;
        s.lastRing = d->lastRing;
        s.startDegrees = startDegrees;

        sampleInDistance(leds, led_count, GradientSampler, &s, d->firstRing, d->lastRing);

        free(s.cols);
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
};
