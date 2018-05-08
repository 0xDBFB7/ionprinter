Project:
===
Status:
-------

#### Date started: 2016-??-??
#### Date finished: 2017-??-??
#### Date written: 2017-08-14


Background:
---


Description:
---


---

Thread milled the two holes for the 3/4 NPT feedthroughs - used an end mill to taper, a ball mill to finish and a 60 degree HSS tool in a boring bar to thread mill. I milled the thread in 6 passes, at 34% of 600 mm/m - 12k rpm. Coolant definitely required.

Leak.

Did a huge amount of reading (mainly Goebels et al and nasa ion thruster datasheets) over florida vacation, and I believe an inductively coupled plasma might be the way to go now. Currently, I don't really know what the charge on each droplet of aluminum is - how many atoms the vapour is composed of, etc. With a powerful enough ICP coil, I'll get full ionization for every single atom, making the math for deflection easier. With a mass flow rate of about 0.01g/s (easily achivable with the current 300w filament system), the complete ionization energy is somewhere around 500 watts. I've found an efficiency figure of 40% from a few ICP systems, so basically I need a 1kw RF supply. I think I'm going to go with an "induction heater"-style resonant ZVS royer driver - since everything's fully shielded, I don't think I'm going to have to worry about the frequency that much; and using a resonant system means I can skip antenna matching, which was an issue for me in the past. Good for testing, anyhow.

Mass flow rate: $$2.232×10^20 atoms/s, 0.01g/s$$ - about 40 grams an hour
2.407678×10^-18 j/atom - 15 ev, roughly $$1/2((29 amu)((10000m/s)^2))$$


Regarding the ICP ionization, the thermal velocity of the gaseous aluminum will be around 1.7 km/s, meaning at 250khz the atoms'll move around 7 mm before being ionized - if the coil isn't long enough, the atoms'll be out of the magnetic field by the time they leave.

Found an awesome paper that describes the behavior of 500khz plasmas. It seems like LF plasmas actually have some desirable properties over the standard 13.56mhz ISM plasmas, once you get past E-mode - high uniformity, etc. LF is also much easier from a generator standpoint.

I think I'm going to transition to an "untapped" royer topology - this'll mean I only need two high current feedthroughs.

A 12mm diameter 25 mm long 5-turn air-core inductor has an inductance of roughly 0.00012755 millihenries. A resonant frequency of 500 khz will require a tank capacitance of around 0.8 microfarads. I want to err on the side of higher frequency, I think, so...

Whipped up a few LTspice simulations, found I needed inductance on the input (naturally), but it seems like I could actually just use a h-bridge on the high side...

Tested out the chamber a few more times, still leaking at the feedthroughs. Need to re-design them.

<lots of stuff happens off-camera>

Oh jeez.

This is going to have to be fast.

Really fast.

With a 100g/h deposition rate, a 10 mil dot size, and reasonable Z resolution of somewhere around 5 mil, the slew time of the deflection supplies will have to be under 50 us.

With a 10 mil dot size over a 1 cm deflection (400 steps), the slide will have to move at around $$1 cm/ 20 milliseconds$$ - around 40,000 mm/min. This seems just barely achievable with stepper slides.

With those same values and a requirement of 5 bytes per point , the software will have to stream points at nearly 100kbps. That seems achivable too. Proper curve fitting etc will also reduce that.

Values I'll probably have to update live: 

- Beam X Y position, focus, astigmatism.
- Stepper X and Y position.
- 

RF power, beam current, diffusion pump power, etc will presumably be set before a print.

I'm kind of coming to grips with how monumental the realtime requirements for this thing will be. I don't think I have any time to stream values to different processors - I think I'll have to abandon my original modular idea and make everything run on one STM32.

Now, the deflection amplifier.




