#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "PolarLedEffect.h"
#include "font_5x5.h"

class TextEffect : public PolarLedEffect {
public:
    struct TextEffectPolarData {
        unsigned int nChars;
        const char *chars;
        CRGB colours[2];
        bool transparentBg;
        unsigned int firstRing;
        unsigned int lastRing;
    };

private:
    // don't just use polar data because we want to add time
    struct SamplerData {
        TextEffectPolarData *TextEffectData;
        unsigned int leftOffset;
    };

    static CRGB TextEffectSampler(unsigned int deg, unsigned int ring, CRGB old_colour, void *data) {
        SamplerData *sd = (SamplerData*)data;

        CRGB fg_col = sd->TextEffectData->colours[0];

        CRGB bg_col = old_colour;
        if (!sd->TextEffectData->transparentBg)
            bg_col = sd->TextEffectData->colours[1];

        int *xy = polar_to_xy(deg, (int)ring);

        int scale = 3 / sd->TextEffectData->lastRing;
        xy[0] = xy[0] * scale;
        xy[1] = xy[1] * scale;

        // make y=0 top pixel of chars
        xy[1] += 2;

        int c_row = xy[1];
        if (c_row < 0 || c_row > 4) return bg_col;

        unsigned int c_idx = (xy[0] - sd->leftOffset) / 5;
        if (c_idx >= sd->TextEffectData->nChars) return bg_col;

        uint8_t c_char = sd->TextEffectData->chars[c_idx];
        if (c_char < 32 || c_char > 126) return bg_col;
        c_char -= 32;

        // const uint8_t *c_bitmap = char_5x5_G;
        const uint8_t *c_bitmap = font_5x5_ascii_map[c_char];

        int c_col = 4 - ((xy[0] - sd->leftOffset) % 5);
        // if (c_col < 0 || c_col > 4) return bg_col;

        if (bitRead(c_bitmap[c_row], c_col))
            return fg_col;
        else
            return bg_col;
    }

public:
    using PolarLedEffect::PolarLedEffect;
    
    // returns true when effect is complete (false while still running)
    virtual bool UpdateLeds(CRGB *leds, unsigned int led_count, void *data) {
        TextEffectPolarData *d = (TextEffectPolarData*)data;
        
        float t = easedTime();

        int text_len_pixels = 5 * d->nChars;
        int fix_end_scroll_pos = d->nChars > 1 ? 1 : 0;
        //int leftOffset = d->lastRing - t * (text_len_pixels + 2*d->lastRing);
        int leftOffset = -2 - t * (text_len_pixels - 5 + fix_end_scroll_pos);

        SamplerData s;
        s.leftOffset = leftOffset;
        s.TextEffectData = d;

        sampleInDistance(leds, led_count, TextEffectSampler, &s, d->firstRing, d->lastRing);
        
        if (t < 1.0f)
            return false;
        else
            return true;
    }
};
