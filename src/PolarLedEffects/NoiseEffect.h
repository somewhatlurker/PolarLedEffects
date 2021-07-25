#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "PolarLedEffect.h"

class NoiseEffect : public PolarLedEffect {
public:
    enum NoiseEffectColourMode {
        NoiseEffectColourModeRGB,
        NoiseEffectColourModeHSVRainbow,
        NoiseEffectColourModeHSVSpectrum
    };

    template<typename COLOUR_T = CRGB>
    struct NoiseEffectPolarData {
        NoiseEffectColourMode colourMode;
        COLOUR_T colours[2];
        byte lavaThreshold;
        unsigned int speedMultiplier;
        unsigned int firstRing;
        unsigned int lastRing;
    };

private:
    // don't just use polar data because we want to add time
    template<typename COLOUR_T = CRGB>
    struct SamplerData {
        NoiseEffectPolarData<COLOUR_T> *NoiseEffectData;
        unsigned int startTime;
    };

    static CRGB NoiseEffectSampler(unsigned int deg, unsigned int ring, void *data) {
        SamplerData<> *sd = (SamplerData<>*)data;

        unsigned int scale = 16000 / sd->NoiseEffectData->lastRing;
        unsigned int *xy = polar_to_xy(deg, (int)(ring*scale));

        byte noiseSample = inoise8(xy[0], xy[1], sd->startTime);

        CRGB out;

        if (sd->NoiseEffectData->colourMode == NoiseEffectColourModeRGB) {
            NoiseEffectPolarData<CRGB> *nd = (NoiseEffectPolarData<CRGB>*)sd->NoiseEffectData;
            if (noiseSample < nd->lavaThreshold) {
                out = nd->colours[1];
            }
            else {
                out.r = Mix(nd->colours[1].r, nd->colours[0].r, noiseSample, 255);
                out.g = Mix(nd->colours[1].g, nd->colours[0].g, noiseSample, 255);
                out.b = Mix(nd->colours[1].b, nd->colours[0].b, noiseSample, 255);
            }
        }
        else {
            NoiseEffectPolarData<CHSV> *nd = (NoiseEffectPolarData<CHSV>*)sd->NoiseEffectData;
            if (noiseSample < nd->lavaThreshold) {
                out = nd->colours[1];
            }
            else {
                CHSV out_hsv;
                out_hsv.hue = MixCirc(nd->colours[1].hue, nd->colours[0].hue, noiseSample, 255);
                out_hsv.sat = Mix(nd->colours[1].sat, nd->colours[0].sat, noiseSample, 255);
                out_hsv.val = Mix(nd->colours[1].val, nd->colours[0].val, noiseSample, 255);
            }
        }

        return out;
    }

public:
    using PolarLedEffect::PolarLedEffect;
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        NoiseEffectPolarData<> *d = (NoiseEffectPolarData<>*)data;
        
        float t = easedTime();
        unsigned int startTime = (unsigned int)(t * d->speedMultiplier);

        SamplerData<> s;
        s.startTime = LedEffect::start_time + startTime;
        s.NoiseEffectData = d;

        sampleInDistance(leds, led_count, NoiseEffectSampler, &s, d->firstRing, d->lastRing);
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
};
