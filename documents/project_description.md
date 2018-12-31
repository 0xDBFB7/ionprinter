
Thread milled the two holes for the 3/4 NPT feedthroughs - used an end mill to taper,
a ball mill to finish and a 60 degree HSS tool in a boring bar to thread mill.
I milled the thread in 6 passes, at 34% of 600 mm/m - 12k rpm. Coolant definitely required.

Leak.

Did a huge amount of reading (mainly Goebels et al and nasa ion thruster datasheets) over florida vacation, and I believe an inductively coupled plasma might be the way to go now. Currently, I don't really know what the charge on each droplet of aluminum is - how many atoms the vapour is composed of, etc. With a powerful enough ICP coil, I'll get full ionization for every single atom, making the math for deflection easier. With a mass flow rate of about 0.01g/s (easily achivable with the current 300w filament system), the complete ionization energy is somewhere around 500 watts. I've found an efficiency figure of 40% from a few ICP systems, so basically I need a 1kw RF supply. I think I'm going to go with an "induction heater"-style resonant ZVS royer driver - since everything's fully shielded, I don't think I'm going to have to worry about the frequency that much; and using a resonant system means I can skip antenna matching, which was an issue for me in the past. Good for testing, anyhow.

Mass flow rate: $$2.232×10^20 atoms/s, 0.01g/s$$ - about 40 grams an hour
2.407678×10^-18 j/atom - 15 ev, roughly $$1/2((29 amu)((10000m/s)^2))$$... roughly 500 watts of beam power.


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

With those same values and a requirement of 5 bytes per point, the software will have to stream points at nearly 100kbps. That seems achievable too. Proper curve fitting etc will also reduce that.

Values I'll probably have to update live:

- Beam X Y position, focus, astigmatism.
- Stepper X and Y position.
-

RF power, beam current, diffusion pump power, etc will presumably be set before a print.

I'm kind of coming to grips with how monumental the realtime requirements for this thing will be. I don't think I have any time to stream values to different processors - I think I'll have to abandon my original modular idea and make everything run on one STM32.

Now, the deflection amplifier.




Someone on phys.se raised a good point: at low acceleration voltages, how will stray magnetic fields affect the beam?

Since the magnetic force on a moving charge is F=qvB, the fuzzing due to magnetic fields should actually go down - but let's make sure.

At 48 eV, Aluminum will travel at 18,528 m/s. qvB at 100 gauss = -3×10^-17 newtons. The particles will take 2.7×10^-5 seconds to travel 0.5m to the build plate. This means that the beam will be slewed

0.5 * 6.695846×10^8 meters per second squared * (2.7×10^-5s)^2 = 0.2 meters.

Darn.

At 1 gauss, about 2.5 mm.

The deflection seems to be unrelated to V, but I know that this is impossible, since 30 kv CRTs aren't affected by the earth's magnetic field.

I must have something wrong in my reasoning. Going again:

(0.5*(((sqrt(2r)/sqrt(m))*b)/m)*(d/(sqrt(2r)/sqrt(m)))

Where r is energy, in eV.

R just falls out of the equation:

(0.5*b*d)/m

And so I am correct in that voltage is unrelated to magnetic deflection.

Wait, what?

Okay, looking it up, I'm totally wrong. The actual equation is:

X deflection = (0.5bd)*sqrt(q/(2(v)m))

Where v is volts. This means:

(0.5*(100 gauss* (0.5m)^2)) * sqrt(-(electron charge)/(2*48 volts *(27 amu in kg))) = 0.2 m.


At earth's magnetic field and 1000v, we get 0.1 mm. 100v: 0.5 mm. 48v: 0.7mm.

10 Gauss, 0.1m distance to build plate, and 48v: 1mm.

Now that, I believe, is fine - that's an N52 magnet at 0.1 m. Steady state components can be calibrated out, too - I'm mainly just worried about oscillating magnetic fields due to the RF coil.

With 150a going through the coil, there'll be a roughly 1.5 gauss field oscillating at 6 in away.

I can put a bunch of hall effect sensors in different places in the chamber and try to compensate for fields measured.

Aha! Existing induction heaters use inductors on the high side. When a piece of metal enters the coil, the frequency drops significantly - giving the inductor more time to allow more current through, increasing the power draw and dissipation. Cool!

Huh, wait. Why can't I just ionize the aluminum with bare +48v? The ionization energy of the aluminum is only like 15eV. If I can strike the arc with a few kilovolts (like a fluorescent tube), I should be able to ionize everything that goes through...

Hmm. Perhaps that "striking" effect relies on the gas already being ionized. That would make sense - but it'd also mean that I can't do that, since the gas is constantly being replenished.

Had an enlightening discussion on The H Bar about this striking effect; apparently it is related to the electron drift rate, and is likely to work on a stream of gas. Also, in 20170210_100824.mp4

When my milligram balance comes in, I'll try running a test with +48v and see what kind of ionization I can get - deposit onto a plate, measure the charge, weigh the plate, and divide by atomic mass of aluminum.

Going back to my graphite "bowtie" filament/crucibles; I can't find a reliable source on the resistivity of graphite, suprisingly enough; it seems to vary wildly between batches and processes. Measuring my block from McMaster, I get a value anywhere between 10 and 0.3 ohms with varying contact pressure. The block is 20.5 cm long by 2.5cm wide and 0.7cm thick. To get a slightly better reading, I connected up my Korad PSU to both ends and set it to 0.5a. This gave me a voltage of around 50-70mv and falling, for a resistance of around 1.5 ohms. Scaling that roughly to my previous bowtie's area, I get about 0.8 ohms. This explains why I didn't get good results with my previous bowtie tests at 20v; I'd only be drawing a few hundred watts. I'll have to try again with a smaller bowtie and with my new PSU.

Ooh, I could SPTM the graphite crucible! Nah, I don't need to.

Re: Speed: at 1Mbps, (about the peak for STM32 usb), transferring 1 point (6 floats; 4 bytes per float, no encoding) would take about 200 microseconds, plus response, which is longer than my target 50 us value. I guess I'll really have to fill up that buffer beforehand.

It seems like my ICP coil inductance value is incorrect: a more accurate value would be around 1 uh.
jb weld



Darnit. Another first-principles error; and I know why my many graphite/filament tests weren't working. With a surface area of 808 mm^2 (the v2 bowtie), and a temperature of 3070 K,
the bowtie will be radiating *nearly 4 kilowatts* as heat; the Stefan-Boltzmann law goes as the 4th power of temperature. The emissivity of tungsten is 0.04, which makes for a loss of 160w with the same area.

If I can get the bowtie surface area to below 20 mm^2, I'll only have to
contend with 100w of radiative losses, which seems manageable. Power budget is being eaten into a bit, but oh well.  I could also try using an aluminum reflector to increase efficiency;
aluminum is 87% reflective at infrared.



GAH; another bloody problem. If an alloy is injected into the bowtie, the different molecular masses will cause a "mass spectrometer" effect in the deflection plates, blurring the deposition horrendously.
Since we need to deposit all the materials in the alloy simultaneously, not sequentially, multiple bowties with multiple acceleration voltages tuned to their respective masses would work.

Wire feedthroughs: Buna-N has a TML of around 7; I don't even have to worry about a tiny bit of JB-weld, I don't think.

The Bowtie; the graphite that I purchased from mcmaster has an exceptionally low resistivity; around 1.54×10^-5 ohm meters. This means that a 10mm long 4*4mm rod would have a resistance of 0.0385 ohms,
drawiing 20a at 12v. When the injector holes are drilled, and some extra contact resistance is added, this is almost perfect...though this bowtie would also have an area of 80mm^2; a loss of 400w.

Might have some issues dissipating 500-odd watts of beam power in a cm^3 scale part. Liquid-cooled or cryo-chilled build platform might be required.

Even at a super slow print speed of 0.4 g/h, a 1 oz PCB will be done in 17 hours.

A serious problem: the beam current density is much, much higher than standard particle accelerators. A typical value seems to be 50 A/m^2; with a beam diameter of 0.5 cm, the initial beam current density will
be nearly 125,000. A beam diameter of 1 cm will yield a current density of 31,800; but the electrostatic optics become problematic at these charge density levels.

So, I either have to decrease my beam power (how?), or figure out how to focus at that power.

I could split the beam into seperate beams, each with a lower energy...

A small dot size is only required for fine details, where the print speed can be lower.

Some simple math: 40g/h of aluminum, which is 15 cubic centimeters (a cube 2.46cm to the side).

Tested the micro stepper injector motor: good torque at raw 5v from an arduino even up to 240rpm. It has an unusual 8-10-9-11 pinout, strangely.
