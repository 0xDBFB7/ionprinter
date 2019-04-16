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