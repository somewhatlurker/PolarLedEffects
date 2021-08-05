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
        bool transparentBg;
        byte displayTheshold;
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

    static CRGB NoiseEffectSampler(unsigned int deg, unsigned int ring, CRGB old_colour, void *data) {
        SamplerData<> *sd = (SamplerData<>*)data;

        int scale = 32767 / sd->NoiseEffectData->lastRing;
        int *xy = polar_to_xy(deg, (int)(ring*scale));

        byte noiseSample = inoise8(xy[0]+32768, xy[1]+32768, sd->startTime);

        CRGB out;

        if (sd->NoiseEffectData->colourMode == NoiseEffectColourModeRGB) {
            NoiseEffectPolarData<CRGB> *nd = (NoiseEffectPolarData<CRGB>*)sd->NoiseEffectData;

            CRGB fg_col = nd->colours[0];
    
            CRGB bg_col = old_colour;
            if (!nd->transparentBg)
                bg_col = nd->colours[1];
            
            if (noiseSample < nd->displayTheshold) {
                out = bg_col;
            }
            else {
                out.r = Mix(bg_col.r, fg_col.r, noiseSample, 255);
                out.g = Mix(bg_col.g, fg_col.g, noiseSample, 255);
                out.b = Mix(bg_col.b, fg_col.b, noiseSample, 255);
            }
        }
        else {
            NoiseEffectPolarData<CHSV> *nd = (NoiseEffectPolarData<CHSV>*)sd->NoiseEffectData;

            CHSV fg_col = nd->colours[0];
    
            CHSV bg_col = rgb2hsv_approximate(old_colour);
            CRGB bg_col_nomix = old_colour; // avoid round tripping BG through HSV when not blended
            if (!nd->transparentBg) {
                bg_col = nd->colours[1];
                bg_col_nomix = nd->colours[1];
            }
            
            if (noiseSample < nd->displayTheshold) {
                out = bg_col_nomix;
            }
            else {
                CHSV out_hsv;
                out_hsv.hue = MixCirc(bg_col.hue, fg_col.hue, noiseSample, 255);
                out_hsv.sat = Mix(bg_col.sat, fg_col.sat, noiseSample, 255);
                out_hsv.val = Mix(bg_col.val, fg_col.val, noiseSample, 255);
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
