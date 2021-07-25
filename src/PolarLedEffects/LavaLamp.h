#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "PolarLedEffect.h"

class LavaLamp : public PolarLedEffect {
public:
    enum LavaLampColourMode {
        LavaLampColourModeRGB,
        LavaLampColourModeHSVRainbow,
        LavaLampColourModeHSVSpectrum
    };

    template<typename COLOUR_T = CRGB>
    struct LavaLampPolarData {
        LavaLampColourMode colourMode;
        COLOUR_T colours[2];
        byte lavaThreshold;
        unsigned int firstRing;
        unsigned int lastRing;
    };

private:
    // don't just use polar data because we want to add time
    template<typename COLOUR_T = CRGB>
    struct SamplerData {
        LavaLampPolarData<COLOUR_T> *lavaLampData;
        unsigned int startTime;
    };

    static CRGB LavaLampSampler(unsigned int deg, unsigned int ring, void *data) {
        SamplerData<> *sd = (SamplerData<>*)data;

        unsigned int scale = 16000 / sd->lavaLampData->lastRing;
        unsigned int *xy = polar_to_xy(deg, (int)(ring*scale));

        byte noiseSample = inoise8(xy[0], xy[1], sd->startTime);

        CRGB out;

        if (sd->lavaLampData->colourMode == LavaLampColourModeRGB) {
            LavaLampPolarData<CRGB> *lld = (LavaLampPolarData<CRGB>*)sd->lavaLampData;
            if (noiseSample < lld->lavaThreshold) {
                out = lld->colours[1];
            }
            else {
                out.r = Mix(lld->colours[1].r, lld->colours[0].r, noiseSample, 255);
                out.g = Mix(lld->colours[1].g, lld->colours[0].g, noiseSample, 255);
                out.b = Mix(lld->colours[1].b, lld->colours[0].b, noiseSample, 255);
            }
        }
        else {
            LavaLampPolarData<CHSV> *lld = (LavaLampPolarData<CHSV>*)sd->lavaLampData;
            if (noiseSample < lld->lavaThreshold) {
                out = lld->colours[1];
            }
            else {
                CHSV out_hsv;
                out_hsv.hue = MixCirc(lld->colours[1].hue, lld->colours[0].hue, noiseSample, 255);
                out_hsv.sat = Mix(lld->colours[1].sat, lld->colours[0].sat, noiseSample, 255);
                out_hsv.val = Mix(lld->colours[1].val, lld->colours[0].val, noiseSample, 255);
            }
        }

        return out;
    }

public:
    using PolarLedEffect::PolarLedEffect;
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        LavaLampPolarData<> *d = (LavaLampPolarData<>*)data;
        
        float t = easedTime();
        unsigned int startTime = (unsigned int)(t * 150.0f);

        SamplerData<> s;
        s.startTime = LedEffect::start_time + startTime;
        s.lavaLampData = d;

        sampleInDistance(leds, led_count, LavaLampSampler, &s, d->firstRing, d->lastRing);
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
};
