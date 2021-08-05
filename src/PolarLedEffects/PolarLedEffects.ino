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
TextEffect defaultTextEffect = TextEffect(10000, 0);

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

NoiseEffect::NoiseEffectPolarData<> retroLavaLampNoisePolarData = {NoiseEffect::NoiseEffectColourModeRGB, {CRGB::DeepSkyBlue, CRGB::DarkViolet}, 150, 150, FIRST_POLAR_RING, LAST_POLAR_RING};
NoiseEffect::NoiseEffectPolarData<> sparkleNoisePolarData = {NoiseEffect::NoiseEffectColourModeRGB, {CRGB::White, CRGB::Black}, 180, 3000, FIRST_POLAR_RING, LAST_POLAR_RING};

TextEffect::TextEffectPolarData helloWorldTextPolarData = {13, "Hello, World!", {CRGB::White, CRGB::Black}, FIRST_POLAR_RING, LAST_POLAR_RING};


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
  {&defaultGradient, &retroGradientPolarDataHSV},
  {&defaultGradient, &solarGradientPolarData},
  {&defaultNoiseEffect, &retroLavaLampNoisePolarData},
  {&defaultNoiseEffect, &sparkleNoisePolarData},
  {&defaultTextEffect, &helloWorldTextPolarData},
};
int cur_effect = 0;

unsigned long last_frame_micros;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  FastLED.setBrightness(LED_BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, LED_CURRENT_LIMIT);
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
