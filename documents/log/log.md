# Log introduction

#### 1545020962 >

I got out of university today. Let's see if we can get this functional before the next semester starts.

<hr>

Based on last night's progress on the ion beam focus theory, I'm doing some beam simulations with IBSimu to see exactly what kind of magnetic field I'm going to need. 

http://ibsimu.sourceforge.net/examples/analytic_bfield.html

![jackson](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/jackson.png)

If you do things just wrong, you get this lovely output. Friggin' Jackson Pollock #4, that's what that is.



Some great info from[^5]

[^5]: Doni, Michele. "Optimization of the ion extraction from an inductively coupled plasma (ICP) by IBSimu modeling." (2013). [Internal](../../references/1545067618.pdf) [External](https://www.politesi.polimi.it/bitstream/10589/81162/3/thesis.pdf)



IBSimu doesn't seem to say anywhere what the bfield units are. I'm assuming they're tesla, since we're running SI and not CGS? To verify, I'm going to check the radius:
$$
r=\frac{mv\sin(\omega)}{qB}
$$
Main beam velocity:
$$
9990.616\text{ m/s}=\sqrt{\frac{2(15eV)}{29\text{ amu}}}
$$

$$
\text{if } B=1\text{ Tesla } r = \text{0.0030 m (3 mm)}
$$


Inconclusive. I can't tell if the path curvature is due to space-charge effects or due to the magnetic field.

https://sourceforge.net/p/ibsimu/mailman/message/28479056/ says SI units are used.

Ah, here we go. Units are definitely Tesla.



![null](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/null.png)

Here's what we get with no magnetic field, 15eV, 1eV normal and transverse.

![1T](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/1T.png)

 1T field in +Z.

Putting a field in +Y crashes IBSimu.

A KJ magnetics RXC88 magnet has a peak 7T field:![ring3.50000-5](../../references/ring3.50000-5.png)

![7T](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/7T.png)

<hr>

Acceleration power:

$$
15eV \times 2.232×10^{20}\text{ particles/sec} = 536.4\text{W}
$$

Whew. That's a bunch. 

<hr>
$5.985eV$ will be emitted upon recombination - almost assuredly in the form of a photon with a wavelength of 207nm. That's intense. Probably can't put a window on this thing, though glass and polycarbonate both block almost all  far UV. Have to see if the OD is high enough for safety.

<hr>

Electrostatic focusing seems to be completely ineffective at these high current densities.

![no_effect](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/no_effect.png)

The LHC's beam current is only around 0.54A. Our beam current will be roughly 66.2 times higher!

<hr>

Oh, if your cylindrical BC is wrong you'll get bad data. Doesn't affect the above.

<hr>
We've got around 7 cm of chamber radius as our maximum focus constraint.

![gradient](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/gradient.png)

A 5T gradient just about does it.



Remembered that accelerators use quadrupole and sextupole magnets for focusing. Researching why. 

Man, that is killer. A ~3t field for 0.2+ m. That's going to be tricky to pull off without superconductors. How's the focusing after that?

![partial_gradient](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/partial_gradient.png)

![full_gradient](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/full_gradient.png)

Yup, that all makes sense, looks good. Pretty stable at 35.75a of beam current and 15eV. Good stuff.

![gradient_30eV](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/gradient_30eV.png)

The exact same test at 30eV or 5eV accel. produces identical plots - as expected via analytical methods.

Nulling out the space charge effects doesn't seem to have a large impact on the trajectories. Very strange.

Same for the epot.  

Same even for the bfield. Huh.

Without:

![no_scharge_epot](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/no_scharge_epot.png)

With:

![with_scharge](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/with_scharge.png)

Okay, so there is a slight difference. I guess the scharge effects just aren't big enough yet.

![narrow_input](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/narrow_input.png)

Narrowing the input makes the output wider? Le what? Hmm, might be an artifact of the different magnetic field.

![narrow_long](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/narrow_long.png)

Looks roughly the same. Let's try a lower field with a longer distance to convergence.

![no_scharge_long](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/no_scharge_long.png)

Yeah, our paths are definitely straight now.

![gaussian_2](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/gaussian_2.png)

Using a crude gaussian/gaussian distribution for the magnetic field now. Strange "crossover" effect which remains even if the magnet is moved further away.

![loopy](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/loopy.png)

20T may be slightly too strong.

![crossover_10t](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/crossover_10t.png)

Yeah, still happening. I'm sure future readers have written me off as an idiot. Oh hey, is the magnetic field being mirrored across the x axis? I'm not sure. 

Let's first get a slightly more accurate magnetic field curtesy FEMM.

Ooh, https://www.particleincell.com/starfish/ looks really nice. Later.



<hr>

### 1545147474 >

<hr>

Some ideas for ease of servicing:

Each control board can have an analog mux that takes ~16 voltages and pipes them into the main controller for self-test functionality.

There could be a little multimeter probe going to an ADC so that the user doesn't need an oscilloscope to troubleshoot the system.

A hall probe can be attached to the X/Y/Z? gantry and used to compensate for the magnetic field.

What kind of build platform are we going to use?

 Rulon or Delrin AF could be used for the linear motor slides.

<hr>

> The use of *FIELD_MIRROR* in case of symmetric cases, where beam is traversing next to the geometry boundary, is necessary to get physical results.

Ah.

Tried out 3d: A little bit too slow. IBSimu is only using one thread for some reason.

How about 2d?

I remembered that the UMFPACK lib wasn't installed when I configured this install of IBSimu. Installing and reconfiguring now - perhaps that'll speed things up a bit.

I love this processor. -j16 is so ungodly fast.

Had to compile SuiteSparse with 

`export CXXFLAGS="$CXXFLAGS -fPIC"` 

Nope, actually it's 

`SET(CMAKE_POSITION_INDEPENDENT_CODE ON)`

in some SuiteSparse cmake file. But that file doesn't exist anymore.

<hr>

Several hours later...

I'd already installed umfpack5 via apt, but it apparently missed the pkg-config .pc file. 

Ended up having to run `sudo dpkg -L libumfpack5`then write my own package description in a .pc and append it to the `PKG_CONFIG_PATH`.

Good stuff.

Anyway, cylindrical geometry is now unreasonably fast - 5 or 10 seconds.

Scratch that! Had the same solver enabled. The importance of controls and an impartial observer! That was some reprehensible science.

Okay, no, *that* was unreasonably fast. Reconfigured and reinstalled everything and now UMFPACK is definitely up and running.

<hr>

![no_collision](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/no_collision.png)

This crude 2D sim shows clearly where those spurious trajectories were coming from -  there's no collision at the boundaries.

Ah, and that's due to the PDB mirror values. Cool!

![now_talking](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/now_talking.png)

Here we go! A double-gaussian field with recombination.

![more_polish](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/more_polish.png)

Gradually tightening focus. I'm going to automate this process now.

I'm a little rusty in C++, apparently. Tried using fmt:: to procedurally generate filenames. 

I thought there was an easy way to batch-convert svgs to PNGs with linux, but apparently not - all suggestions require inkscape, which'll surely be quite slow. Switching to PNG export.

<video src="../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/bscan.mp4"></video>

BEAUTIFUL - a scan from 0 to 10T by .1T. Took somewhere around 2000 seconds.

<video src="../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/xbscan.mp4"></video>

![accidental](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/accidental.png)

Trying some piecewise functions.

What effect does the transverse temperature have on the beam?

Quick 20T test (no transverse or normal eV):

![20t](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/20t.png)

And 10T, for comparison:

![10t](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/10t.png)

Field strength doesn't seem to have a huge impact on the dot size beyond a certain point.

Let's get some better magnetic fields.

### 1545366135 >

<hr>

Exported magnetic field from FEMM using export.lua from https://www.particleincell.com/2012/femm-export/

Oh, right, I wanted to fix the vtu file export for Elmer. Next time.



Import's working, looks a little broken though.

Ah, just messed up the scaling.

![bfield_import](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/bfield_import.png)

![import_femm1](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/import_femm1.png)

Gorgeous!

Now adding recombination...done.

Oh, hang on. We're mainly interested in the Z-axis magnetic field.

![transposed](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/transposed.png)

I've temporarily transposed X for Z for some rough calculations - I'll probably have to spin up an Elmer 3D field calculation to really do this properly.

FEMM uses positive and negative turn numbers to denote solenoid polarity.

![aha](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/aha.png)

All right! Transposed Y for Z, some hundred amps going through two triangular coils...

![further_recombination](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/further_recombination.png)

Looks like there's a bit of beam distortion, presumably due to the strange coil shape:

![triangular_coil](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/triangular_coil.png)

Let me fix that.

![oh_my](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/oh_my.png)

Oh my! I may have set the coil current slightly too high. On the plus side, though, the beam is extremely tightly focused!

![slightly_more_sensible](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/slightly_more_sensible.png)

Oookay. Lots going on here. First, that's a friggin' 16 T field - surely I can whittle that down a bit. Secondly, it looks like the bulk of the beam focuses really nicely, but there's a few stragglers that are causing issues. And third, how the *heck* do you make a "Z axis" magnetic field? I still have to wrap my noodle around that one. Presumably CRTs are able to do it, but I haven't yet found a good description of CRT focus coils.

Ah, the 16T is only at the very periphery of the sim - other areas are ~8T.

Space charge effects are getting serious here - $0.004 \text{ C/m}^3$ peak. 

![magnets_relocated](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/magnets_relocated.png)

Spurious streams subsequently shifted. I moved the coil slightly further from the beam area. 

http://www.virginia.edu/ep/SurfaceScience/class2.html - neat resource on some vacuum theory.

((0.001 millipascals / ((ideal gas constant) * 270 Kelvin ))/10) * 6.022*10^23 = 2.6x10^13 molecules

![widened_magnets](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/widened_magnets.png)

Even less distortion. Lowish Bfield.

### 1545432643 >

<hr>

[Here](http://www.labguysworld.com/5FPn_CRT_Tester.htm) is some good discussion of CRT magnetic focus setups.

![FocusCoils_000](../../references/labguy/LabGuy's World_ 5FPn CRT TESTING_files/FocusCoils_000.jpg)





Running a quick axisymmetric FEMM sim to see what kind of field strengths would be acheivable with a copper coil. 
$$
Turns: 0.02m * 0.05m * 0.33mm^2 (22AWG wire cross sectional area) * 0.025m^2\approx3000
$$

$$
\pi*0.1*2=376m = 100 ohms
$$

All that for a measly half-tesla:

![measly](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/measly.png)



### 1545976211 > 

<hr>

Building up some intuition from the simple solenoid equation:
$$
\text{(magnetic constant) * (1000 turns / meter) * 100 amps = 0.1257T}
$$
17 Ga wire (cross section of 1mm^2) 

Dammit, I really don't want to use superconductors. Ferrous core?

What happens if I extend the coil the entire length of the chamber?

Would liquid-cooled brass tubing work? We can use the magnet coil as part of a high current transformer.

<hr>

Oh! The main limitation to using this device as a plasma beam cutter would probably be the tendency for the ablated material to foul the vacuum chamber in short order. We can apply the same ionization principles to the cutter region - perhaps a high voltage could be applied to the build plate to draw the waste to a sacrificial plate of some sort? This would, of course, require quite a drastic re-design of the build platform. 

<hr>

Naming:

`Neutralized / Recombinant Directed Deposition of Ions`

`Ne/RDDI`

`N/RDDI`

<hr>

There don't seem to be any non-conductive elements that are solid at room temperature. It'd be really great

<hr>

Can we spray other elements at the beam after recombination in order to create compounds? Better yet, can we have multiple beams? 

<hr>

100eV, wide coil:

![beam](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/beam.png)

15eV, same coil:

![15eV](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/15eV.png)

1000eV, same coil:

![1000ev](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/1000ev.png)

<hr>



![narrow](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/narrow.png)

![narrow2](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/narrow2.png)



<hr>

Could I sinter copper powder and solder to make the solenoid? Possibly. McMaster-Carr 

Solenoid could also be made out of magnetic iron?

<hr>

My current chamber only allows us about 0.3m in which to focus. I might have to upgrade that later.

<hr>
[Mark H on Physics StackExchange:](https://physics.stackexchange.com/a/249212/101785)

> Solenoid magnets can focus in both the X and Y planes, unlike quadrupoles. They are also simpler devices that don't require the precise alignment that a FODO array would require. The limitation on solenoid magnets is that they are only effective at focusing low-energy beams, like the positrons from your spallation source or primary electrons after being emitted from a cathode.

As expected. Cool.

<hr>

Use 3d printer to get cool macro pan shots

Taking advantage of vacuum for cooling; evaporative cooling? Small amount of liquid is injected into the foreline and evaporates, chilling the port. The water would probably wreak havoc on the roughing pump - some sort of trap could be added, but that would also have to be cooled. A separate refrigerant pump might be simpler.



<hr>

Now that I think about it, our IBSimu starting beam is really quite unphysical, in that it's almost perfectly collimated; there's no non-parallel components. Our beam won't be nearly as clean, and therefore won't focus quite as well as in the simulation.

<hr>

Trying Einzel lenses again. 

![einzel_test](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/einzel_test.png)

Interesting. Aperture-style lens, 1000v:

![iiinteresting](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/iiinteresting.png)

Neodymium ring magnet:

![neodymium](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/neodymium.png)

It looks like something's wrong with the field here. Very strange.

![better](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/better.png)

Ah, much better. Changed the magnetization axis. That will be a seriously huge magnet, though, which'll cause a lot of hassle during shipping etc. Let's try some more electrostatics.

<hr>

I had decreased the beam radius to 0.005m for the last few tests. I've brought it back up now.

<hr>

http://www.massspecpro.com/technology/ion-optics/einzel-lens-0 has some good discussion on einzel lenses. It mentions a process called "thermalization" in an "ion guide" to narrow the distribution of stuff. 

http://www.binep.ac.ru/Publics/Pdf/1997_p013_e.pdf

Interesting - this seems to largely concern mass spectrometers with very high input pressures.

<hr>
1 mil is 25.4 microns, for reference. 

<hr>

At 0.05A of beam current, an einzel lens with 1000v on the center cylinder looks like this:

![lowcurrent](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/lowcurrent.png)

At the full 35.75A, 

![fullcurrent](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/fullcurrent.png)

It looks like the beam is just too *wide* for the electrostatic field to have any significant effect. Dropping the grid size to 0.005 and reducing the beam and einzel radii yields:

![narrow_einzel](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/narrow_einzel.png)

Voltage is too high - it just repels everything. 

<hr>

What kind of energies will we have upon exiting the bowtie?
$$
V_{rms}=\sqrt{\frac{3kt}{29amu}}=\sqrt{\frac{3 * \text{boltzmann constant} * 2800K}{29 amu}}=1551.88 \text{m/s}
$$

$$
E_{rms}=0.5mv^2=0.361\text{eV}
$$

$$
\frac{0.361eV}{15eV} \approx 0.024\%
$$

Almost definitely completely negligible! Probably. Maybe. We'll see.

<hr>

### 1546128786 >

<hr>

https://cloudfront.escholarship.org/dist/prd/content/qt7652n8md/qt7652n8md.pdf

<hr>

Many ion beam systems have requirements that differ considerably from this project. Electron microscopes, for instance, generally operate with very low charge densities. Klystrons, however, happen to have electron guns quite similar in principle to our system.

https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19650023758.pdf

Jackpot! This reference describes some analytical methods for electrostatic focusing. They're optimizing for a 0.2A beam current, but what's an order of magnitude or two between friends?



> There were a number of aspects of the demountable nature of these experiments
> which caused some difficulty. Since the system was not bakeable, cathode activation
> took considerable time, and the repeated letting down and reactivation of the tube took
> more time than it would have in a bakeable system. A number of precautions were
> taken to minimize this difficulty, and as a result vacuum during operation was always
> held below 10 -7 -8 torr, and in fact typical operating vacuum was closer to 3 x 10 torr. 



<hr>

https://inis.iaea.org/collection/NCLCollectionStore/_Public/27/019/27019494.pdf

holy DARN 350A of beam current - $12.8A/cm^2$ on the cathode!

<hr>

https://dspace.mit.edu/bitstream/handle/1721.1/93971/01ja008_full.pdf%3Bjsessionid%3D562FEC11C8604EE9488C26217EBAADF2?sequence%3D1

<hr>

Some terminology: `perveance`is the coefficient between the space charge current`I` and the acceleration voltage $U_a$. Confusingly, `microperveance` is `perveance` * $10^6$. That's a little silly, isn't it?

<hr>

.[^6] is really cool:

> [snip] In principle, beam confinement may be achieved with either magnetic fields or electrostatic fields. In practice, however, almost all klystrons utilize magnetic focusing [snip] The minimum amount of magnetic field required to maintain a "pencil" electron beam at a constant diameter is the Brillouin field. The Brillouin field in gauss is 

$$
B_b = \frac{8.3\times10^{-4}I^{1/2}}{\text{V}^{1/4}r_\text{(inches)}}
$$

I am unsure as to whether this also applies to heavy ions, such as aluminum; however, let's give it a try and see whether we get reasonable results - the particle mass may well drop out of the equation during derivation.
$$
\frac{8.3\times10^{-4}35.75^{0.5}}{15^{0.25}\times0.196}=0.0128 \text{ Gauss}
$$
I guess not. That's absurdly low.



[^5]: Garrione, Maurizio, and Manuel Zamora. "Periodic solutions of the Brillouin electron beam focusing equation." *Commun. Pure Appl. Anal* 13.2 (2014): 961-975, [Internal](../../references/garzam_preprint.pdf) [External](https://www.researchgate.net/profile/Manuel_Zamora3/publication/258237448_Periodic_solutions_of_the_Brillouin_electron_beam_focusing_equation/links/00b7d52865c3eeac4a000000/Periodic-solutions-of-the-Brillouin-electron-beam-focusing-equation.pdf?origin=publication_detail)

<hr>

Klystrons also encounter the "target heating" effect that we will have to contend with - the collector is generally a liquid-cooled copper cup. ==Very interestingly, the target can sometimes have a negative voltage applied - a "depressed collector"== - which reduces the power dissipation. Very interesting! It may be possible to do something similar in our application.

https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19710007110.pdf

[^6]: Handbook of Microwave Technology: Volume 2, T. Koryu Ishii

Cathode life seems to be associated with something called a "miram curve". 

[^7]: CATHODE LIFE PREDICTION, ARC Professional Services Group,  https://apps.dtic.mil/dtic/tr/fulltext/u2/a245697.pdf

<hr>

[^8]: [External](https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19810021815.pdf)

> more generously radiused electrodes

How titillating!

<hr>

![1500ev](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/1500ev.png)

1500eV test with a very small beam. 640v on the einzel electrode.

![15eV_smallbeam](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/15eV_smallbeam.png)

Same situation with 15eV.

*weird*.

<hr>

Oh, since einzel lens descriptions generally assume a negatively charged beam, we need to swap the voltages.

Dropping the beam current enormously and swapping the element polarity as above produces the following:

![low_current](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/low_current.png)

But this lens is ineffective at high beam currents.

Hmm, I have a feeling that the grid/mesh size is affecting the charge density equation near the beam entry.

0.001m grid, 0.01m beam radius, 15eV, 35.75A:

![001m](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/001m.png)

0.0001m:

![0001m](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/0001m.png)

Yup! Definitely a significant difference.



https://link.springer.com/chapter/10.1007/978-1-349-00447-8_6

Solver crashed at 0.00001m grid size - OOM.

Decreasing the beam radius to 1mm seems to take away some of that - ahem - "generous radius".

![1mm_beam_5e-5grid](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/1mm_beam_5e-5grid.png)

I'm not sure if that's due to the mesh size interaction with the space charge simulation. 

This simulation (with a 0.00005m mesh) has a beam entry space charge of $1.1 C/m^3$, and an entry epot of 200kv.

At 0.0005m, epot=40kv, and scharge=0.3C/m^3.

I mean, it makes perfect sense that we're not having any effect - a few hundred volts on a hundred-kilovolt space charge is essentially negligible. 

![null_wide_finegrid](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/null_wide_finegrid.png)

> Well let me bring you back to the subject / Pep's on set / increase your RMS kinetic energy and provoke evaporative cooling

<hr>

The epot remains at around 200kv even with a 0.01m beam radius. That doesn't seem correct.

> If space charge is now included, a uniform beam of density p generates a field 

$$
E_p=\frac{p}{2\epsilon_0}y
$$

What the hell is `y`? Label ya variables, son! Surely it's not translational position?

<hr>

Ooh, this is a great source:

[^9]: Applications of Electrons, Ions and Atomic Beams, Dr. T J Reddish [External](http://web2.uwindsor.ca/courses/physics/reddish/teaching/587/Space-Charge-I.pdf)

Electric field at the surface of a cylinder of charge is given by:
$$
v=\sqrt{\frac{2eV}{m}}=\sqrt{\frac{2e\times15\text{v}}{29 \text{amu}}}=9990.616 \text{ m/s}
$$

$$
D_c=\frac{I}{v}=\frac{35.75A}{9990.616 m/s}= 0.0035784 coulombs per meter
$$

$$
E_r = \frac{D_c}{2\pi\epsilon_0r} = 6.43 GV/m
$$

Not quite what I wanted.
$$
\beta=30329.5\frac{I}{V^{3/2}}=0.615\\
$$
Where $$\alpha$$ is the initial convergence angle - 0 for a parallel beam, which ours isn't quite going to be.

<hr>

[^10]: http://www-eng.lbl.gov/~dleitner/USPAS_2016_Fundamental_Of_Ion_Sources/4_Thursday_AM_SpaceCharge.pdf

I've heard tell of "rf focusing". 

<hr>

Let's try making this thing *way bigger*.

http://accelconf.web.cern.ch/accelconf/l02/AUTINDEX/..%5CPAPERS%5CTU465.PDF

Describes a "gun coil" around the electron gun. This device runs at 115-380A of beam current. How the hell do they focus the thing? Their simulations look so pretty and tight.

https://arxiv.org/pdf/physics/0503145.pdf

https://arxiv.org/pdf/1401.3951.pdf

Maaaan this is so close:

https://patentimages.storage.googleapis.com/83/c5/bf/e627ab33fda8e4/US6768265.pdf

Describes electrostatic "lobes" that guide the beam away from the cathode.

BUT HOW

https://www.researchgate.net/profile/Sergey_Kazakov/publication/265074526_THE_TOSHIBA_E3736_MULTI-BEAM_KLYSTRON/links/5437cdca0cf2590375c55628.pdf



https://arxiv.org/pdf/1510.06065.pdf

Nice breakdown of the coil currents in a high power klystron. We don't need the main or lens coils, more than likely; the gun-coil is all we're going to need, which drew a reasonable 42 amps. I'm assuming the "gun current" is ~50a based on one of the charts.

http://accelconf.web.cern.ch/accelconf/ipac2016/papers/mopmy015.pdf

Many references can be found to a software known as "DGUN". I can find "EGUN", a FORTRAN program written in 1979.

http://www.slac.stanford.edu/cgi-wrap/getdoc/slac-r-226.pdf

I can only assume that DGUN is a more modern version.

<hr>

### 1546365491 >

There's something wrong with my FEMM sim. An N52 ring magnet:

![huh](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/huh.png)

But here's the depiction on KJ Magnetics' website:

![noope](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/noope.png)

This magnet has a peak field of 15T. What's up with that? 

I can cheat by raising the coercivity to an unrealistic value - the focusing works really well in this case, but there's obviously something wrong here.



1 T = 10,000 gauss. Now everything makes sense.

God*damn.* What an idiot.

<hr>

So, how can I get around this problem?

- I could split the beam into 10 parts, each focused separately.

- I could just make one enormously powerful solenoid

- I could make a hollow beam, and then focus after recombination. Let's try that out.

I'm not sure how the acceleration "grid" will work during fanout. 

First I have to fan the beam out - let's use a backwards ring magnet for that:

![did_just_work](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/did_just_work.png)

Not enough to fan out yet... but the focusing is significantly better, strangely. What's up with that? This sim had a very fine mesh of 0.0001m, so it's not an incomplete scharge effect as previously observed...

![whaaaat](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/whaaaat.png)

Now that's particularly interesting.

Varying the beam start position changes the beam focusing power significantly. Here's what the field looks like.

![huuuuh](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/huuuuh.png)

Recall that the Xb/Zb coordinate axis and field vectors are swapped in FEMM vs IBSimu. 

I think it's time for some **iterative testing.**

> wicked wicked, had to ask for the digits
>
> ffmpeg -framerate 3 -pattern_type glob -i '*.png' xposscan.mp4

<video src="../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/xposscan.mp4"></video>

Not sure why some frames in the video are out of order - some issue with ffmpeg glob parsing? Anyhow, it serves to illustrate my point. 

![mistake](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/mistake.png)

Oh, here's what a zero-field test looks like. That explains everything.

Beam radius of 5 mm, for reference.

![zero_field_2](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/zero_field_2.png)

Test with scalar B rather than B1 or B2 in export script:

![bent](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/bent.png)

Curious.

 ![another_null](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/another_null.png)



https://ilc.kek.jp/Conf/LAM27/8P-13.pdf

clearly shows the first cathode coil as being significantly behind the cathode. 

> The cathode coils produce a uniform magnetic
> field on the whole cathodes. 

![accidental](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/accidental.png)

I was trying to fanout, but this is pretty neat. Mirrored across midpoint - the two downstream magnets have a 180 deg magnetization direction (counter-beam).

![backshell](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/backshell.png)

Trying an iron backshell, but that doesn't quite work.

![fanout_succesful](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/fanout_succesful.png)

Haha, looks like I've got fanout working! This is with three pieces of iron on the top, bottom and middle gaps respectively.



Oh man, I just realized something. We're going to encounter the exact same problem during recombination; the electrons are going to go *everywhere*. Oh well. We can use this to our advantage, I guess - direct the electrons to a thin slice of the beam.



Higher beam energy lets us bring the field strength up before the beam goes all screwy.

![we_have_focusing](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/we_have_focusing.png)

Hooray! Full current focusing! Lots of tweaking to do, but this'll work fine! Very simple linear focus structure with KJ RY0X04 magnets - though this sim was done with N52 grade material. These cost 16 bucks apiece in low volume, a total of $64 - totally reasonable!

Objects that produce a field greater than 0.00525 gauss at 15 feet away cannot be shipped via air, which would make international orders a bit of a hassle.

<hr>

I forgot to remove a x10 multiplier. All is lost.

<hr>

Hang on a min. Since this is a 2d simulation, what happens when the beam spreads out? Rather than going like $$r^2$$ , won't it just go like $r$? 

Let's get cylindrical.

<hr>

![no_effect](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/no_effect.png)

No effect.

<hr>

Increasing magnetic field by 10x has a far more significant effect than reducing beam current by 10x.

<hr>

Why did I have 2pir^2 as the area formula? What an idiot.

<hr>

An annulus with an IR of 0.05 and an OR of 0.005 has an area of $0.00165m^2$. Compare with a simple circle, which has an area of $7.85\times10^{-5}$. A hollow beam with these proportions will have a 21x lower current density.

![not_working](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/not_working.png)

This should work. 

Increasing the beam IR to 0.1 only decreased the epot from 500kv to 447kv, and the scharge dropped from 0.1 to 0.078 - even though the area decreased by 

3.22×10-3

https://arxiv.org/pdf/1401.3951.pdf page 17

Page 26 contains the phrase "plasma meniscus", which might be my favorite statement ever. 

Charge density is 


$$
\frac{I}{Av} = \frac{35.75}{0.00165m^2*9990.616m/s}=2.169 C/M^3
$$
Whaaat? That's several orders of magnitude higher than I've ever seen. Is my simulation that screwed up?

<hr>

Oh, interesting - beam deflection is significantly affected by particle mass. A mass of 0.01 amu is easily bent around:

![lowmass](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/lowmass.png)

Higgs manipulation notwithstanding, there's nothing I can possibly do to change the mass.

This makes sense given that the beam deflection is given by 
$$
\theta=LB\sqrt{\frac{q}{2mV}}
$$
`L` being the length of the magnetic field, and `V` the beam voltage.

Without scharge effects, a single aluminum ion would be bent by 
$$
0.05 meters \times 0.35 T \times sqrt(1 electron charge / (2 * 29 amu * 15 volts)) = 5.8 degrees.
$$


| L (m) | B (Tesla) | V (volts) | $\theta$ (degrees) |
| ----- | --------- | --------- | ------------------ |
| 0.05  | 0.35      | 15        | 5.8                |
|       |           |           |                    |
|       |           |           |                    |
|       |           |           |                    |

==Ah! We're not space-charge-limited, just magnetic field limited!==

Screw this hollow-beam stuff - we just need a bigger magnet!

Daniel Correia, goddamn idiot.

I guess even an excessively simplistic analytical method can tell you more about the *character* of a problem than an accurate numerical simulation. This puts my ignorance of the majority of physics into stark contrast. F***ing idiot. I need to hit FLP some more.

<hr>

### 1546479874 > 

<hr>

I guess the optimization problem here is finding the minimum magnetic field that prevents the beam from contacting the magnets. As the magnet radius goes up, we have more time and length in which to bend the beam - but the field also goes down.

<hr>

Page 23 of  https://arxiv.org/pdf/1401.3951.pdf describes a Pierce geometry beam extractor.

I wanna play with that. Let's try winding the simulation back a bit to when the metal atoms have just been ionized by electron collision, where they have a typical thermal velocity of 0.361 eV. Let's say the spread is 0.1eV. Given these parameters, the beam will be bent by 37.6 degrees in a 0.35T field.

Oh, and let's also switch back to a non-cylindrical beam.

5.985 eV (the first ionization energy of aluminium) is our absolute minimum electrode voltage.



![0361ev](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/0361ev.png)

Huh.

| Energy (spread)    | Radius  | Mesh     | scharge   | epot |
| ------------------ | ------- | -------- | --------- | ---- |
| 0.361 eV (0.1/0.1) | 0.005 m | 0.0005 m | 0.9 C/m^3 | 9 MV |

Oh, I made a mistake in setting the beam area.

![barely](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/barely.png)

| Energy (spread)    | Radius  | Mesh     | scharge     | epot  |
| ------------------ | ------- | -------- | ----------- | ----- |
| 0.361 eV (0.1/0.1) | 0.005 m | 0.0005 m | 0.032 C/m^3 | 400kv |

Charge density (and therefore the repulsion force) goes like
$$
\frac{I}{\sqrt{\frac{V}{m}}}
$$




<hr>

| Material            | Tensile Strength |
| ------------------- | ---------------- |
| Pure 1100 aluminium | 75 MPa (11 ksi)  |



| Energy (spread)    | Radius   | Mesh      | scharge  | epot  |
| ------------------ | -------- | --------- | -------- | ----- |
| 0.361 eV (0.1/0.1) | 0.0005 m | 0.00001 m | 10 C/m^3 | 400kv |

https://inis.iaea.org/collection/NCLCollectionStore/_Public/08/297/8297280.pdf describes pierce-style extraction electrode configurations.

![50kv](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/50kv.png)

50kv is sufficient to pull a beam. The overall size of the electrodes has a significant effect on the required voltages, as one would expect.



![pierce1](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/pierce1.png)



![10kv_ineffective](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/10kv_ineffective.png)

But lower voltages pose considerable issues. Let's try magnets again!

http://inspirehep.net/record/710232/files/slac-pub-11688.pdf another interesting klystron extraction configuration, a "racetrack".



![halbach_iron](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/halbach_iron.png)

Ooh, pretty. Pseudo-halbach array with three N52 magnets and two iron pole pieces at the edges.

<hr>

Let's re-derive the Brillouin field for heavy particles, since I can't find it anywhere. 

Beam boundary force equation, #46 in https://arxiv.org/pdf/1401.3951.pdf:
$$
F = \frac{qI}{2\pi\epsilon_0rv}
$$

$$
F=qvB
$$

$$
B=\frac{I}{2\pi\epsilon_0rv^2}
$$

It should be clear at this point that I have no idea what I'm doing. 

Interesting - while the Larmor radius has a mass component, Brillouin focusing seems to be independent of mass. We might be able to get away with alloys if beam deflection is not required.

Oh, scratch that. Since everything's going to attain the same energy, the velocity will be dependent on mass anyway.

Plugging in some values,
$$
(35.75 amps)/(2pi*(vacuum permittivity)*(0.005 meters)*(9990 m/s)^2) = 1.29 megateslas
$$
Right. This indicates the necessity of incorporating a neutron star into my design. 

Great, so I have two estimates for the necessary magnetic field which differ by nine orders of magnitude.

ON THE DESIGN OF THE TRANSITION REGION OF
AXISYMMETRIC, MAGNETICALLY FOCUSED
BEAM VALVES

https://ieeexplore.ieee.org/abstract/document/5259415 

equation 4

One more time!
$$
B_b = \sqrt{\frac{\sqrt{2}I}{\pi\epsilon_0(\frac{e}{m})^{(3/2)}V^{(1/2)}r^2}}
$$
I'm such an idiot. I can't even get this simple stupid equation right. 

To validate my solution, I plugged in one of the klystron beam parameters that have been repeated ad nauseam above - using electrons in the mass term. This returned a required Brillouin field of 0.504 T - perfectly reasonable.

With a particle mass of 29 amu, a 1.75 kilotesla field would be required.

Well then.

| I (Amps) | V (volts) | Radius (m) | B (Tesla) |
| -------- | --------- | ---------- | --------- |
| 35.75    | 15        | 0.005      | 1750      |
| 1        | 3000      | 0.05       | 7.82      |

Let's check out a hollow beam for a sec. Just need to add in the annulus term in the bottom...

`sqrt((sqrt(2)*0.001 amps)/(((vacuum permittivity)*((electron charge)/(29 amu))^(3/2)*(3000V)^0.5*   ((pi*(0.1+0.005 meters)^2)-(pi*(0.1 meters)^2    )     )`



Well, here's the good part! The ion curtain will be easy to design, since it's just electrons!

yaaay

Capping our accel. power to 1000w, we can gain a bit on the voltage.

Any areas that have overspray can be coated in a layer (or a continuous pumped laminaresque stream) of silicone diff. pump oil to prevent adhesion 

<hr>

### 1546621015 >


<hr>

https://accelconf.web.cern.ch/AccelConf/p95/ARTICLES/FAQ/FAQ08.PDF

It should be noted that my Mk3 chamber's turbomolecular pump doesn't like magnetic fields. 

https://inis.iaea.org/collection/NCLCollectionStore/_Public/08/297/8297280.pdf

I realized last night that we actually *can* have a focus electrode at a few hundred kilovolts, because they don't actually need to provide any power, just produce a static field. The accel. electrodes can still be maintained at 15v, and supply the substantial amount of current required.

A Cockroft-Walton multiplier can be employed (likely placed on the inside of the vacuum chamber to lessen the insulation requirements) to produce this voltage.

Here's an interesting paper on high-temp CW supplies.

https://pdfs.semanticscholar.org/b23b/2032cd75a47770b857c7b7d0af440256e4e1.pdf

The beam can be deflected either by altering plate voltages, or by deflecting the recombination electron stream.

The only problem to be solved now involves the design of a Pierce-type electrode configuration. I don't trust my IBSimu simulation - I think the extraordinary current densities are not properly simulated by the mesh.

I'd like to verify the epot analytically.

Hmm, no, perhaps not:

Tests run at 0.361 eV input - a conservative scenario:

| Mesh     | Beam radius | scharge | epot   | Particle # | Current |
| -------- | ----------- | ------- | ------ | ---------- | ------- |
| 0.00005  | 0.005       | 0.073   | 109586 | 10000      | 35.75   |
| 0.000025 | 0.005       | 0.079   | 109702 | 10000      | 35.75   |
| 0.00005  | 0.005       | 0.09    | 109000 | 100        | 35.75   |
| 0.00001* | 0.005       | 0.161   | 57000  | 1000       | 35.75   |

*I had to alter the mesh dimensions to stay within memory limitations.

That looks pretty reasonable - scharge and epot seem to be relatively mesh-invariant at these small sizes. I greatly reduced the problem domain in order to simulate at these small mesh sizes - these tests were only 0.025x0.05m. The beam was offset from the left boundary by 0.015m.

http://www.rtftechnologies.org/emtech/cockroft-walton.htm

http://blazelabs.com/l-c-superv1.asp - a 14 watt 54kv C/W. They really seem quite simple.

<hr>

Safety is going to be a concern. X-ray production will probably be detectable as excess current draw on the C/W. A small magnetic field can be employed to restrain the ionization electrons, to prevent them from being accelerated by the focus electrode.

https://apps.dtic.mil/dtic/tr/fulltext/u2/a278139.pdf

<hr>

I'm a little concerned about the power that'll be required to focus. Keeping the beam in equilibrium takes zero power; W=F*D, so as long as the geometry of the electrode is such that the particles don't move along the force vector, no power will be expended.

Focusing the beam, however, will require power. This'll have to be provided by the UHV electrode, so we can only really afford ~20 watts. 

Let's assume a focus distance of 0.1 meters, and an initial beam radius of 0.005m.
$$
\frac{0.1}{9990 m/s} = 1.001001e-5s
$$

$$
= 499.5 m/s
$$

Uh oh.
$$
0.5 \times 29 amu \times (499.5m/s) ^2 = 0.0375 eV\\
\times2.232×10^{20}/s = 1.341 \text{ watts}
$$
Hell yeah! That's *totally* reasonable!

We're on the home stretch! I'm so giddy I can barely function right now!

I can add the recombination electron beam to the same simulation after.

<hr>

As above, we have to make sure that nothing can get near the HV plates - they'll be super sensitive.

<hr>
Had to switch back to BiCGSTAB solver for one test - UFMPACK doesn't seem to like allocating anything over 4Gb.

EarlyOOM is wonderful.

<hr>

Let's try confirming the sim analytically again. 0.005 mm beam:
$$
\frac{I}{Av} = \frac{35.75}{7.85\times10^{-5}m^2\times1550m/s}=293.67 C/M^3
$$
Oh no. That's not good :(

The simulation is using a beam area of 0.01m^2 in 2d mode. That's surely where my issues arise. Let's switch back to cyl. 

Now we have a beam area of 0.000079 - that's correct. Scharge is now ~2.5 at a mesh  of 0.00005 - very close to the ideal value - so we can be pretty confident in this simulation.

Epot, regrettably, is over 600kv, which'll be a bit tricky.

Beam energy doesn't seem to affect the sim? Curious.






$$
K = \frac{V_1}{V_0cosh(va)} = \frac{100 \text{ kilovolts}}{15 volts*cosh(1550 m/s * 0.01)} = 0.002473\\
charge density = e\epsilon_0V_0K^2v^2 \approx 4.76 C/m^3\\
\text{Substituting back into I/Av r}=0.039
$$
where `a` is the radius of the focus electrodes. r is greater than a, so 100kv is not sufficient to prevent collision.



https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19650023758.pdf

Gah, this is tricky. We're trying to make a Pierce electrode where the acceleration and focusing voltages are five orders of magnitude apart.

![whatsup](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/whatsup.png)

Strange!

If magnets are so ineffective, how can ion thrusters operate at similar charge densities with magnetic confinement? The atomic mass of xenon is 132, further exacerbating the problem.

The NSTAR runs at around 1.7 amps of beam current.

Oh, and there's a problem with recombination prior to exiting the accel. chamber. As long as the velocity difference between the electrons and the 

We can use a 

Goebel et al describes hollow cathodes for ion thrusters. It might be possible to add a small amount of barium oxide to some surface of the graphite nozzle which is cooler than 1900c (the melting point of BaO). Cathode poisoning might be an issue, however.

https://www.bnl.gov/isd/documents/82781.pdf

http://cds.cern.ch/record/176058/files/198704214.pdf

http://www.darshan.ac.in/Upload/DIET/Documents/EE/HV_2160904_Ch_2_05022018_045216AM.pdf

If only I knew what I was doing.

https://sourceforge.net/p/ibsimu/mailman/message/35502967/ 

> There should be 10 or more mesh units within the size of the beam to correctly model the space charge forces if that is what you want to do. Is your beam in micrometer size? If it is then you need simulation methods that are not available in IBSimu.

Aha! Excellent. Some actual data.

Page 256 of Goebel et al and 

https://trs.jpl.nasa.gov/bitstream/handle/2014/6401/03-0240.pdf?sequence=1

discuss mean free path and striking distances. Penning gauge papers might also be a good source, since they're quite similar. This turns out to be a fiendishly complex topic.

Penning/cold cathode gauges use a magnet to make the electrons spiral in a helical fashion in order to increase the odds of colliding with a neutral gas molecule. 

<hr>

Let's try this Starfish thing, hey? https://www.particleincell.com/starfish/

Supposedly, it can simulate supersonic systems - might be helpful for the diff. pump, too.

file:///home/arthurdent/Downloads/Bondarenko_Daniel.pdf describes a neat Colpitts RF generator. I like!

<hr>

Can I run a large current through the beam plasma channel in order to amplify the magnetic repulsion? A la Z-pinch?

https://apps.dtic.mil/dtic/tr/fulltext/u2/a057689.pdf Uh huh! 

Does this actually change anything? The lorentz F=ILB force will apply, I suppose.
$$
50A\times0.01 m\times 1 T = 0.5 N
$$
Okay, fine, but is that the force on each particle? Surely not! So would that be divided by the number of particles? Let's see:
$$
5\times10^{-21}N\\
F = \frac{qI}{2\pi\epsilon_0rv} = 2\times10^{-9}
$$
Nooope! Or, if so, we'll need a tremendous current to confine the beam. 

This also gets into some tricky magnetodynamics - possibly with a hydro stuck in there too. What's going to be the main charge carrier? What's going to feel the force? Seems like it should be the electrons - but Z-pinch works with heavy ions and deuterium too, so clearly not. And how do we recombine a super-hot pinched plasma in a magnetic field? We have to somehow match electron velocities within the ionization energy of the ions. AAAA!

Let's not, before I lose the plot.

Also:

![cpu](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/cpu.png)

<hr>

http://erps.spacegrant.org/uploads/images/images/iepc_articledownload_1988-2007/2013index/73a3ga1n.pdf

<hr>

Oh, ion drive neutralizers aren't interested in the recombination of the downstream gas per se - they only care that the spacecraft isn't gradually charged. The neutralizer voltage is therefore not particularly important.

<hr>
http://www.usp.br/massa/2014/qfl2144/pdf/Capitulo6_WatsonSparkman.pdf 
Mass spectrometers use a similar 

https://lib.dr.iastate.edu/cgi/viewcontent.cgi?article=1120&context=chem_pubs

Oh hey, there's a book called "Electron Impact Ionization" by T.D. Mark. 

http://articles.adsabs.harvard.edu/full/1967ApJS...14..207L/0000207.000.html 

> It can be said that there currently exists no rigorous theory of the electron impact ionization of atoms and ions

We're outside the environment again! Good lord.

<hr>

Well, perhaps there are some empirical fits that I can use. Indeed!

http://adsbit.harvard.edu/cgi-bin/nph-iarticle_query?1967ApJS...14..207L&defaultprint=YES&filetype=.pdf

<hr>

Starfish looked daunting. Let's try this SMILEI PIC code:

https://smileipic.github.io/Smilei/namelist.html



Here's an indication of how cool a program's going to be:

![supercomputer](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/supercomputer.png)

Simulation is 5% inspiration, 95% compilation.

Installed according to directions, copied hdf5 to the smilei dir (removing the lfs flag), set in makefile.

https://github.com/UCLA-Plasma-Simulation-Group/QuickPIC-OpenSource

<hr>

There's something seriously the matter with my intuition for where the accel. and focus power comes from. Discussion on space.stackexchange details how theoretically the acceleration grids in an ion drive require zero current. The potential energy already exists upon ionization due to the charge's position in the electric field. I can't believe I've labored under this misunderstanding for so long - the mark of a poor scientist.

<hr>

Bunch of focusing structures:

http://inspirehep.net/record/829291/files/slac-pub-13741.pdf

https://dspace.mit.edu/bitstream/handle/1721.1/93971/01ja008_full.pdf%3Bjsessionid%3D562FEC11C8604EE9488C26217EBAADF2?sequence%3D1

https://dspace.mit.edu/bitstream/handle/1721.1/93930/00ja005_full.pdf?sequence=1

http://accelconf.web.cern.ch/accelconf/r06/PAPERS/TUHO08.PDF

http://inspirehep.net/record/159/files/slac-pub-0056.pdf

https://patentimages.storage.googleapis.com/83/c5/bf/e627ab33fda8e4/US6768265.pdf

https://ilc.kek.jp/Conf/LAM27/8P-13.pdf

http://www.femm.info/wiki/PermanentMagnetExample

http://science.slc.edu/~mfrey/post/femm-simulations/

https://www.astro.umd.edu/~richard/ASTR680/Smith_school_Mar18.pdf

<hr>

I have three days left.

<hr>

SMili doesn't consider recombination. That sucks. Oh well, it'll work for the ionization chamber.

Had to downgrade openmpi to get hdf5.

<hr>
Seriously considering ICP again. Seems like it's a lot easier to get a high ionization percentage via ICP. I'm thinking of scaling the ICP coil down quite a bit, and making an RF-transparent alumina hot column that's attached to the bowtie. Anything that contacts the hot column will be melted and will simply flow back into the bowtie to be vaporized anew - but ionized gas can be drawn out separately somehow. A charged graphite cap with a nozzle?

<hr>

Hey, wait a minute. 

Arc welders have no trouble sustaining an arc, even with super low voltages. What's the ionization percentage in an arc welder?

http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.666.1290&rep=rep1&type=pdf is an interesting but not sufficiently detailed look at arc welding ionization processes.

<hr>

We're back in hardware mode. Milling gauge features on the 6040.

| Tool size | XY feed   | Z feed | RPM   | Z step | Stepover |
| --------- | --------- | ------ | ----- | ------ | -------- |
| 1/8"      | 7 mm/s    | 1 mm/s | 15000 | 0.5 mm | 50%      |
| 0.25"     | 11.6 mm/s | 1 mm/s | 14000 | 0.5 mm | 50%      |
|           |           |        |       |        |          |

<hr>

https://pure.tue.nl/ws/portalfiles/portal/2274668/588538.pdf a "root level" paper on a deposition gun.

<hr>
DigiKey sells these neat [ceramic spacers](https://www.digikey.ca/product-detail/en/essentra-components/CER-4/RPC1922-ND/3811721) - resistant up to around 1700c. A bit big for the bowtie.

<hr>

Resources on merging bodies with elmer:

http://www.elmerfem.org/forum/viewtopic.php?f=4&t=4111&p=14490&hilit=merge+bodies#p14490

<hr>

FEMM can display the inductance of a coil by pressing the "circuit properties" button. That'll be quite useful for the RF coil.

<hr>
Brazing to alumina is apparently doable. 

McMaster sells Kovar. 

<hr>



### 1547366450 >

<hr>

Ah! A flash of inspiration. Going to model this thing up...

Page 148 of Fundamentals has some really amazing info. I think I can pipe some FEMM data into the analytical models it provides. 

First, we move the bowtie stack off the feedthroughs. This reduces the paralleism requirements and allows us to use standards spark plugs for the high current connections.

Aluminium joints were previously unattainable - however, I've recently learned of the existence of aluminium brazing flux - available at McMaster! TIG is of course also an option - though it has the disadvantage of being rather more permanent than a simple braze.

http://home.earthlink.net/~jimlux/hv/sparkplug.htm

https://www.gsparkplug.com/shop/spark-plug-construction/

Spark plugs apparently use a talcum powder ring for sealing. Very unusual. I'm going to look into this.

In any case, the plug body is typically made of a nickel steel alloy. 

https://weldtalk.hobartwelders.com/forum/application-talk/aluminum/32053-brazing-or-soldering-aluminum-to-stainless-steel

All right! Looks like I'll probably have to weld the plugs in two stages - first braze a copper transition tube to the nickel, and then braze that to the aluminium.

I'm going all out here. We're talking diamond dremel blades, three types of brazing alloys - good stuff.

Designing for 0.33 or 0.41mm aluminium wire.

It might be possible to inject the wire at the top "cold" electrode. We'll see.

Hmm, no, it might be advantageous to have the alumina go through the top graphite block. 

No, scratch that - we probably want an acceleration electrode at the top of the column.

All we really need is to reach 700c throughout the length of the alumina column.

Oh, but copper melts at 1100c. That's what we're aiming for.

The two graphite heaters can be crossed against each other.

The low thermal conductivity of ceramic will aid in the heating efficiency of the bowtie.

Jesus christ, is it 4:30? Good lord.

We have to be careful that the bowtie connector temperature doesn't exceed the melting point of the solder. Might have to braze it in place. Oooooor have a thin copper strip extend quite far, and then bolt a wire onto it. That'll do!

<hr>


https://github.com/MahdiNabil/CFD-PC

https://www.cfd-online.com/Forums/openfoam-programming-development/157898-solver-evaporation-condensation-modeling.html

OpenFOAM has an interCondensatingEvaporatingFoam.

Elmer doesn't natively support evaporation.

Aha! We're actually modelling something like a resistojet. https://arc.aiaa.org/doi/pdf/10.2514/3.26431?casa_token=i2LW00ju9Z0AAAAA:uchPVgnfn9YSBm--KQTgm2lTBYmXeswyaFbrS1eIztJLepFaMLeJwP4E9Vx7JVefCHD8neRIV-U

Solid teflon electrothermal:

https://arc.aiaa.org/doi/pdfplus/10.2514/3.30181?casa_token=DyjNtM037tAAAAAA:1bzo0A4aWYGAuIgogxR7wylh7FNlZxqY3ZmLMSofx-KmzuRFwg6vRx3yb4VXw7w-PmYvkNRiWRA

https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19880015584.pdf describes some ways to get a more ideal "dry" gas stream from a vaporizer.

The NASA MOC nozzle designer requires pressure, regrettably.

"Z" in PIC is usually atomic number. "A" is mass number.

Installed piccante.

Piccante does not seem to support ionization yet. Installed AlaDyn.



Installed SMILEI. Very nice. Playing around with it now.



<hr>

Re: XY stage:

Standard bearings aren't likely to be happy running in vacuum; however, PTFE linear slides will probably work pretty well. We should be able to go pretty fast, since there'll be no air resistance.

<hr>

Let's get a rough approximation of the pressure in the vicinity of the bowtie. 
$$
2.232*10^{20} / 6.022*10^{23} = 0.0003706 \text{ moles} \\
P=(nRT/V)  = (0.0003706 moles * (8.314 ideal gas constant) * 2500 K) /((pi*r^2)*1550 m) \\
 = r=0.25 mm P=220 kPa / ~2 atm \\
 = r=4 mm P= 880 Pa / ~0.008113 atm \\
$$
Well, the dimensional analysis works out, anyways. I'm totally ignoring the nozzle, of course, and assuming that the thermal velocity is unchanged. I'm considering spinning up an OpenFOAM sim to get some better nozzle simulations.

Oh hey, now that I have the pressure I can use NASA's nozzle designer to look at plume shapes.

Meta: it'd be cool to be able to embed little calculators for different values - almost, but not completely unlike Wolfram's CDF format. Jupyter might be a good option.

### 

The rest of these notes will be written in the third person. 



A series of equations that determine klystron radiation exposure: http://www.slac.stanford.edu/cgi-wrap/getdoc/slac-pub-3719.pdf

It may be useful to re-examine the assumption that Al is an ideal metal. The space-charge density difficulties are reduced as atomic mass increases.

Pure copper has a tensile strength of around 200 MPa, and can be heat treated. With an atomic mass of 63.54 amu, only half as many ions would be required. Iron is also an attractive option, at 540 MPa and 55.84 amu.

<hr>

A depressed collector won't work in this situation - there won't be any charge on the beam! Idiot.

### 1548734507

It's looking like I will have to break the business end into a large number of individual bowties - several hundred, potentially, probably each with its own deflection/focusing amplifier but a collective RF coil.



Wire feed might get a little tricky in that case. I may have to switch to a liquid feed system, with graphite tubes that run throughout the bowtie. This would also allow us to use different input material shapes, like beads or ingots.

Having more than one bowtie assembly is actually really cool: it allows us to spread out the deposition heat transfer a bit (though we could just run a single bowtie a little faster) - but more importantly, it probably allows us to do alloys or even compounds. We'll see.

Oh neat! Jupyter includes mathjax.

The chamber plates can be deposited isogrid to make the whole thing lighter - might be difficult to maintain vacuum cleanliness if the grid is on the inside, and it'll look super cool if it's on the outside.



g++ 6 produces compiler errors.



Tried fbpic, timescale issues.

trying xoopic https://ptsg.egr.msu.edu/#Software as one might expect, this program crashes essentially every 10 seconds.

<hr>

Trying Warp.

MPI_FORTRAN_MOD_DIR must be set



compiled with /configure --with-X11_LIBDIR=/usr/lib/x86_64-linux-gnu/ --with-MPICC=mpicc --with-MPICXX=mpicxx --enable-MPI

<hr>

xoopic compiled with 

./configure --with-X11_LIBDIR=/usr/lib/x86_64-linux-gnu/ --with-MPICC=mpicc --with-MPICXX=mpicxx --enable-MPI --enable-debug



`#GUY DID THE ADDDON`

XOOPIC always segfaults on X exit. Seems to be a libtk bug.

How many bowties and coils can we reasonably fit 

0.004^2 = 0.0000502m^2

0.03^2 / 0.0000502 = 56 

And that's without all the supporting hardware n' stuff

Each gun will do about 0.15 g/hr

http://web.mit.edu/skendig/Public/6.777/project/hopwood.%201999pdf.pdf

https://sci-hub.tw/10.1366/0003702924125276

It looks like the driving frequency isn't of much importance in ICP.





This is a little outlandish, but it might actually be possible to build this whole thing without any feedthroughs - couple power and signals in via transformer coils, put logic in vacuum if possible







<hr>

Looking at ~15 torr per bowtie for 0.1g/h

Oooooooh. I was concerned about bowtie power consumption, right? Having ~100 individual bowties each drawing 200w would be disastrous. I could make some kind of heated graphite plumbing, where one hot element pressurizes a tree...however, the vapor pressure of Al at 1900c is 21.5 torr - we're gaining a huge amount of power there. Recall the $T^4$ term in Boltzmann's eponymous equation. Graphite vapor pressure at the same is 2.47e-6 torr. Cu is similar at 15 torr.

Cool! This means that we can operate the bowties at a much lower temperature - below boiling; and each bowtie will have a lower overall enthalpy load.

Having an array of bowties also reduces the current density requirements on the recombination cathode. It adds *tremendous* complexity to the deflection amplifiers, but that's the easy part. 

Good stuff.



Openfoam nozzle reference added - good stuff again. 



Might be useful for the diff. pump, too.



isogrid: https://femci.gsfc.nasa.gov/isogrid/NASA-CR-124075_Isogrid_Design.pdf

With many deposition chambers, dopants can be added to deposited crystals! Do I smell *semiconductors*?

6502 processor feature size was 8 microns. I think we can hit that target.

<hr>
If we need high deflection voltages, 1000v mosfets are <$1 on digikey. A class A+D PWM+resistor amp should be doable - high-side mosfet drivers at these voltages will be tricky AF.

We now have the ability to use a short "drift tube" section - it may be possible to deflect between two focusing lenses in case we need some extra deflection distance.

The entire 50+ bowtie structure can be built into a spherical cap section to minimize required deflection. Worst case scenario, the bowtie structure can be mounted to a 2-axis voice coil for fine details.



Oh! Better yet, the bowties can be built in hexagonal honeycomb panels of 7 or 19, and then arranged spherically, such that each bowtie is within deflection range of the focal point. This'll avoid each individual section being curved.



### 1549692113 >

Oxides and compounds should totally be possible. The mean free path at 100 torr is ~539 nanometers, so it is guaranteed that a collision will occur for every particle upon focusing.

solder deposition on pads



Hey, hang on a minute. I should be able to make a TIG welder with pretty easily - I just need argon. 

There seems to be some contention over whether "Scratch start/lift-start" TIG can be performed on aluminium. 

I can mount the torch to the CNC! Then I can close the loop with an SSR... eh, couldn't be bothered.

electric hotplate sufficient for preheating braze?

electric arc furnace for alumina ceramic sintering

C-cell graphite electrodes

canadian tire carries alumiweld.

<hr>

Tried Pic-las again, got irritated with XOOPIC's opacity and brittleness.

Sandia's SPARTA is pretty cool. Does recombination, emission, the whole lot. Built very smoothly the first time, but would hang when running on multiple cores with MPI for some reason. Ended up trying a few different versions of openmpi, then resorted to using the internal Debian package, which worked fine - version 2.0.2, for reference. Took the better part of 8 hours. 

```
MPI_INC = -I/usr/lib/x86_64-linux-gnu/openmpi/include/
MPI_PATH = -L/usr/lib/x86_64-linux-gnu/openmpi/lib/
MPI_LIB = -lmpi -lmpi_cxx
```

```For mpi
./configure --prefix=/usr/local/
```



```
mpirun -np 15 --hostfile hostfile spa_mpi_dan -v x 100 -v y 100 -v z 100 -in in.free -l my.log
```



Went to a meeting, got news that SafeSump Inc. isn't getting paid as expected - about $70k in the hole or something 

pretty dissociated right now 

<hr>

I'm trying to confirm that the geometric optics of this device are likely to work. With one deposition chamber, we have a workaround in the focal length/ dispersion distances. With 50, the build platform must be placed relatively far away from the chambers.

The key seems to be that the output from the bowtie nozzle must be perfectly collimated, laminar, and parallel, with *zero* turbulence. OpenFOAM will probably be required to design such a nozzle. If this is possible, the next order of business is to ensure that the ionization process does not disrupt this ideal motion. 



Hang on a minute.



If we can design a nozzle that can perfectly "focus" a gas stream to a micron point, the entire ICP+einzel issue becomes moot, unless extremely fine deflection is required. Is such a nozzle possible?



Let's ignore that for now.

Sn has a molar mass of 118g, so we'll print at almost 5x the speed. Tin is 5x the density, however, so we really aren't gaining that much. 40g/h is still our target.



https://pure.tudelft.nl/portal/files/8786408/2016_SPc_HEATER_CHIP_WITH_DIFFERENT_MICROCHANNELs_GEOMETRIES_FOR_A_LOW_PRESSURE_FREE_MOLECULAR_MICRO_RESISTOJET.pdf

Interesting: 

> Because of the high Knudsen number (or low
> pressure) the Navier-Stokes equation cannot be
> applied. The Direct Simulation Monte
> Carlo (DSMC) method is ideal

That simplifies things a little bit - while I'll still use OpenFOAM for the diffusion pump nozzle, I can keep the bowtie in sparta.



A rough estimate of the print speed of a standard reprap yields a print speed of ~8 grams/hr. 



If the focus pierce electrodes are made out of graphite, the entire ionization chamber can be raised to 2000c to prevent deposition. Might absorb the ICP field.



growing quartz?



https://www.tandfonline.com/doi/pdf/10.1080/02786829408959748



<hr>


![0](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/0.png)



Back in IBSimu after a while - accurately setting space charge density is a bit more difficult in Warp, so I'm doing this for now.



choose random particle in domain and change charge to emulate collision - probably not required, however.

<hr>

Okay, we've got a bit of a problem here. Enabled diagnostics. It looks like the main reason why I'm able to focus the beam in IBSimu is that the particles are being accelerated to high velocities - over 5kev - without any einzel voltage applied. Why's that?

The beam has a certain potential energy due to its space charge, of course - if all fields are removed, the beam explodes, which requires energy. 



Frequency multiplexing? Put a band-pass filter on each plate with a different frequency range and connect them all in parallel? Would need to be rectified...

It'd be great if we could dump the energy from the beam somehow.



This einzel optimization problem is a perfect application for a genetic algorithm. 

st5 antenna evolved: 

https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/20060024675.pdf

https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/20040152147.pdf

https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/20060015686.pdf

https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/20030067398.pdf



If we substitute in electrons, we're fine again:



![ebeam](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/ebeam.png)



![ebeam](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/images/individual/back_to.png)

But a 29 amu particle increases the scharge density commensurately.

Oh, those last runs used 50eV injection energy. Void.



https://www.eng-tips.com/viewthread.cfm?qid=254704

> SREID: In my opinion 1018 and 12L14 give permeability more than 1000 when annealed and slow cooled. Nevertheless, consider that Ni or Cr plating process ( after magnetic annealing)reduces permeability due to Hydrogen adsorbation. Several test of Silicon -Iron alloy confirm this problem. However a simple baking (390-400 F) after plating restores magnetic properties.

Planned on dumping every point to disk and then gnuplotting, but there's just too much data - many gbps.

Planned on scanning over entire problem domain and filtering out good points, but that's inefficient.

How about a GUI? Considered a few GUI options (Qt, GTK+), ended up chosing FLTK - really lightweight and quick to set up.

<http://seriss.com/people/erco/fltk/>



<https://link.springer.com/content/pdf/10.1007/BF01393132.pdf>

> ai, b~, and c~ are
> individual constants, which have to be determined by experiment,
> theory, or reasonable guesswork. 
>
> In order to facilitate the guesswork, I made the following assumptions, which should be accepted as working hypotheses: 

legend.

oh, a more modern version: <https://link.springer.com/content/pdf/10.1007/BF01392963.pdf>

Lotz, Wolfgang. "Electron-impact ionization cross-sections and ionization rate coefficients for atoms and ions from hydrogen to calcium." *Zeitschrift für Physik* 216.3 (1968): 241-247.

also, 

Shevelko, V. P., and H. Tawara. "Semiempirical formulae for multiple ionization of neutral atoms and positive ions by electron impact." *Journal of Physics B: Atomic, Molecular and Optical Physics*28.18 (1995): L589.

has formulae for n>=3.

> Aquadag is used in many types of high-voltage lab apparatus where a conductive coating is needed on an insulating surface. The surfaces of some metals (most notably aluminum) can develop nonconductive oxide layers, which tend to disrupt the electrostatic field produced around the surface of the metal when used as an electrode. Aquadag is not subject to such effects and provides a completely uniform equipotential surface for electrostatics.

Aha! The electrode material is important!

Spent an insane amount of time trying to get a GUI going for IBSimu so that I could play around with solutions live. Used imagemagick to import an image that defines electrodes, then realized that IBSimu already comes with DXF import stock. Dammit :(

Anyhow, it's working. This really doesn't speed things up very much, however. I'm an idiot.



I need an analytic solution.

And I think I've got one. All we're really interested in is that the E-field along each cross-section of the cylindrical beam is equal or greater than the cross section. Maybe I can step along the Y axis...Let's see if I can work something out.

Ah, but this will ignore the all-important transverse acceleration component. What if I approximate that with a cylinder of charge in front and behind the particle.

Here's what's really bothering me, though: I don't have any intuition as to where the energy is *coming* from - a massive, blatant oversight indeed, but *Fundamentals* doesn't seem to discuss it, nor does Feynman. Also concerning is the terrifically high charge assembly power. I'm very skeptical of that value, however, cheifly because I derived the equation, and also because charge assembly isn't the limiting factor in ion drives, which have approximately similar beam currents and total areas.



I should assemble a quick ionization test without regard to focussing: just throw a bunch of electrons at a pair of plates. 

OH! For testing, the faraday cup can be part of the ion optics
$$
$$ U=\left(\frac{I*C}{2}\right) $$
$$




"A measurement of the ionization cross-section
of helium ions by electron impact"

> "...did not gain sufficient momentum to scatter them from the beam"

We may not have to worry about the e-beam's effect!

Quick back-of-the-envelope momentum transfer yielded 100m/s transverse deltaV on the heavy ions with a 100eV e-Beam - totally acceptable, I think. Might increase beam emissivity a bit. We'll have to see.

hoooly F*** perhaps we can filter out charge that way! That angle can deflect particles through an aperture of some sort.

Perhaps not - the beam will spread out massively as soon as the collision occurs. We'll see how it works out.



Microfluidic cast bowtie with shrinky-dink



"Recoil-Ion Momentum Distributions for Single and Double Ionization
of Helium in Strong Laser Fields" - electron collisions with atomic core are possible, and can lead to a loss of momentum transfer

By relativity, it would seem that there should be no energetic difference between a 100eV ion hitting a 1eV recombination electron, or a 100eV electron hitting a 1 eV neutral. The ions will presumably have to be slowed considerably before recombination will occur.



oxygen is diatomic.

Secondary molecular beams, like oxygen, can be simply a nozzle pointed in the right general direction.









<https://en.wikipedia.org/wiki/Bourbaki_dangerous_bend_symbol>



I wonder if we could filter *again* after recombination - that is, deflect the particles that retained a charge towards a dump.

Liquid-ring pump



Ah! What if we try to do scharge compensation with electrons, rather than just pure recombination?

Found a paper that discusses beam neutralization directly. 

Adding a "compensation" NSCCB



Tried myriad warp, IBSimu sims. Been working on my own little sim, but

Oh hey cool! I independently figured out the ring thing!

>  Another technique is the charge ring method [14–17].



@ vallette2010analytical

> When
> the electrons were ejected along the parallel direction
> with respect to the ion beam axis, the increase of the
> ion current was not large; the integrated current at z =
> 730 mm was only 0.14 mA. 
>
> Although the electron
> current was increased, the transported ion current
> was saturated. As the direction of the electron
> ejection declined, the neutralization became more
> effective. At the angle of 600 and 90o the transported ion current became the same value with that obtained
> by the thermionic filaments, as shown in Figure 2.
> The electron currents necessary to neutralize the ion
> beam were 80 μA and 5 μA for the cases of 60o and 90o incidence. 

Fascinating and completely against all my analysis! 



I just can't get it to focus. Somehow the whole thing is invariant to beam current?





I'm not physics-ing correctly. Sure, I've got this laplace sim and everything, but there are things going on here that I don't quite understand - base essentials like where precisely the extraction energy is coming from. Fucking idiot. Anyways, I'm going to build it now.

The best I've gotten so far is this:

 

## Quick notes

printed he-ne laser - very simple

  O. Tange (2011): GNU Parallel - The Command-Line Power Tool,
  ;login: The USENIX Magazine, February 2011:42-47.

*fix elmerfem VTU/VTE before wasting more time!* - darn, I can't remember why I wanted to do this. Ah, right! So that I can run in parallel with domain decomposition!

Brass water jacket on diffusion pump - inside and outside soldered
Better yet, TIG welded output
flapper on diffusion pump for quick cycling without cooldown
diffusion pump cooling loop on oil?



plasma de-ionizes due to *black body radiation*? Wait, what? How does that work? That can't be. Perhaps it cools to below the ionization temperature, but it can't possibly spontaneously form an electron.

Pyrolytic graphite as mu-metal alternative

dumet glass-to-wire seal

Fresnel zone plate

https://www.nasa.gov/sites/default/files/atoms/files/cryogenic_selective_surfaces_final_report_niac_phase_i.pdf

Very cool: cryogenic selective surfaces.

Also cool:

https://www.thevespiary.org/library/Files_Uploaded_by_Users/no1uno/pdf/Instrumentation/NMR/McDowell.Adolphi.Operating.Nanoliter.Scale.NMR.MicroCoils.in.a.1.Tesla.Field.pdf

[^100]: [Internal]() [External]()











https://www.alphalabinc.com/product/asmgm/





I want my company to die in the fires of the free market if it cannot maintain a constant 

Nuance Applied Sciences - already taken.

