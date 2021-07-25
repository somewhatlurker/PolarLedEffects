Just playing around with some LED effects for use on WS2812 circles/rings.  
Kinda haphazard, not particularly good, but still a bit of fun.

So far it implements some simple wipes, gradients, and time-based animation of
Perlin's improved noise (FastLED noise func). Overall it's pretty basic but
alright enough.  
Performance seems like it can suffer in the more taxing effects, but using a
relatively fast microcontroller like Teensy 3.x or Raspberry Pi Pico should
make it handle pretty much everything decently. There's quite a few easy
optimisation opportunities that should get it running well on weaker devices,
if anyone is so inclined.

Uses Arduino IDE+FastLED, and unlike common convention assumes the "strip"
starts at the centre of the circle. (this is subject to change)

Works alright on the Wokwi simulator.

I don't really have any plans for this at the moment. If you want to use this
code feel free, but issues will likely be ignored.