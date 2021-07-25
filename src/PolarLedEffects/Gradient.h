#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "PolarLedEffect.h"

class Gradient : public PolarLedEffect {
public:
    enum GradientColourMode {
        GradientColourModeRGB,
        GradientColourModeHSVRainbow,
        GradientColourModeHSVSpectrum
    };

    template<typename COLOUR_T = CRGB>
    struct GradientPolarData {
        unsigned int nPoles;
        GradientColourMode colourMode;
        unsigned int nColours;
        COLOUR_T *colours;
        unsigned int firstRing;
        unsigned int lastRing;
    };

private:
    // sampler data is distinct from the effect data so that it can
    // be guaranteed that there's always two colours per pole
    // (input data only strictly requires one)
    struct SamplerData {
        unsigned int nPoles;
        GradientColourMode colourMode;
        // [0..nPoles-1] outer hues
        // [nPoles..2*nPoles-1] inner hues
        union rgbhsv_ptr {
            CRGB *rgb;
            CHSV *hsv;
        } cols;
        unsigned int firstRing;
        unsigned int lastRing;
        unsigned int startDegrees;
    };

    static CRGB GradientSampler(unsigned int deg, unsigned int ring, void *data) {
        SamplerData *d = (SamplerData*)data;

        deg = (deg - d->startDegrees) % 360;
        //while ((int)deg < 0) deg += 360;

        unsigned int poleSize = 360 / (d->nPoles - 1);
        unsigned int pole = deg / poleSize;
        if (pole >= d->nPoles) pole = d->nPoles - 1;
        unsigned int poleDeg = deg - (pole * poleSize);

        CRGB out;

        if (d->colourMode == GradientColourModeRGB) {
            byte r_outer = Mix(d->cols.rgb[0].r, d->cols.rgb[1].r, poleDeg, poleSize);
            byte g_outer = Mix(d->cols.rgb[0].g, d->cols.rgb[1].g, poleDeg, poleSize);
            byte b_outer = Mix(d->cols.rgb[0].b, d->cols.rgb[1].b, poleDeg, poleSize);

            byte r_inner = Mix(d->cols.rgb[2].r, d->cols.rgb[3].r, poleDeg, poleSize);
            byte g_inner = Mix(d->cols.rgb[2].g, d->cols.rgb[3].g, poleDeg, poleSize);
            byte b_inner = Mix(d->cols.rgb[2].b, d->cols.rgb[3].b, poleDeg, poleSize);

            out.r = Mix(r_inner, r_outer, ring - d->firstRing, d->lastRing - d->firstRing);
            out.g = Mix(g_inner, g_outer, ring - d->firstRing, d->lastRing - d->firstRing);
            out.b = Mix(b_inner, b_outer, ring - d->firstRing, d->lastRing - d->firstRing);
        }
        else {
            byte hue_outer = MixCirc(d->cols.hsv[0].hue, d->cols.hsv[1].hue, poleDeg, poleSize);
            byte sat_outer = Mix(d->cols.hsv[0].sat, d->cols.hsv[1].sat, poleDeg, poleSize);
            byte val_outer = Mix(d->cols.hsv[0].val, d->cols.hsv[1].val, poleDeg, poleSize);

            byte hue_inner = MixCirc(d->cols.hsv[2].hue, d->cols.hsv[3].hue, poleDeg, poleSize);
            byte sat_inner = Mix(d->cols.hsv[2].sat, d->cols.hsv[3].sat, poleDeg, poleSize);
            byte val_inner = Mix(d->cols.hsv[2].val, d->cols.hsv[3].val, poleDeg, poleSize);

            CHSV out_hsv;
            out_hsv.hue = MixCirc(hue_inner, hue_outer, ring - d->firstRing, d->lastRing - d->firstRing);
            out_hsv.sat = Mix(sat_inner, sat_outer, ring - d->firstRing, d->lastRing - d->firstRing);
            out_hsv.val = Mix(val_inner, val_outer, ring - d->firstRing, d->lastRing - d->firstRing);

            if (d->colourMode == Gradient::GradientColourModeHSVRainbow) {
                hsv2rgb_rainbow(out_hsv, out);
            }
            else {
                hsv2rgb_spectrum(out_hsv, out);
            }
        }

        return out;
    }

public:
    using PolarLedEffect::PolarLedEffect;
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        GradientPolarData<> *d = (GradientPolarData<> *)data;
        
        float t = easedTime();
        unsigned int startDegrees = (unsigned int)(t * 360.0f);

        if (d->nColours < d->nPoles) return true;

        SamplerData s;

        s.nPoles = d->nPoles;
        s.colourMode = d->colourMode;

        if (s.colourMode == GradientColourModeRGB) {
            s.cols.rgb = (CRGB*)malloc(s.nPoles * 2 * sizeof(CRGB));
            if (s.cols.rgb == nullptr) return true;

            for (unsigned int i = 0; i < s.nPoles; i++) {
                s.cols.rgb[i] = d->colours[i];
            }
            if (d->nColours >= s.nPoles * 2) {
                for (unsigned int i = 0; i < s.nPoles; i++) {
                    s.cols.rgb[s.nPoles+i] = d->colours[s.nPoles+i];
                }
            }
            else if (d->nColours > s.nPoles) {
                for (unsigned int i = 0; i < s.nPoles; i++) {
                    s.cols.rgb[s.nPoles+i] = d->colours[s.nPoles];
                }
            }
            else {
                for (unsigned int i = 0; i < s.nPoles; i++) {
                    s.cols.rgb[s.nPoles+i] = d->colours[i];
                }
            }
        }
        else {
            s.cols.hsv = (CHSV *)malloc(s.nPoles * 2 * sizeof(CHSV));
            if (s.cols.hsv == nullptr) return true;

            GradientPolarData<CHSV>* d_hsv = (GradientPolarData<CHSV>*)d;

            for (unsigned int i = 0; i < s.nPoles; i++) {
                s.cols.hsv[i] = d_hsv->colours[i];
            }
            if (d_hsv->nColours >= s.nPoles * 2) {
                for (unsigned int i = 0; i < s.nPoles; i++) {
                    s.cols.hsv[s.nPoles+i] = d_hsv->colours[s.nPoles+i];
                }
            }
            else if (d_hsv->nColours > s.nPoles) {
                for (unsigned int i = 0; i < s.nPoles; i++) {
                    s.cols.hsv[s.nPoles+i] = d_hsv->colours[s.nPoles];
                }
            }
            else {
                for (unsigned int i = 0; i < s.nPoles; i++) {
                    s.cols.hsv[s.nPoles+i] = d_hsv->colours[i];
                }
            }
        }

        s.firstRing = d->firstRing;
        s.lastRing = d->lastRing;
        s.startDegrees = startDegrees;

        sampleInDistance(leds, led_count, GradientSampler, &s, d->firstRing, d->lastRing);

        if (d->colourMode == GradientColourModeRGB) {
            free(s.cols.rgb);
        }
        else {
            free(s.cols.hsv);
        }
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
};
