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

PolarLedEffect::SingleColourPolarData redPolarData = {CRGB::Red, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData greenPolarData = {CRGB::Lime, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData bluePolarData = {CRGB::Blue, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData blackPolarData = {CRGB::Black, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData whitePolarData = {CRGB::White, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING};

PolarLedEffect::MultiColourPolarData rgbPolarData = {3, new CRGB[3] {CRGB::Red, CRGB::Lime, CRGB::Blue}, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING};

struct EffectListEntry {
  LedEffect *effect;
  void *data;
};

EffectListEntry effListBG[] = {
  // 4s of circle wipes (RGBW with text overlaid)
  // (use filled BG colours to cover old text)
  {&defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Red, CRGB::Black, false, FIRST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Lime, CRGB::Red, false, FIRST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Blue, CRGB::Lime, false, FIRST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::White, CRGB::Blue, false, FIRST_POLAR_RING, LAST_POLAR_RING}},

  // 4s of circle zooms (RGBW)
  {&defaultCircleZoom, &redPolarData},
  {&defaultCircleZoom, &greenPolarData},
  {&defaultCircleZoom, &bluePolarData},
  {&defaultCircleZoom, &whitePolarData},

  // 2s for multi-wipe into spinner (RGB)
  {&defaultMultiCircleWipe, &rgbPolarData},
  {&defaultSpinner, &rgbPolarData},

  // 1s transition to black
  {&defaultCircleZoomReversed, &blackPolarData},

  // 8s outer ring fill+unfill
  {&defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Red, CRGB::Black, true, LAST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Lime, CRGB::Black, true, LAST_POLAR_RING-1, LAST_POLAR_RING-1}},
  {&defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Blue, CRGB::Black, true, LAST_POLAR_RING-2, LAST_POLAR_RING-2}},
  {&defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::White, CRGB::Black, true, LAST_POLAR_RING-3, LAST_POLAR_RING-3}},
  {&defaultCircleWipeReversed, new PolarLedEffect::SingleColourPolarData {CRGB::Black, CRGB::Black, true, LAST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultCircleWipeReversed, new PolarLedEffect::SingleColourPolarData {CRGB::Black, CRGB::Black, true, LAST_POLAR_RING-1, LAST_POLAR_RING-1}},
  {&defaultCircleWipeReversed, new PolarLedEffect::SingleColourPolarData {CRGB::Black, CRGB::Black, true, LAST_POLAR_RING-2, LAST_POLAR_RING-2}},
  {&defaultCircleWipeReversed, new PolarLedEffect::SingleColourPolarData {CRGB::Black, CRGB::Black, true, LAST_POLAR_RING-3, LAST_POLAR_RING-3}},

  // 5s hello text
  {&longTextEffect, new TextEffect::TextEffectPolarData {8, " Hello! ", CRGB::White, CRGB::Black, false, FIRST_POLAR_RING, LAST_POLAR_RING}},
  
  // 5s rainbow (RGB) gradient
  {&defaultGradient, new Gradient::GradientPolarData<> {2, Gradient::GradientColourModeRGB, 3, new CRGB[3] {CRGB::Red, CRGB::Lime, CRGB::Blue}, FIRST_POLAR_RING, LAST_POLAR_RING}},

  // 5s retro gradient, 5s solar gradient
  //{&defaultGradient, new Gradient::GradientPolarData<> {2, Gradient::GradientColourModeRGB, 4, new CRGB[4] {CRGB::DeepSkyBlue, CRGB::DarkViolet, CRGB::LightCyan, CRGB::Lavender}, FIRST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultGradient, new Gradient::GradientPolarData<CHSV> {2, Gradient::GradientColourModeHSVSpectrum, 4, new CHSV[4] {CHSV(135, 200, 255), CHSV(210, 255, 255), CHSV(135, 20, 255), CHSV(210, 20, 255)}, FIRST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultGradient, new Gradient::GradientPolarData<> {2, Gradient::GradientColourModeRGB, 3, new CRGB[3] {CRGB::Yellow, CRGB::Yellow, CRGB::DarkOrange}, FIRST_POLAR_RING, LAST_POLAR_RING}},

  // 5s lava lamp, 5s sparkle
  {&defaultNoiseEffect, new NoiseEffect::NoiseEffectPolarData<> {NoiseEffect::NoiseEffectColourModeRGB, CRGB::DeepSkyBlue, CRGB::DarkViolet, false, 150, 150, FIRST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultNoiseEffect, new NoiseEffect::NoiseEffectPolarData<> {NoiseEffect::NoiseEffectColourModeRGB, CRGB::White, CRGB::Black, false, 180, 3000, FIRST_POLAR_RING, LAST_POLAR_RING}},
};
unsigned int curEffectBG = 0;

EffectListEntry effListFG[] = {
  // 4s of circle wipes (RGBW with text overlaid)
  {&defaultTextEffect, new TextEffect::TextEffectPolarData {1, "3", CRGB::White, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultTextEffect, new TextEffect::TextEffectPolarData {1, "2", CRGB::White, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultTextEffect, new TextEffect::TextEffectPolarData {1, "1", CRGB::White, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING}},
  {&defaultTextEffect, new TextEffect::TextEffectPolarData {1, "0", CRGB::Lime, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING}},

  // 4s of circle zooms (RGBW)
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},

  // 2s for multi-wipe into spinner (RGB)
  {&nullEffect, NULL},
  {&nullEffect, NULL},

  // 1s transition to black
  {&nullEffect, NULL},

  // 8s outer ring fill+unfill
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},
  {&nullEffect, NULL},

  // 5s hello text
  {&longNullEffect, NULL},

  // 5s rainbow (RGB) gradient
  {&longNullEffect, NULL},

  // 5s retro gradient, 5s solar gradient
  {&longNullEffect, NULL},
  {&longNullEffect, NULL},

  // 5s lava lamp, 5s sparkle
  {&longNullEffect, NULL},
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

  // force_reset_fg is used to resync FG to BG when the full cycle repeats
  // minor frame timing differences aren't the end of the world, but this will make sure they don't grow too much
  // I should probably just build single list of effects to run simultaneously though
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
