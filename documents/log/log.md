



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
>  ffmpeg -framerate 3 -pattern_type glob -i '*.png' xposscan.mp4

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

### Quick notes

Brass water jacket on diffusion pump - inside and outside soldered
Better yet, TIG welded output
flapper on diffusion pump for quick cycling without cooldown
diffusion pump cooling loop on oil?

Nail polish barium carbonate coating

plasma de-ionizes due to *black body radiation*? Wait, what? How does that work? That can't be. Perhaps it cools to below the ionization temperature, but it can't possibly spontaneously form an electron.

Pyrolytic graphite as mu-metal alternative

dumet glass-to-wire seal

Fresnel zone plate

ion curtain can use a grid

https://www.nasa.gov/sites/default/files/atoms/files/cryogenic_selective_surfaces_final_report_niac_phase_i.pdf

Very cool: cryogenic selective surfaces.

Also cool:

https://www.thevespiary.org/library/Files_Uploaded_by_Users/no1uno/pdf/Instrumentation/NMR/McDowell.Adolphi.Operating.Nanoliter.Scale.NMR.MicroCoils.in.a.1.Tesla.Field.pdf

[^100]: [Internal]() [External]()

The cost of production should be lower than the cost of customer support would be - if any problem arises, we automatically ship out a replacement, no human involved. I'd rather spend money on R&D and in-field testing than a call center.

Brazing to ceramic is totally possible:

> Where ceramics were to be brazed
> to metal, monel or even copper would be considered as a buffer material to permit
> the high temperature brazes without cracking the ceramics. 

`depositron` already taken

`ionitron`also

`beamotron` and variations taken

`nrdditron`? A bit clunky, but not yet taken...

`ionolith`

`nrddilith`

oh ha I'm going to be smiling in the video so one could say I'm "beaming" haha

