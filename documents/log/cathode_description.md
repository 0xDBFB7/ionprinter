# Hot Cathode

-check it and see

*ahem.*

### Introduction

Hot cathodes produce a cloud of electrons via thermionic emission. We need a bunch of electrons for a number of processes: 

* For discharge ionization, to catalyze sparking an arc across the ionization chamber via townsend avalanche
* For plasma recombination, to neutralize the exiting ions.
* For vacuum tubes?

There's two main types of hot cathodes: directly and indirectly heated.

The effectiveness of a hot cathode depends on the temperature and the work function of the surface material. The work function of raw tungsten is around 
$$
4.5eV
$$

 much too high for our purpose. 

Oh, and it doesn't seem like you can buy either coated wire or paste off the shelf, so we'll have to do this ourselves.

### The ionizer cathode

To my current (ha!) understanding, electric arcs form when a few initial electrons (typically ejected from the electrodes via cold cathode effects) collide with the intervening gas particles with enough energy to liberate an electron from each. A chain reaction then occurs, where each electron ejected from an atom collides with (on average) one other atom. 

There's a whole bunch of nuance to be added here regarding electron drift velocities etc. I'll get to this part in a bit.

I don't yet know how many electrons we'll need for this section.

<hr>
### The ion curtain cathode

Hope that joke isn't

over the line

Our target print speed is 2.232×10^20 atoms/s.

To make our lives easier, I think we'll want at least double the electrons at the ion curtain in order to guarantee complete neutralization of the beam; so let's aim for 5×10^20 electrons.

An amp is a Coulomb per second, and 1 Coulomb is equal to around 6.242×10^18 electrons. Therefore we will require somewhere around 80 amps of emission current.

Using equation 14 in [2],

$$(1.2x10^6 A/(m^2 (kelvin^2))) * (((1000 Kelvin)^2)*exp(-((1eV)/(Boltzmann constant * (1000 Kelvin)))))$$



This yields a current density of around 

Hmm, that's not quite right.

Page 10 of [1] provides the following value:

> If modern highpower
> valves are considered, especially those designed for shortwave
> operation, it is found that they are operated with current
> densities between 100 and 300 mA/cm^2 (average D.C.). A very
> high momentary current density is drawn from cathodes for pulsed
> operation, especially from those in magnetrons. The peak values
> obtained in this case are between 10 and 30 amps/cm^2

Source 4, from Wikipedia, states the following value:

> Oxide filaments - 10 A/cm2

Assuming a typical value of 5A/cm^2, we will require a 16 cm^2 cathode; about 6 meters of 0.08mm OD wire - if coiled with a 2.5mm radius, 400 turns. Perhaps an indirectly heated cathode would be a better choice.

Separately, from [4] we find

> 500 mA/W

And so heating this behemoth of a cathode will eat up ~160 watts of our power budget, plus whatever power is required to accelerate the electrons into a proper curtain.

By measuring the ionization current with the cathode hot, we conveniently get a marginally effective high vacuum gauge for free. As you might imagine, the hot cathodes described here cannot be run in low vacuum, so we'll have to figure out some other method of measuring vacuum in the micron range. It might be possible to use the bowtie itself as a crude thermocouple gauge, measuring convective heat loss.

There's a whole bunch of factors that go into making a reliable, long-life cathode, but we have the luxury of being able to replace the cathode in short order, since we have a vacuum pump and a resealable chamber. Worst case scenario, if I am unable to improve the lifespan, the cathodes can be a cheap consumable. Even a 200 hour lifespan would be more than sufficient. This also lets us drive the cathodes a lot harder and get a higher specific power.

The vapor pressure of barium is quite significant at these temperatures. Certainly something to look out for.

[1]:

> "When discussing the manufacture of the emission paste the
> necessity of using pure materials and clean processes cannot be
> emphasized too strongly. As shown later, very small additions of
> foreign materials to the emission paste may influence the emission
> decisively, giving rise to a deterioration of emission in nearly all
> cases (cf. Vol. II, Sec. 26.2)."

Hmm. Seems like I might encounter some difficulty with this process.

[7]:

> The early tubes were all equipped with tungsten filaments. The filaments were low cost
> and performed very well. However, on many occasions, the operators forgot to turn the ion
> gauge tube OFF before letting the system up to atmospheric pressure. When the oxygen in the
> air reached the hot tungsten filament, it cause instant oxidation and burn-out of the filament. The
> tungsten oxide which forms on burn-out covers about half of the inside of the glass bulb with a
> creamy white coating which is easy to recognize. The solution was to find a filament material
> which did not easily oxidize, even when exposed to atmospheric pressure when hot.

This won't really be an issue for us, since our repressurisation will be automatically sequenced.

Contrary to standard vacuum tubes, we don't actually care about cathode lifespan very much; cathodes can be a cheap consumable.

[^10]

> The cathodes can be made to give usable
> emission in demountable systems by increasing their temperature (thus shortening the life very considerably)

***


Okay, so let's look at all our cathode options. [10] provides a good summary.


| Material      | Work Function | Typ. Emission | Efficiency       | Operating temp. | Resistant to poisoning | Vacuum level required |
| ------------- | ------------- | ------------- | ---------------- | --------------- | ---------------------- | --------------------- |
| Pure tungsten | 4.54 eV       | 4A/cm^2       | 17.5 W/A (1400w) | 2400K           | :heavy_check_mark:     |                       |

Tungsten wire is cheap and readily available, but the low efficiency is killer.

| Material           | Work Function | Typ. Emission | Efficiency      | Operating temp. | Resistant to poisoning |
| ------------------ | ------------- | ------------- | --------------- | --------------- | ---------------------- |
| Thoriated tungsten | 2.7 eV        | 4A/cm^2       | 6.25 W/A (500w) | 2000K           | :heavy_check_mark:     |

Again, readily available, and again a really abysmal efficiency. Good enough for testing, perhaps.

| Material     | Work Function | Typ. Emission | Efficiency   | Operating temp. | Resistant to poisoning |
| ------------ | ------------- | ------------- | ------------ | --------------- | ---------------------- |
| Barium Oxide | 1.5 eV        | 0.5A/cm^2     | 2 W/A (160w) | 1100K           |                        |

Oxide cathodes are apparently a little tricky to make.

.[^5]: 

> One drawback of oxide-coated nickel is that it is one use only. Once the system is let up to air, the cathode must be replaced. This is a problem in demountable systems where test elements within the system must be changed occasionally.

.[^1]

Contrary to the above, however, oxide cathodes are, in fact, demountable, if operated at a higher temperature and maintained at 120c when exposed to atmosphere in order to prevent water absorption.

***


 * [^1]: Herrmann, Günther, and Siegfr Wagener. The oxide-coated cathode. Vol. 2. London: Chapman & Hall, 1951.1.
 * [^2]: Grilj, Marjan. "Thermionic emission."

[^3]: [internal](../references/filament_coatings.pdf)
 * [^4]: L.W. Turner,(ed), Electronics Engineer's Reference Book, 4th ed. Newnes-Butterworth, London 1976 ISBN 0408001682 pg. 7-36
 * [5]: http://ebeaminc.com/thermionic-cathodes.htm
 * [6]: https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19760024327.pdf
 * [7]: https://www.duniway.com/images/_pg/hot-filament-ion-gauge-tube.pdf
    https://electrontubestore.com/thoriated-tungsten.pdf
 * [^8]: Criado, E., et al. "Ion beam neutralization and properties of plasmas from low power ring cusp ion thrusters." Physics of Plasmas 19.2 (2012): 023505.
 * [9]: http://www.imajeenyus.com/vacuum/20060813_thermionic_emission/index.shtml
 * [^10]: Jenkins, R. O. "A review of thermionic cathodes." Vacuum 19.8 (1969): 353-359. [Internal](../../references/jenkins1969.pdf)

[^5]: ebeam Inc. - Thermionic Cathodes [Internal]() [External](http://ebeaminc.com/thermionic-cathodes.htm)



