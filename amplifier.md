Design goal: an amplifier capable of slewing at 50us per point, with a resolution between 255 (40 um at a cm of swing) to 4096 (2.44 um), with an input voltage of ~100v.

There's a few ways of making this amplifier:

1. A classic A or B-class amp. Transistor to ground, or two transistors.

First, this requires a top-of-the-line DAC. 400khz I2C isn't fast enough: 24 bits (address+12b)*2.5us = 60 us * 4 channels = 240 us. So, an SPI dac will be required. 

Secondly, the biasing requirements for a DC class B amp will be intense - I'll probably need 4 DACs per axis, so that I can control astigmatism. At this point, we're talking roughly $20 just in DACs.

2. 
