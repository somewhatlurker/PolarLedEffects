#define FIRST_POLAR_RING 0
#define LAST_POLAR_RING 3

#define LED_PIN 3
#define NUM_LEDS 37

#define ANIM_FPS 120
#define ANIM_FRAME_MICROS (1000000 / ANIM_FPS)

#include <FastLED.h>
#include "LedEffect.h"
#include "PolarLedEffect.h"

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

PolarLedEffect::SingleColourPolarData redPolarData = {CHSV(HUE_RED, 255, 255), FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData greenPolarData = {CHSV(HUE_GREEN, 255, 255), FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData bluePolarData = {CHSV(HUE_BLUE, 255, 255), FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData blackPolarData = {CHSV(0, 0, 0), FIRST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData whitePolarData = {CHSV(0, 0, 255), FIRST_POLAR_RING, LAST_POLAR_RING};

PolarLedEffect::MultiColourPolarData rgbPolarData = {3, new CHSV[3] {CHSV(HUE_RED, 255, 255), CHSV(HUE_GREEN, 255, 255), CHSV(HUE_BLUE, 255, 255)}, FIRST_POLAR_RING, LAST_POLAR_RING};

PolarLedEffect::SingleColourPolarData red1stOutsideRingPolarData = {CHSV(HUE_RED, 255, 255), LAST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData green2ndOutsideRingPolarData = {CHSV(HUE_GREEN, 255, 255), LAST_POLAR_RING-1, LAST_POLAR_RING-1};
PolarLedEffect::SingleColourPolarData blue3rdOutsideRingPolarData = {CHSV(HUE_BLUE, 255, 255), LAST_POLAR_RING-2, LAST_POLAR_RING-2};
PolarLedEffect::SingleColourPolarData white4thOutsideRingPolarData = {CHSV(0, 0, 255), LAST_POLAR_RING-3, LAST_POLAR_RING-3};

PolarLedEffect::SingleColourPolarData black1stOutsideRingPolarData = {CHSV(0, 0, 0), LAST_POLAR_RING, LAST_POLAR_RING};
PolarLedEffect::SingleColourPolarData black2ndOutsideRingPolarData = {CHSV(0, 0, 0), LAST_POLAR_RING-1, LAST_POLAR_RING-1};
PolarLedEffect::SingleColourPolarData black3rdOutsideRingPolarData = {CHSV(0, 0, 0), LAST_POLAR_RING-2, LAST_POLAR_RING-2};
PolarLedEffect::SingleColourPolarData black4thOutsideRingPolarData = {CHSV(0, 0, 0), LAST_POLAR_RING-3, LAST_POLAR_RING-3};

Gradient::GradientPolarData rgbGradientPolarData = {2, 3, new CHSV[3] {CHSV(HUE_RED, 255, 255), CHSV(HUE_GREEN, 255, 255), CHSV(HUE_BLUE, 255, 255)}, FIRST_POLAR_RING, LAST_POLAR_RING};
Gradient::GradientPolarData retroGradientPolarData = {2, 4, new CHSV[4] {CHSV(140, 255, 255), CHSV(200, 255, 255), CHSV(140, 20, 255), CHSV(200, 20, 255)}, FIRST_POLAR_RING, LAST_POLAR_RING};
Gradient::GradientPolarData solarGradientPolarData = {2, 3, new CHSV[3] {CHSV(HUE_YELLOW, 255, 255), CHSV(HUE_YELLOW, 255, 255), CHSV(HUE_ORANGE, 255, 255)}, FIRST_POLAR_RING, LAST_POLAR_RING};


struct EffectListEntry {
  LedEffect *effect;
  void *data;
};

EffectListEntry effList[] = {
  {&defaultCircleWipe, &redPolarData},
  {&defaultCircleWipe, &greenPolarData},
  {&defaultCircleWipe, &bluePolarData},
  {&defaultCircleWipe, &whitePolarData},
  {&defaultCircleZoom, &redPolarData},
  {&defaultCircleZoom, &greenPolarData},
  {&defaultCircleZoom, &bluePolarData},
  {&defaultCircleZoom, &whitePolarData},
  {&defaultMultiCircleWipe, &rgbPolarData},
  {&defaultSpinner, &rgbPolarData},
  {&defaultCircleZoomReversed, &blackPolarData},
  {&defaultCircleWipe, &red1stOutsideRingPolarData},
  {&defaultCircleWipe, &green2ndOutsideRingPolarData},
  {&defaultCircleWipe, &blue3rdOutsideRingPolarData},
  {&defaultCircleWipe, &white4thOutsideRingPolarData},
  {&defaultCircleWipeReversed, &black1stOutsideRingPolarData},
  {&defaultCircleWipeReversed, &black2ndOutsideRingPolarData},
  {&defaultCircleWipeReversed, &black3rdOutsideRingPolarData},
  {&defaultCircleWipeReversed, &black4thOutsideRingPolarData},
  {&defaultGradient, &rgbGradientPolarData},
  {&defaultGradient, &retroGradientPolarData},
  {&defaultGradient, &solarGradientPolarData},
};
int cur_effect = 0;

unsigned long last_frame_micros;

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  effList[cur_effect].effect->MarkStartTime();
  last_frame_micros = micros();
}

void loop() {
  EffectListEntry &eff = effList[cur_effect];

  // returns true when effect is complete (false while still running)
  if (eff.effect->UpdateLeds(leds, NUM_LEDS, eff.data)) {
    cur_effect += 1;
    cur_effect %= sizeof(effList) / sizeof(effList[0]);
    effList[cur_effect].effect->MarkStartTime();
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
