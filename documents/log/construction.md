# Mk3

Log starts at gauge.

Need to drill 8.80mm or 11/32 holes in some aluminum and weld with brazing rod.

Brazed. See Brazing.md.

Seemed like one of the crush gaskets didn't fully seal on one of the spark plug feedthroughs - but in fact the brazing was porus.

# Mk3-5

Using a scrap piece of aluminum from an old boat engine mount. Had a few holes, but I could work them into the design.

Milled on 6040.

| Tool size | XY feed   | Z feed | RPM   | Z step | Stepover |
| --------- | --------- | ------ | ----- | ------ | -------- |
| 1/8"      | 7 mm/s    | 1 mm/s | 13000 | 1 mm   | 50%      |
| 0.25"     | 11.6 mm/s | 1 mm/s | 13000 | 1 mm   | 50%      |
|           |           |        |       |        |          |

Vaporized an entire bottle of WD-40 as coolant. This permeated into the MDF spoilboard, gradually reducing the clamping force until the part broke loose. Part was salvageable.

Used an entire pack of 180-grit trying to knock down some of the rack-rash. My friend Arnold tried to flycut  the thing, but there was excessive chatter.

Used an oscillating palm sander with some 320-grit silicon carbide. Took it to a pleasant dull finish.

Cleaned 5 times with water and then isopropyl.

THIS THING looks BAMF.



Spun up a few more times, achieved a base vacuum level of around 0.8 millibar (660 micron) - still not great, but I shut everything down prematurely as heating was excessive.

Set up a liquid cooling system for the turbo.

Purchased some nichrome wire from DashVapes. Needed a driver's license, amusingly.

Quite nervous around this thing right now; the clamping force on the turbo is almost surely insufficient. If it crashes, it will send high-energy shrapnel in all directions, including through my torso. I *like* my torso the way it is, thank you very much.



I raised the input voltage to the turbopump controller to 56v, which worked admirably. The turbo audibly spun up significantly.

Unfortunately, upon re-cycling the chamber, the controller exploded from the excess voltage, presumably spikes due to high startup current. I am unsure as to the cause; the mosfet drivers were clearly blown, but the mosfets themselves were also shorted. The drivers have headroom to 85v, and the mosfets are good to 100v - I can't imagine that switching noise got that high.



Holy crap! The high-side mosfet gate trace and 2 ohm gate resistor blew! That's an incredible failure mode.

Found out why the mosfet gate oxide blew in the first place: the gate was swinging across the entire 50v. Tried increasing the charge pump cap to 0.22 uf , but the driver blew up immediately.

Simplified everything to one channel, same problem. Swapped mosfets, drivers, added an external boostrap diode. Voltage across the diode 



I am an ABSOLUTE NINCOMPOOP.  A complete and total kneebiter.

So I tried to determine what caused the tragic loss of my gate oxide. Lo and behold, it seemed that the mosfet gate was swinging the full 48v input, plus 12v from the charge pump.

This is super strange for several reasons: one, the circuit was functioning normally, where the gate should have been destroyed in microseconds; 
and two, the block diagram of the MIC4604 driver doesn't reveal a failure mode that could possibly cause this issue.

Cue several hours of anti-Hippocratic probing and prodding that James A. Garfield would have blushed at, and wanton swapping of $40 in drivers

until I realised 

it's a *charge pump*, you total f***ing idiot. It's *not ground referenced*.


Floating the board's supply and connecting the scope ground to the drain of the MOSFET revealed a perfect 12v square wave.

<hr>

Spun the pump up again: noise is causing the driver Arduino to reset, which sucks. However, I was able to spin it all the way up; the current visibly dropped from 8a to ~4. The liquid cooling water warmed up quite pleasantly. Capacitors on the driver board warmed up slightly. Only achieved a base vacuum of around 0.7 mBar.

The pump occasionally spins down rapidly when latchup occurs: the energy is being dumped into the mosfets and TVS diode.



Tried using solder to seal up the 

Need to cast some crush gaskets.

Wood seems to be an effective mold for solder. Aluminum coated with aquadag or graphite also works.

caulk also works? Huh.

<https://www.youtube.com/watch?v=92OLGmu3hws>

Tried casting lead-free solder in sand: failed miserably, sand melted.

Made a little aluminum mold for a gasket: failed miserably, surface tension of solder was too high. 

Tried O-rings to seal, failed miserably, couldn't get below 400 mbar.

Ah, but pure 1101 aluminum wire works perfectly!

Mm, not quite - they were prone to cracking. 

Soldered some copper wires into rings - worked slightly better, but still not perfectly.

Ordered in some copper crush washers (#97725A250) from McMaster. These worked well, though a large amount of torque was required to seat them, and I'm not sure how the aluminum threads are holding up. A substantial amount of black residue was deposited onto the surface of the spark plugs from the last attempts. I am unsure of its composition.



Got everything cleaned and polished, washers seated, spark plugs attached - then the second roughing pump coupler exploded. 

Designed in openscad, 3D-printing some new ones now.

Printed, installed. Worked fine, except there was a large amount of vibration - might split it into a spider coupling.

Pumped down. Copper sealing rings worked perfectly. It wasn't worth the time to try to make my own.

Hit 5x10^-4 mBar. 



Two days later, hit 2.37x10^-5 millibar, even with a few things in the chamber. 

![second_pulldown](../../data/second_pulldown.png)

Some discussion on the Vacuum Hackers discord led me to believe that the pump was almost definitely going to fly away, what with the minimal clamping force provided

Printed out a 122mm bolt circle, disassembled everything, drilled three holes into the table, and re-assembled everything. The pump is now very rigidly affixed via the M6 bolt pattern on the bottom. 

Planning on swapping out the buna-n gaskets for viton. Very expensive, but whatever.

Planning on using a few CER-4 standoffs from digikey for the bowtie. These are made of STEATITE C220, with a maximum temperature of 1700c.

Milling graphite; used MeshCAM to generate the features. Went quite well, most broke but one survived. needed G64P.001 to prevent linuxcnc from rounding corners.

Assembled the bowtie module - I'm not actually sure why I'm doing all this, but it went really well in any case.

<hr>

#### E-beam test 1:

4.7 milliamps of e-beam current was drawn with a white-hot 0.1mm tungsten hairpin and a nickel flag at 20v bias. Initial tests with 100v bias were unsuccessful; however, it is believed that this was due to reversed polarity.  

Pressures were stable at 1x10^-5 mbar. 

Some tungsten was deposited on surfaces on the chamber; it is assumed that this is due to the unregulated temperature of the filament. As expected, the emission peaked at a certain input power and did not increase further with additional current. The current was increased regardless, and the filament vaporized in a satisfactory manner.

An aluminum baffle was used to prevent debris ingress into the pump.

The pump was accidentally dumped to atmosphere while at operating speed; this does not appear to have caused any damage.

It was expected that beam alignment could be performed manually, by viewing the ion trail left by the electron beam; however, the thermionic cathode produced too much light for this to be possible.

<hr>

A precise reading of the bowtie temperature is now required in order to set the vapor pressure. The resistance of the graphite is an excellent indicator, but the temperature forms a gradient between the center and the mounting points. This gradient has been simulated via ElmerFEM, but ultimately depends on the ceramic interface thermal conductivity etc. 

The temperature could also be inferred backwards from the beam current, or by measuring light output. 

A thermistor could be used to determine the mount temperature in-situ for a series of power levels, but the thermal mass of a thermistor is likely greater than that of the mount.

Some thermocouple wire was cut to 

<hr>

The precise secondary electron energy was at this point unknown. A crude momentum analysis showed a beam deflection angle of some 20 degrees - but this effect is significantly affected by secondary electron energy. 

It was decided that a SPARTA simulation would be used to determine the beam deflection; however, DSMC VSS and VHS parameters are required for an accurate picture, and can only be found in 

**(Bird94)** G. A. Bird, Molecular Gas Dynamics and the Direct Simulation of Gas Flows, Clarendon Press, Oxford (1994).

A copy of Bird94 was not available online (save for a few pages on Google Books), and a hard copy would cost more than this entire vacuum system. Substituting 

<hr>

#### E-beam test 2

A tungsten filament was produced by wrapping 5 0.1mm tungsten wires around each other. The filament was visually aligned with the bowtie.

This filament consumed a whopping 44 watts - while emitting the same 4 milliamps of beam current. Certainly a far cry from the 17w/A figure described in the literature.

As with the last test, this filament saturated the high-voltage power supply. A 31v bias was used instead.

The base pressure attained was significantly higher (2x10T-5 millibar) than in the previous test; this was attributed to the increased clutter within the chamber. Some flux had also splattered.

The charge on the bowtie was measured to determine focusing. Very curiously, the charge increased when the e-beam was activated, but dropped rapidly. This may be an artifact of the relative nature of the measurement, or a symptom of a greater electron-deflection problem.

<hr>

#### ElmerFEM simulations of bowtie prototype 1

Now 

The .sif files had been overwritten somehow - reverting to a previous commit fixed the problem.

ElmerFEM requires a .step input. This was meshed with Max h. 0.025 and Min h. 0.001. The enthalpy heat flux was set to zero. Convergence was not well behaved.

#### Bowtie test 1, 2 and 3

The chamber was pumped down and 10 A was applied across the bowtie. The bowtie glowed a dim red, but not nearly sufficient for deposition. The resistance of the entire assembly was only approximately 0.25 ohms, a significant discrepancy from the predicted 4 ohms. 

The "300-1200v" HV e-beam bias power supply was supplied with 12v, and output 2 kilovolts. A replacement NCH6100HV was purchased. 

The bowtie was connected to the 20v-45A supply, and 20a was briefly supplied. Aluminum was readily deposited, and no degradation of the bowtie nozzle was found. The deposited coating was not 

One of the bowtie connection wires melted. 

