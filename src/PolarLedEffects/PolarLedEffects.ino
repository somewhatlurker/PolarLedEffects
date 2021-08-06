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

LedEffect nullEffect = LedEffect(0);

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
    LedEffect *bgEffect;
    void *bgData;
    LedEffect *fgEffect;
    void *fgData;
};

EffectListEntry effList[] = {
  // 4s of circle wipes (RGBW with "3 2 1 0" text overlaid)
  // (use filled BG colours to cover old text)
  {
      &defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Red, CRGB::Black, false, FIRST_POLAR_RING, LAST_POLAR_RING},
      &defaultTextEffect, new TextEffect::TextEffectPolarData {1, "3", CRGB::White, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING},
  },
  {
      &defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Lime, CRGB::Red, false, FIRST_POLAR_RING, LAST_POLAR_RING},
      &defaultTextEffect, new TextEffect::TextEffectPolarData {1, "2", CRGB::White, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING},
  },
  {
      &defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Blue, CRGB::Lime, false, FIRST_POLAR_RING, LAST_POLAR_RING},
      &defaultTextEffect, new TextEffect::TextEffectPolarData {1, "1", CRGB::White, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING},
  },
  {
      &defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::White, CRGB::Blue, false, FIRST_POLAR_RING, LAST_POLAR_RING},
      &defaultTextEffect, new TextEffect::TextEffectPolarData {1, "0", CRGB::Lime, CRGB::Black, true, FIRST_POLAR_RING, LAST_POLAR_RING},
  },

  // 4s of circle zooms (RGBW)
  {
      &defaultCircleZoom, &redPolarData,
      &nullEffect, NULL,
  },
  {
      &defaultCircleZoom, &greenPolarData,
      &nullEffect, NULL,
  },
  {
      &defaultCircleZoom, &bluePolarData,
      &nullEffect, NULL,
  },
  {
      &defaultCircleZoom, &whitePolarData,
      &nullEffect, NULL,
  },

  // 2s for multi-wipe into spinner (RGB)
  {
      &defaultMultiCircleWipe, &rgbPolarData,
      &nullEffect, NULL,
  },
  {
      &defaultSpinner, &rgbPolarData,
      &nullEffect, NULL,
  },

  // 1s transition to black
  {
      &defaultCircleZoomReversed, &blackPolarData,
      &nullEffect, NULL,
  },

  // 8s outer ring fill+unfill
  {
      &defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Red, CRGB::Black, true, LAST_POLAR_RING, LAST_POLAR_RING},
      &nullEffect, NULL,
  },
  {
      &defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Lime, CRGB::Black, true, LAST_POLAR_RING-1, LAST_POLAR_RING-1},
      &nullEffect, NULL,
  },
  {
      &defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::Blue, CRGB::Black, true, LAST_POLAR_RING-2, LAST_POLAR_RING-2},
      &nullEffect, NULL,
  },
  {
      &defaultCircleWipe, new PolarLedEffect::SingleColourPolarData {CRGB::White, CRGB::Black, true, LAST_POLAR_RING-3, LAST_POLAR_RING-3},
      &nullEffect, NULL,
  },
  {
      &defaultCircleWipeReversed, new PolarLedEffect::SingleColourPolarData {CRGB::Black, CRGB::Black, true, LAST_POLAR_RING, LAST_POLAR_RING},
      &nullEffect, NULL,
  },
  {
      &defaultCircleWipeReversed, new PolarLedEffect::SingleColourPolarData {CRGB::Black, CRGB::Black, true, LAST_POLAR_RING-1, LAST_POLAR_RING-1},
      &nullEffect, NULL,
  },
  {
      &defaultCircleWipeReversed, new PolarLedEffect::SingleColourPolarData {CRGB::Black, CRGB::Black, true, LAST_POLAR_RING-2, LAST_POLAR_RING-2},
      &nullEffect, NULL,
  },
  {
      &defaultCircleWipeReversed, new PolarLedEffect::SingleColourPolarData {CRGB::Black, CRGB::Black, true, LAST_POLAR_RING-3, LAST_POLAR_RING-3},
      &nullEffect, NULL,
  },

  // 5s hello text
  {
      &longTextEffect, new TextEffect::TextEffectPolarData {8, " Hello! ", CRGB::White, CRGB::Black, false, FIRST_POLAR_RING, LAST_POLAR_RING},
      &nullEffect, NULL,
  },
  
  // 5s rainbow (RGB) gradient
  {
      &defaultGradient, new Gradient::GradientPolarData<> {2, Gradient::GradientColourModeRGB, 3, new CRGB[3] {CRGB::Red, CRGB::Lime, CRGB::Blue}, FIRST_POLAR_RING, LAST_POLAR_RING},
      &nullEffect, NULL,
  },

  // 5s retro gradient, 5s solar gradient
  /*{
      &defaultGradient, new Gradient::GradientPolarData<> {2, Gradient::GradientColourModeRGB, 4, new CRGB[4] {CRGB::DeepSkyBlue, CRGB::DarkViolet, CRGB::LightCyan, CRGB::Lavender}, FIRST_POLAR_RING, LAST_POLAR_RING},
      &nullEffect, NULL,
  },*/
  {
      &defaultGradient, new Gradient::GradientPolarData<CHSV> {2, Gradient::GradientColourModeHSVSpectrum, 4, new CHSV[4] {CHSV(135, 200, 255), CHSV(210, 255, 255), CHSV(135, 20, 255), CHSV(210, 20, 255)}, FIRST_POLAR_RING, LAST_POLAR_RING},
      &nullEffect, NULL,
  },
  {
      &defaultGradient, new Gradient::GradientPolarData<> {2, Gradient::GradientColourModeRGB, 3, new CRGB[3] {CRGB::Yellow, CRGB::Yellow, CRGB::DarkOrange}, FIRST_POLAR_RING, LAST_POLAR_RING},
      &nullEffect, NULL,
  },

  // 5s lava lamp, 5s sparkle
  {
      &defaultNoiseEffect, new NoiseEffect::NoiseEffectPolarData<> {NoiseEffect::NoiseEffectColourModeRGB, CRGB::DeepSkyBlue, CRGB::DarkViolet, false, 150, 150, FIRST_POLAR_RING, LAST_POLAR_RING},
      &nullEffect, NULL,
  },
  {
      &defaultNoiseEffect, new NoiseEffect::NoiseEffectPolarData<> {NoiseEffect::NoiseEffectColourModeRGB, CRGB::White, CRGB::Black, false, 180, 3000, FIRST_POLAR_RING, LAST_POLAR_RING},
      &nullEffect, NULL,
  },
};
unsigned int curEffect = 0;

unsigned long last_frame_micros;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  FastLED.setBrightness(LED_BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, LED_CURRENT_LIMIT);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  effList[curEffect].bgEffect->MarkStartTime();
  effList[curEffect].fgEffect->MarkStartTime();
  last_frame_micros = micros();
}

void loop() {
  EffectListEntry &eff = effList[curEffect];

  // unsigned long pre_render_micros = micros();

  // UpdateLeds returns true when effect is complete (false while still running)
  bool bgUpdateRes = eff.bgEffect->UpdateLeds(leds, NUM_LEDS, eff.bgData);
  bool fgUpdateRes = eff.fgEffect->UpdateLeds(leds, NUM_LEDS, eff.fgData);

  // Serial.print("Render Time: ");
  // Serial.print(micros() - pre_render_micros);
  // Serial.println("us");

  // if BG and FG effect are both complete, increment curEffect and mark start of next effects
  if (bgUpdateRes && fgUpdateRes) {
    curEffect += 1;
    if (curEffect >= sizeof(effList) / sizeof(effList[0])) {
        curEffect = 0;
    }
    effList[curEffect].bgEffect->MarkStartTime();
    effList[curEffect].fgEffect->MarkStartTime();
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
