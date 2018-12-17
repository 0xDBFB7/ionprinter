# Log introduction



#### 1545020962 >

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

![gradient_extreme](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/gradient_extreme.png)

Man, that is killer. A ~3t field for 0.2+ m. That's going to be tricky to pull off without superconductors. How's the focusing after that?

![partial_gradient](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/partial_gradient.png)

![full_gradient](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/full_gradient.png)

Yup, that all makes sense, looks good. Pretty stable at 35.75a of beam current and 15eV. Good stuff.

![gradient_30eV](../../files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1/gradient_30eV.png)

The exact same test at 30eV accel. produces an identical plot.













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



