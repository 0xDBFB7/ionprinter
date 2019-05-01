# Mk3

Log starts at gauge.

Need to drill 8.80mm or 11/32 holes in some aluminum and weld with brazing rod.

### Brazing aluminum

Aluminum brazing rod: seems like pre-heating is important. Someone on youtube uses a barbecue to great effect.

Purchased some Benzomatic AL-3 aluminum soldering rods from Canadian Tire. Encountered some difficulty in attaining the required temperature. Melted my jacket in the process.

Tried preheating with an electric stovetop element. The element itself got to nearly 500c, but the aluminum only got to around 300c - below the ~400c required for the AL-3 rods.

Tried oxy-acetalyne, not sufficiently warm. 

Tried a 100,000 btu/hr flamethrower: sufficiently warm.

Many voids were found in the final braze - it was not in any way airtight. This may be due to the fact that it was not clamped properly. Re-brazed several times.

Standard table salt is apparently effective as a flux. AvE recommends using a glass rod to remove dross.

Electrochemical weld cleaning

Looks like I put a bit too much heat on the feedthroughs - porosity

Seemed like one of the crush gaskets didn't fully seal on one of the spark plug feedthroughs - but in fact the brazing was porous.

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

Some discussion on the Vacuum Hackers discord led me to believe that the pump was almost definitely going to fly away, what with the minimal clamping force provided. 

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

The base pressure attained was significantly higher (2x10^-5 millibar) than in the previous test; this was attributed to the increased clutter within the chamber. Some flux had also splattered.

The charge on the bowtie was measured to determine focusing. Very curiously, the charge increased when the e-beam was activated, but dropped rapidly. This may be an artifact of the relative nature of the measurement, or a symptom of a greater electron-deflection problem.

<hr>

#### ElmerFEM simulations of bowtie prototype 1

The .sif files had been overwritten somehow - reverting to a previous commit fixed the problem.

ElmerFEM requires a .step input. This was meshed with Max h. 0.025 and Min h. 0.001. The enthalpy heat flux was set to zero. Convergence was not well behaved.

The simulation potential was set to 20v, and an ideal temperature of approximately 2000 K was attained. The bowtie drew approximately 100w.

#### Bowtie test 1, 2 and 3

The chamber was pumped down and 10 A was applied across the bowtie. The bowtie glowed a dim red,  not nearly sufficient for deposition. The resistance of the entire assembly was only approximately 0.33 ohms, a significant discrepancy from the predicted 4 ohms. Clearly the graphite resistivity value set in the Elmer sim was incorrect.

The "300-1200v" HV e-beam bias power supply was supplied with 12v, and output 2 kilovolts. A replacement NCH6100HV was purchased. 

The bowtie was connected to the 20v-45A supply, and 20a was briefly supplied. Aluminum was readily deposited, and no degradation of the bowtie nozzle was found. The deposited coating was quite easily wiped off the witness microscope slide.

One of the bowtie connection wires melted. 

#### IBSimu simulations of entire column 

A 3 by 3 by 6 mm 3D simulation was created. Unfortunately, an accurate measurement of space charge requires about 10 grid points across the beam - a mesh of 0.00003mm. This is not reasonable to compute in a short period of time. 

However, human factors were encountered - that is, the designer was filled with abject boredom. An automatic solution is required.

#### Beam solver, trial 5

After the modest successes over the previous week, more effort was put into an automatic beam solver.

This was based on the following premise:

1. I am too stupid to design a lens of this sort.
2. I know something about software.
3. Software can design a lens of this sort.

QED.

A mesh of 50x50 pixels over the entire beam would seem to be reasonable. 

- 3d "Slice" solver 
  - analytic function for slices was not found
- 3d Analytic concentric infinitesimal ring space charge solver, with automatic beam envelope
  - poorly implemented in Python, unusably slow
  - horrific write-only blob of vectors and linear algebra
  - There is precedent for this beam slicing technique, likely quite valuable if correctly implemented
- Brief diversion: an attempt to use TRACE-3D and TRANSPORT to fit the beam
  - miserable failure
  - what 
  - what even
- 2d simplified analytic concentric ring space charge solver, predefined beam envelope, rings also used for electrode field
  - Almost usable
  - on the order of a few seconds per iteration in Python
- 2d Laplace + Jacobi relaxation solver for electrode e-field, cylindrical beam scharge using analytic equation from @ Kalvas
  - Simple array operations in Python: 1-2 seconds
  - Numpy convolution: ~0.15 seconds
  - Pretty diagnostics with matplotlib
  - Random search ineffective, genetic algorithm considered with DEAP 
- Wonderfully fast C++ Laplace solver
  - 24 ms per iteration!
  - aww yis

The search space of possible lens arrangements spans a triple-digit exponent of combinations, so an exhaustive search is clearly not possible. However, seeding the system with some common extraction and focusing arrangements (Pierce, Wehnelt, Einzel, etc) greatly reduces the complexity of the problem.



#### Low-voltage, high-current electron gun, Peter W. Erdman and Edward C. Zipf 

and

#### Design of Low Voltage Electron Guns, J. AROL SIMPSON AND C. E. KUYATT

This is the breakthrough I've been hoping for. An e-beam gun capable of ~1-3 mA at 100 eV.

Moreover, simulations (and, indeed, basic Child-Langmuir expressions) have shown that a comparatively high extraction voltage is required to form a reasonable beam. I previously dismissed this technique; my malformed intuition 

> There is a general technique which can be used to circumvent these limits on unipotential guns. The technique is to multistage the gun; i.e., utilize a higher voltage to overcome the space charge in front of the cathode and then decelerate to the required final energy. 
>
> This technique seems to have first been arrived at on empirical grounds.
>
> Once the advantages of multiple staging are recognized,
> the problem of low voltage gun design breaks easily into
> two parts: the extraction stage and the deceleration stage.
> These designs can be done almost independently and can
> make use of time-proven electron optical elements. 

Amazing.

I still haven't been able to fully wrap my head around the energy dynamics of these devices. 

Let's draw a diagram.

```
E-beam:

~0.2 eV Emission 														  /\
----------------							100 eV Exit    			  'o  |
				\						--------------              -----   
				  \					  /  								Hapless victim
				  	\			 ___/   									Tree
				  	  \--------/
			~500 eV Extraction	   Focussing, Deceleration

```

That seems quite sensible. If one were to roll a ball down this mount improbable, an observer by the tree would only be struck with 100 eV. 

Intuition still rejects this, however. Consider the following: if the second half of the electron gun vanishes, the thoughtful vacuum scientist will have at hand only a 500 eV beam and a shocked expression. If a small plate is surreptitiously inserted centrally, will it not be heated by 0.003 mA * 500 eV = 1.5 W ?  

But then intuition is but a manifestation of a poorly adapted primate, and can only guide one so far in matters of science. I am missing something critical.

In any case, I then simulated this arrangement with IBSimu. Worked admirably - some minor tweaking required, but nothing serious.

![beam](../../files/ionprinter/simulation/IBSimu/simulations/legun/success/paper/beam.png)

![diags](../../files/ionprinter/simulation/IBSimu/simulations/legun/success/paper/diags.png)



#### Erdman-Zipf gun construction

A 1/4" end mill was used to drill the central features - a slight discrepancy from Erdman and Zipf's design, but simulations did not show any effect.

As the mini-lathe used did not have an adjustable tool post, a parting tool holder was ground down to fit. 

Machining from a 1/2" stock of copper was attempted; however, the lathe in question (or, more likely, the technique of the machinist) was not capable of machining to any reasonable surface. One particularly aggressive parting operation even caused the fuse to blow. 

The copper was exchanged for 360 free-machining brass, and the parts were turned to within ~0.3 mm tolerances. Mill-turning using the CNC would be ideal for rapid prototyping and iteration.

The zinc content of the brass was quite worrying, especially since the extractor will sit ~0.2 mm away from a tungsten wire at some 2000 K. Nickel was therefore electroplated onto the electrodes. The solution drew between 0.05 and 0.1a. 

Discussion on Vacuum Hackers indicated that outgassing would be excessive. This idea was abandoned.

12L14 was found to machine quite well on this lathe; however, lead outgassing may be a concern.

Brief diversion to ceramics: On a whim, tried pressing alumina with crude graphite dies. Not effective. Applying small amount of pure alumina to graphite also not effective. Mixed some gelatine and alumina in some random ratio, and applied to tungsten on a whim. Heated with propane torch. The gel-cast bakeoff was very clear and visible - really amazing. 

The 12L14 die corroded almost immediately. This effect has been referenced previously as one of the main shortcomings of leaded 

After ~6 months and likely several thousand tests, I can say the following with uncharacteristic confidence:

Al2O3 is a BASTARD. It lures you hither with tales of insulating properties, of wonderous high temperatures

There is aluminum dust everywhere

#### The hunt

A search was carried out in an attempt to find an insulating coating or electrodeposition technique that could be used for a few purposes:

- as insulator for electrostatic lens elements, required immediately
- to decrease bowtie emissivity (and, potentially, reactivity) without switching to EDM tungsten

Anodized aluminum could be used - many vacuum tubes have cathodes insulated by aluminum oxide. However, this is slow.

Water glass may also be an option

Things I know how to do:

- electroplate with copper
- electroplate with nickel



- Can nickel be passivated?

- Some nitride? Nickel nitride?

- Silica coating graphite seems to exist

- Iron/steel passivation or pickling could be used

- Anodization of aluminum with mere vinegar has been mentioned once or twice. 

- Graphite can be copper plated.
- Copper can be "calorized" - a layer of alumina is applied, which then reacts to form copper aluminate and a coating of alumina. Promising method. Little can be found in the literature about this.

<http://tallmantechnologies.ca/iron-making.php>

> Calorizing is a pack diffusion process where the copper tuyere is placed in a sealed retort containing aluminum powder. The casting is heated and aluminum is diffused into the surface, creating a copper aluminide inner layer and an aluminum oxide outer layer.
>
> Calorizing creates a protective layer on the copper tuyere that preventing liquid iron drips from penetrating the surface. The calorizing process is performed at the final stage of production after the tuyere is finished machined and hardfacing alloys are applied. Calorizing eliminates the need to use refractory materials on the top surface of the tuyere.

- io9 on twitter (thanks!) informed me of the the field-assisted sintering technique, which greatly reduces alumina sintering temperature requirements. 

US4418097A

> The present invention comprises a process for reducing high temperature oxidation of graphite electrodes by coating the electrodes with a siloxane fluid.

> Dimethylpolysiloxane, having 100 centistokes viscosity, was sprayed on a graphite electrode while the electrode was at ambient temperature. The electrode was then heated to 1400° F. for eight hours. Weight loss was 6.8%.

Oh wait! That's silicone oil - PMDS!

A bootstrap paradox is found here. All of these techniques will be trivial when the ion printer is functional, but it can't be built without one.

#### SiC Graphite / SiO2

A small sample of graphite was accidentally dropped into a test tube containing a small quantity of ECO-702 silicone fluid. This coupon was then heated with a propane torch. The fluid burst into flames, and produced a bluish deposit on the graphite which immediately flaked off. An inert atmosphere may be required.

One face of the graphite was cleaned, then heated cherry-red and dropped into the siloxane. Some momentary bubbling was observed. The sample appeared to change texture somewhat, and emerged perfectly clean. The sample was still conductive.

Promising method, however. 

Silicon Carbide has an emissivity close to that of graphite, so this technique will be useless for decreasing power consumption.

See @ blocher1957carbide for a comprehensive treatment of this treatment.

#### Alumina Cataphoresis

<https://www.thevalvepage.com/valvetek/heater/heater.htm>

> Cataphoresis or electrophoresis is defined as "the migration of coloidal particles under the influence of an electrical potential". Cataphoresis, as applied to heater coatings, is the process by which positively charged alumina particles are deposited on a negatively charged tungsten heater wire. The alumina used in the suspension consists of very fine particles, usually in the one-to-five micron size range. An increased number of ionised groups on the alumina surface results when the particles are surface charged by the addition of small amounts of selected soluble inorganic salts, such as aluminum nitrate.

> After the heater is coated, it is sintered at 1600C for a short time in a hydrogen-atmosphere furnace.

This process was attempted with graphite at 20v, no effect. Experiment was repeated at 500v, no effect. Solution was very dilute - agitation likely required. See @ lazic2004influence for parameters.

#### Bowtie temperature measurement

Precise print speed measurement (that is, atoms deposited per second) is essential to the accuracy of this concept. 

There exist some thermocouples A program called LPUWS was found. <https://lpuws.scot.sk/> The endoscope from the PnP machine was removed and connected to a pocket spectrometer; however, the resolution was inadequate. A source of a reference calibration spectrum is unknown. A laser or LED of a specified wavelength may be effective.

The bowtie temperature would be useful to determine the vapor pressure (and thus atoms emitted) of the deposition species. On second thought, the pressure in the nozzle depends on a choked-flow parameter, since the external pressure is essentially zero. Relying on temperature to determine print speed.

However, there are many options for print speed feedback. Trace light emissions from ionization, e-beam current attenuation, etc.

#### Aluminum anodizing

<http://www.observationsblog.com/sciencetechnologyexperiments/anodizing-and-dying-aluminum-without-battery-acid>

"Hard" or "type 3" anodization appears to be desirable for high-voltage applications. 

See @ mielke1987 for the breakdown voltages of various thicknesses - >2500v appears to be difficult to achieve.

Bath is generally refrigerated.

<https://www.precisioncoating.com/hardcoat-anodizing-type-iii/>

> "Sanford Low Voltage" process
>
> High voltage rectification normally requires > 30 Amps per square foot current density.
>
>  Dielectric barrier usually > 1000 < 2000 Volt DC.
>
> Hard coat applications require up to 75 V at 24–40 asf. 

A suitable boost converter was purchased.

#### 

The Steacie Library at York U has a shelf (QC 702 onward) with plasma physics books. 

"The Physics and Technology of Ion Sources" was particularly helpful.

#### 

It may pay to re-evaluate the reasoning behind the use of wire stock. Intuition says granular jamming will be a concern, but perhaps some sort of ultrasonic vibratory system could be used to prevent sticktion.

#### Emissivity difficulty

It was discovered that the emissivity of tungsten varies significantly with temperature, starting from 0.05 at 250 K to approximately 0.16 at 1500 K. With 2048 bowties, the 

An aluminum reflector could be used - however, thermal heating is a little counterintuitive. For instance, an isolated, completely enclosed object will eventually reach precisely the same temperature as its surroundings. Some simulations are in order to determine what reflector arrangement will be effective.