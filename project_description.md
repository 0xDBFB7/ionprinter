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


Thread milled the two holes for the 3/4 NPT feedthroughs - used an end mill to taper, a ball mill to finish and a 60 degree HSS tool in a boring bar to thread mill. I milled the thread in 6 passes, at 34% of 600 mm/m - 12k rpm. Coolant definitely required.

Leak.

---

Did a huge amount of reading (mainly Goebels et al and nasa ion thruster datasheets) over florida vacation, and I believe an inductively coupled plasma might be the way to go now. Currently, I don't really know what the charge on each droplet of aluminum is - how many atoms the vapour is composed of, etc. With a powerful enough ICP coil, I'll get full ionization for every single atom, making the math for deflection easier. With a mass flow rate of about 0.01g/s (easily achivable with the current 300w filament system), the complete ionization energy is somewhere around 500 watts. I've found an efficiency figure of 40% from a few ICP systems, so basically I need a 1kw RF supply. I think I'm going to go with an "induction heater"-style resonant ZVS royer driver - since everything's fully shielded, I don't think I'm going to have to worry about the frequency that much; and using a resonant system means I can skip antenna matching, which was an issue for me in the past. Good for testing, anyhow.

Mass flow rate: $$2.232×10^20 atoms/s, 0.01g/s$$ - about 40 grams an hour
2.407678×10^-18 j/atom - 15 ev, roughly $$1/2((29 amu)((10000m/s)^2))$$


Regarding the ICP ionization, the thermal velocity of the gaseous aluminum will be around 1.7 km/s, meaning at 250khz the atoms'll move around 7 mm before being ionized - if the coil isn't long enough, the atoms'll be out of the magnetic field by the time they leave.

Found an awesome paper that describes the behavior of 500khz plasmas. It seems like LF plasmas actually have some desirable properties over the standard 13.56mhz ISM plasmas, once you get past E-mode - high uniformity, etc. LF is also much easier from a generator standpoint.

I think I'm going to transition to an "untapped" royer topology - this'll mean I only need two high current feedthroughs.

A 12mm diameter 25 mm long 5-turn air-core inductor has an inductance of roughly 0.00012755 millihenries. A resonant frequency of 500 khz will require a tank capacitance of around 0.8 microfarads. I want to err on the side of higher frequency, I think, so 





