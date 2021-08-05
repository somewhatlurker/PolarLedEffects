#define FIRST_POLAR_RING 0
#define LAST_POLAR_RING 3

#define LED_PIN 3
#define NUM_LEDS 37
#define LED_BRIGHTNESS 8
#define LED_CURRENT_LIMIT 300

#define ANIM_FPS 120
#define ANIM_FRAME_MICROS (1000000 / ANIM_FPS)

#include <FastLED.h>
#include "LedEffect.h"
#include "PolarLedEffect.h"

PolarLedEffect nullEffect = PolarLedEffect();
PolarLedEffect longNullEffect = PolarLedEffect(5000);

#include "ClearPixels.h"
ClearPixels clearPixels = ClearPixels();

#include "CircleWipe.h"
CircleWipe defaultCircleWipe = CircleWipe();
#include "CircleZoom.h"
CircleZoom defaultCircleZoom = CircleZoom();
#include "MultiCircleWipe.h"
MultiCircleWipe defaultMultiCircleWipe = MultiCircleWipe();
#include "Spinner.h"
Spinner defaultSpinner = Spinner();
#include "CircleZoomReversed.h"
CircleZoomReversed defaultCircleZoomReversed = CircleZoomReversed();
#include "CircleWipeReversed.h"
CircleWipeReversed defaultCircleWipeReversed = CircleWipeReversed();
#include "Gradient.h"
Gradient defaultGradient = Gradient(5000, 1);
#include "NoiseEffect.h"
NoiseEffect defaultNoiseEffect = NoiseEffect(5000, 1);
#include "TextEffect.h"
TextEffect defaultTextEffect = TextEffect();
TextEffect longTextEffect = TextEffect(5000, 0);

LedEffect::SingleColourData redColourData = {CRGB::Red};
LedEffect::SingleColourData greenColourData = {CRGB::Lime};
LedEffect::SingleColourData blueColourData = {CRGB::Blue};
LedEffect::SingleColourData whiteColourData = {CRGB::White};
LedEffect::SingleColourData blackColourData = {CRGB::Black};

PolarLedEffect::SingleColourPolarData redPolarData = {CRGB::Red, FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData greenPolarData = {CRGB::Lime, FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData bluePolarData = {CRGB::Blue, FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData blackPolarData = {CRGB::Black, FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData whitePolarData = {CRGB::White, FIRST_POLAR_RING, LAST_POLAR_RING};

PolarLedEffect::MultiColourPolarData rgbPolarData = {3, new CRGB[3] {CRGB::Red, CRGB::Lime, CRGB::Blue}, FIRST_POLAR_RING, LAST_POLAR_RING};

PolarLedEffect::SingleColourPolarData red1stOutsideRingPolarData = {CRGB::Red, LAST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData green2ndOutsideRingPolarData = {CRGB::Lime, LAST_POLAR_RING-1, LAST_POLAR_RING-1};
PolarLedEffect::SingleColourPolarData blue3rdOutsideRingPolarData = {CRGB::Blue, LAST_POLAR_RING-2, LAST_POLAR_RING-2};
PolarLedEffect::SingleColourPolarData white4thOutsideRingPolarData = {CRGB::White, LAST_POLAR_RING-3, LAST_POLAR_RING-3};

PolarLedEffect::SingleColourPolarData black1stOutsideRingPolarData = {CRGB::Black, LAST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData black2ndOutsideRingPolarData = {CRGB::Black, LAST_POLAR_RING-1, LAST_POLAR_RING-1};
PolarLedEffect::SingleColourPolarData black3rdOutsideRingPolarData = {CRGB::Black, LAST_POLAR_RING-2, LAST_POLAR_RING-2};
PolarLedEffect::SingleColourPolarData black4thOutsideRingPolarData = {CRGB::Black, LAST_POLAR_RING-3, LAST_POLAR_RING-3};

Gradient::GradientPolarData<> rgbGradientPolarData = {2, Gradient::GradientColourModeRGB, 3, new CRGB[3] {CRGB::Red, CRGB::Lime, CRGB::Blue}, FIRST_POLAR_RING, LAST_POLAR_RING};
Gradient::GradientPolarData<> retroGradientPolarData = {2, Gradient::GradientColourModeRGB, 4, new CRGB[4] {CRGB::DeepSkyBlue, CRGB::DarkViolet, CRGB::LightCyan, CRGB::Lavender}, FIRST_POLAR_RING, LAST_POLAR_RING};
Gradient::GradientPolarData<CHSV> retroGradientPolarDataHSV = {2, Gradient::GradientColourModeHSVSpectrum, 4, new CHSV[4] {CHSV(135, 200, 255), CHSV(210, 255, 255), CHSV(135, 20, 255), CHSV(210, 20, 255)}, FIRST_POLAR_RING, LAST_POLAR_RING};
Gradient::GradientPolarData<> solarGradientPolarData = {2, Gradient::GradientColourModeRGB, 3, new CRGB[3] {CRGB::Yellow, CRGB::Yellow, CRGB::DarkOrange}, FIRST_POLAR_RING, LAST_POLAR_RING};

NoiseEffect::NoiseEffectPolarData<> retroLavaLampNoisePolarData = {NoiseEffect::NoiseEffectColourModeRGB, {CRGB::DeepSkyBlue, CRGB::DarkViolet}, false, 150, 150, FIRST_POLAR_RING, LAST_POLAR_RING};
NoiseEffect::NoiseEffectPolarData<> sparkleNoisePolarData = {NoiseEffect::NoiseEffectColourModeRGB, {CRGB::White, CRGB::Black}, false, 180, 3000, FIRST_POLAR_RING, LAST_POLAR_RING};

TextEffect::TextEffectPolarData threeTextPolarData = {1, "3", {CRGB::White, CRGB::Black}, true, FIRST_POLAR_RING, LAST_POLAR_RING};
TextEffect::TextEffectPolarData twoTextPolarData = {1, "2", {CRGB::White, CRGB::Black}, true, FIRST_POLAR_RING, LAST_POLAR_RING};
TextEffect::TextEffectPolarData oneTextPolarData = {1, "1", {CRGB::White, CRGB::Black}, true, FIRST_POLAR_RING, LAST_POLAR_RING};
TextEffect::TextEffectPolarData zeroTextPolarData = {1, "0", {CRGB::Lime, CRGB::Black}, true, FIRST_POLAR_RING, LAST_POLAR_RING};
TextEffect::TextEffectPolarData helloTextPolarData = {5, "Hello", {CRGB::White, CRGB::Black}, true, FIRST_POLAR_RING, LAST_POLAR_RING};


struct EffectListEntry {
  LedEffect *effect;
  void *data;
};

EffectListEntry effListBG[] = {
  {&defaultCircleWipe, &redPolarData},                         // 4s of circle wipes (RGBW with text overlaid)
  {&clearPixels, &redColourData},
  {&defaultCircleWipe, &greenPolarData},
  {&clearPixels, &greenColourData},
  {&defaultCircleWipe, &bluePolarData},
  {&clearPixels, &blueColourData},
  {&defaultCircleWipe, &whitePolarData},
  {&clearPixels, &whiteColourData},
  
  {&defaultCircleZoom, &redPolarData},                         // 4s of circle zooms (RGBW)
  {&defaultCircleZoom, &greenPolarData},
  {&defaultCircleZoom, &bluePolarData},
  {&defaultCircleZoom, &whitePolarData},
  
  {&defaultMultiCircleWipe, &rgbPolarData},                    // 2s for multi-wipe into spinner (RGB)
  {&defaultSpinner, &rgbPolarData},
  
  {&defaultCircleZoomReversed, &blackPolarData},               // 1s transition to black
  
  {&defaultCircleWipe, &red1stOutsideRingPolarData},           // 8s outer ring fill+unfill
  {&defaultCircleWipe, &green2ndOutsideRingPolarData},
  {&defaultCircleWipe, &blue3rdOutsideRingPolarData},
  {&defaultCircleWipe, &white4thOutsideRingPolarData},
  {&defaultCircleWipeReversed, &black1stOutsideRingPolarData},
  {&defaultCircleWipeReversed, &black2ndOutsideRingPolarData},
  {&defaultCircleWipeReversed, &black3rdOutsideRingPolarData},
  {&defaultCircleWipeReversed, &black4thOutsideRingPolarData},
  
  {&defaultGradient, &rgbGradientPolarData},                   // 5s rainbow (RGB) gradient with text
  
  //{&defaultGradient, &retroGradientPolarData},                     // 5s retro gradient, 5s solar gradient
  {&defaultGradient, &retroGradientPolarDataHSV},
  {&defaultGradient, &solarGradientPolarData},
  
  {&defaultNoiseEffect, &retroLavaLampNoisePolarData},         // 5s lava lamp, 5s sparkle
  {&defaultNoiseEffect, &sparkleNoisePolarData},
};
unsigned int curEffectBG = 0;

EffectListEntry effListFG[] = {
  {&defaultTextEffect, &threeTextPolarData},                   // 4s of circle wipes (RGBW with text overlaid)
  {&clearPixels, &redColourData},
  {&defaultTextEffect, &twoTextPolarData},
  {&clearPixels, &greenColourData},
  {&defaultTextEffect, &oneTextPolarData},
  {&clearPixels, &blueColourData},
  {&defaultTextEffect, &zeroTextPolarData},
  {&clearPixels, &whiteColourData},
  
  {&nullEffect, NULL},                                         // 4s of circle zooms (RGBW)
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  
  {&nullEffect, NULL},                                         // 2s for multi-wipe into spinner (RGB)
  {&nullEffect, NULL},
  
  {&nullEffect, NULL},                                         // 1s transition to black
  
  {&nullEffect, NULL},                                         // 8s outer ring fill+unfill
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},

  {&longTextEffect, &helloTextPolarData},                      // 5s rainbow (RGB) gradient with text

  {&longNullEffect, NULL},                                     // 5s retro gradient, 5s solar gradient
  {&longNullEffect, NULL},
  
  {&longNullEffect, NULL},                                     // 5s lava lamp, 5s sparkle
  {&longNullEffect, NULL},
};
unsigned int curEffectFG = 0;

unsigned long last_frame_micros;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  FastLED.setBrightness(LED_BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, LED_CURRENT_LIMIT);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  effListBG[curEffectBG].effect->MarkStartTime();
  effListFG[curEffectFG].effect->MarkStartTime();
  last_frame_micros = micros();
}

void loop() {
  EffectListEntry &eff_bg = effListBG[curEffectBG];
  EffectListEntry &eff_fg = effListFG[curEffectFG];

  static bool force_reset_fg = false;

  // returns true when effect is complete (false while still running)
  if (eff_bg.effect->UpdateLeds(leds, NUM_LEDS, eff_bg.data)) {
    curEffectBG += 1;
    if (curEffectBG >= sizeof(effListBG) / sizeof(effListBG[0])) {
        curEffectBG = 0;
        force_reset_fg = true;
    }
    effListBG[curEffectBG].effect->MarkStartTime();
  }

  if (eff_fg.effect->UpdateLeds(leds, NUM_LEDS, eff_fg.data) || force_reset_fg) {
    if (force_reset_fg) {
        curEffectFG = 0;
        force_reset_fg = false;
    }
    else {
        curEffectFG += 1;
    }
    if (curEffectFG >= sizeof(effListFG) / sizeof(effListFG[0])) {
        curEffectFG = 0;
    }
    effListFG[curEffectFG].effect->MarkStartTime();
  }

  // wait for frame
  unsigned long this_frame_micros = last_frame_micros + ANIM_FRAME_MICROS;
  while (micros() < this_frame_micros) {}

  unsigned long cur_micros = micros();
  if (cur_micros > this_frame_micros + ANIM_FRAME_MICROS) {
    // dropping frames..  uh oh
    // let's at least try to resync last_frame_micros rather than drifting further behind schedule
    // (without this, we'd start running at max rate afterwards)
    Serial.println("WARNING! DROPPED FRAME(S)!");
    last_frame_micros = cur_micros + ANIM_FRAME_MICROS;
  }
  else {
    last_frame_micros = this_frame_micros;
  }

  FastLED.show();
}
