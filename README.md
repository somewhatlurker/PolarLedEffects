Just playing around with some LED effects for use on WS2812 circles/rings.  
Kinda haphazard, not particularly good, but still a bit of fun.

So far it implements some simple wipes, gradients, and time-based animation of
Perlin's improved noise (FastLED noise func). Overall it's pretty basic but
alright enough.

Performance can suffer in the more taxing effects, but using a fast
microcontroller like Teensy 3.x or Raspberry Pi Pico along with non-blocking
LED output makes everything run decently.
On a Raspberry Pi Pico, everything except NoiseEffect runs at over 120 FPS even
with the full 241 LEDs/9 rings (NoiseEffect runs at ~115 FPS). Pretty acceptable
considering that running over 130 FPS would require splitting the LEDs into
multiple strings anyway, but there's still quite a few easy optimisation
opportunities left that would benefit weaker hardware.

Uses Arduino IDE+FastLED, and unlike common convention assumes the "strip"
starts at the centre of the circle.

Works alright on the Wokwi simulator.

I don't really have any plans for this at the moment. If you want to use this
code feel free, but issues will likely be ignored.