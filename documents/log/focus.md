It may behoove you to read a bit of 

> Humphries, Stanley. Charged particle beams. Courier Corporation, 2013.

for some background.

Einzel lenses

Collimation

The ion printer 

*WARNING*: I am confused by ion optics. If you thought I was incompetent before, check out this fuzzy understanding. Everything I say below may be entirely incorrect and based on a totally flawed intuition. 

As far as I can see, the traditional way of doing this is to use something like an electrostatic Einzel lens or magnetic yoke to focus a thick beam down to a point. Schematics of these methods are readily available online. However, you quickly encounter a problem: beam current density and space charge.


To my understanding, each particle in the beam has a charge. When the beam is thick, this poses little issue; the inter-particle forces are negligible, and the beam propagates approximately parallel. However, the tighter you try to focus the beam, the more each particle tries to repel one another. As you increase the beam current density and the number of particles per unit area increases, the distance between them decreases proportionally, and the divergence force increases.

To counteract this effect, you can increase the transverse particle velocity, squishing the beam tighter together. This means increasing the axial particle velocity, requiring more energy.

Since we're trying to move substantial amounts of material, the beam current (charged particles per second) in this ion printer is extremely high - dozens of orders of magnitude higher than even many particle accelerators. So high, the design crashed IBSimu.

IIRC, to attain a dot size of ~50 micrometers, each particle would have to have something around 1 MeV - easily doable with a mini cyclotron or syncrotron. Minor radiation danger, you know, but nothing too serious.

Except for the multi-gigawatt power consumption, the heat dissipation of which would immediately destroy the build platform, along with the printer, the operator, and the building.

This effect is what Feynman referenced way back in his 1969 lecture, "Plenty of Room at the Bottom".

But we have a trick. A glorious, wonderful, magical trick.

#### The trick.

*There is no reason why the beam has to be charged when it converges.*

In other applications, the beam must be charged - often that's the whole point of the beam in the first place, to deliver targeted charged particles or produce secondaries.

But not here. We only care that the particles are in the right place. 

Once the particles are set on their way, we can remove the charge. 

If we can recombine and neutralize the beam before we get anywhere near that tight bit, we elegantly sidestep all these pesky space charge density issues, and our problem becomes delightfully sedate. Since everything's now uncharged, only esoteric weak polar interatomic or Pauli effects remain,  which are surely negligible at these scales.

This, surprisingly, is easy - how this is done is discussed in Hot Cathode.

Optics are confusing. 

Some stream-of-consciousness:

[1] describes vividly how optical systems can't turn a non-collimated beam into a single point due to the conservation of etendue. We can't possibly make a perfectly collimated beam, however we may try; the bowtie orifice must have a non-zero size, and therefore the minimum beam dot size on the target is limited. A bit of playing around with ray-tracing optics programs also seems to confirm this. So, that's it, right? Case closed; the device is impossible.

However, a few moments consideration can falsify this line of thinking. Electron microscopes use tungsten filaments or lanthanum chunks as their electron sources, and yet can resolve details orders of magnitude finer than the source diameter. An early CRT with a cathode aperture size of 0.8mm could form a dot of 0.4mm diameter.

Back to theory.

[3] describes the theory behind CRT optics: it's extremely simple. The minimum dot size is just the distance from the target to the lens divided by the distance from the source to the lens, multiplied by the bowtie orifice size. If we have the print bed located 10mm away from the magnetic lens, set the distance to the bowtie to be 0.5m, and have a 0.5mm bowtie orifice, our minimum feature size will be a perfectly acceptable 10 microns.

This does mean that we'll have a very short distance in which the recombination must occur. The ion curtain will have to have a thin cross-section.





Update, several months on. These problems are aggressively non-linear, and so the precepts of geometric optics no longer hold.



Beam current: 35.757 A 

[1]: https://what-if.xkcd.com/145/
[2]: US Patent 2,825,837, M.D.Dudley, "Electrostatic focusing system".

[3]: http://celco-nj.com/SID_Paper01.htm