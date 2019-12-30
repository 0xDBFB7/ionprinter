Written January 2019.

At this juncture, it seems useful to write a quick description of what has come so far and discuss some of the more interesting papers a bit more intellibly than the quick notes that have been made thus far. Much of this is written from memory; it is almost certain that things have been left out.


#### The original idea. 



#### The Zinc epoch

Very little that was done at this time is worth mentioning; it was all entirely rubbish science. 



A 


#### Simulation

November 2017.

After hopelessly naive attempts to simulate particles in an n-body mode using the Coulomb equation 

Salom_Meca and Code_Aster. Unfortunatey, their scripting language and documentation was in french.

Elmer is integrated with CAELinux (that is, computer-aided engineering linux) 

The main ionization method at this time looked like ICP. I 

A fellow named Daniel Bondarenko wrote an excellent paper and thesis defense (with co-authors, of course!) on plasma simulation with ElmerFEM:

Engineering design of plasma generation devices using Elmer finite element simulation methods
https://doi.org/10.1016/j.jestch.2016.07.015

He was kind enough to send an extremely nice email, which I'll reproduce here:

```
Hello Daniel,

Sorry for a delayed response. I was conducting the study at a university at the time and the files are most likely still one the hard-drives of the computers there.

I have my own setup now and play with Elmer whenever I have a chance. To be honest, Elmer is strange in terms of simulating plasma, you have to make a lot more assumptions and have a narrower scope in terms of simulating a specific behaviour; what I've done in the university has some limitations and I'm actually more happy that I could conduct an experiment because it has some bearing on reality.

In a nutshell, I don't think it would be a good idea for me to share the files that I have, because I do this stuff for fun and get in some strange rabbit holes with Elmer, which might appear strange to someone just starting out with it. Keep in mind that if you want to simulate a large system with multiple components and have a mid-sized GPU ready to go, then Elmer FEM won't be the best choice, unless you are really well versed both with CUDA and Fortran. Smaller sized systems, both in terms of mesh and the number of boundaries, can be manageable, but then again, keep in mind the very specific assumptions that you are taking with you to the simulation.

On the note of ICP design, I'm guessing that you are probably considering using a microwave RF source so if you are using the friendlier GUI of Elmer consider using the manual and the example for the microwave duct (Tutorial 16: Using VectorHelmholtz module to model wave propagation in bent waveguide). Also, if you specifically need to see the distribution of ionized particles the wiki has an old but good introduction: http://www.elmerfem.org/elmerwiki/index.php/Electrochemistry

I could answer some questions about plasma, if you want, full disclosure though I don't know everything about it. I'm learning something new everyday and at the very least could provide some direction.

Also, consider joining the Elmer FEM community, people there are excellent and smart and respond pretty fast. Try to get familiar with Elmer first though, at least the GUI portion, rookie questions tend to be ignored.

All the best to you in your simulations adventures!

```


Not to foreshadow, but his notes regarding GPU acceleration were particularly salient.


Warp started out as a Fortran code. Documentation was a little sparse; and modules with DSMC. Getting cell potentials was a little trick,

Simulations on the order of 100^3 were carried out.



Due to all sorts of effects (), quantities like total energy, momentum, etc in a PiC simulation can diverge and

Particles within the same cell of a PiC simulation won't feel any force, so 'Coulomb collisions' are usually added in seperately.  


HBM2

Summing up charge on electrode

FBPic

Electron recombination is a relatively slow process. It's what gives rise to the latent glow of fluorescent tubes, the green airglow of the atmosphere, and other pretty glows. A similar process (electron-hole recombination) allows LEDs to produce light.




#### Space charge 


Much of the below is paraphrased from SIMION's explanation

https://simion.com/definition/childs_law.html

SIMION is a general-purpose beam simulator.



$ \nabla \cdot \mathbf {E} ={\frac {\rho }{\varepsilon _{0}}} $

This means 'downwards everywhere' - the field always goes towards zero away from space charges and electrodes. It also means everything should be smooth. 

Poisson's equation:

$ {\displaystyle {\nabla }^{2}\varphi =-{\frac {\rho }{\varepsilon }}.}$


The Child-Langmuir law provides a 1d analytic expression that describes how charged particles can become 'bunched up'. 






