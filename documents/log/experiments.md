### Dan's Fast Thin Beam Approximation

#### Background: 

Designing the focus and recombination lens poses no small challenge. High beam rigidity and charge density make conventional extractor systems difficult to implement. The radial velocity kick requirements 

Manual extractor design proved to be too slow, even with highly optimized GUI workflows and automated diagnostics.

Closed-form solutions do not exist for many possible extractor systems.

This problem is an ideal application for evolutionary design via genetic algorithms. Unfortunately, the computational intensity of standard PiC beam simulators like Warp and IBSimu make these techniques ineffective - a minimal mesh required over 3 seconds to converge on 16 cores.

After nearly a year was spent attempting to design th



Aim:

<hr>

I attempted to use a ring approximation method to simulate beam space charge. However, the program complexity quickly grew out of hand, and this concept proved unworkable. 

On a whim, I tried using a simple analytic space charge function for the beam profile. This worked very well, providing a good fit to IBSimu solutions in a fraction of the time. 

Warp was used to determining the electrode field; however, this was slow and 	 



#### Rationale

I'm mad at open-source Computer Aided Engineering.

(note: I'm not actually mad at open-source computer aided engineering. Everyone in this field is doing amazing things that I couldn't replicate in several lifetimes. This is actually just a projection of my own abject incompetence.)

Over the last 6 or so months, I've had cause to try *every single* open-source PIC simulation program, along with a few dozen computer-aided engineering and finite-element programs. I have 19.8 GB of simulation software on my disk, plus another 80 GB in CAELinux installations. 

Each PIC program generally took the better part of a day to install - usually due OpenMPI version conflicts, dependency rabbit-holes, and myriad other yak shaving.

This is a prime use for docker, you say! 

Great! Now I have to figure out how to make a virtual volume where the program wants it, enable parallelization, wait interminable times while the container is built - oh, docker just filled my drive with old cached builds.

No.

Let's examine a simple, straightforward joule heating / radiation simulation in Elmer (a great program).

1. Export .step from FreeCAD (simple, already in my workflow.)
2. Spin up VirtualBox VM with CAELinux.
3. Import .step into gmsh. (works for some parts, crashes immediately for some reason?)
4. Figure out why gmsh is crashing.
5. Run a custom bash script that creates all the 
6. Install a new version 
7. Why isn't the sim converging?
8. Refine mesh, repeat steps 3 onwards
9. Export - oh, no, exporting isn't working
10. Export to paraview.

Nevermind trying to add some esoteric feature.

In software development, a 10-second compile time (or, in TTD, test runner length) is generally regarded as unacceptable. Most toolchains are installed in a few minutes at most.

In hardware, rapid prototyping has become mainstream; the ability to conduct extremely rapid iteration is highly sought after. Most small parts can be made in under a few hours.

In firmware, I can compile, upload a binary to a microcontroller, and begin JTAG debugging the output in ~3 seconds.

The situation is such that building a physical prototype and testing it in a vacuum chamber is far, far faster than a single iteration of a simulation. 

This is ridiculous!

Moreover, this isn't a computing limitation - most simulations only required 

What if I want a diagnostic that doesn't already exist? 