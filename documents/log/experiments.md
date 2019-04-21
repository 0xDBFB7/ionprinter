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

Determining the electric field 

