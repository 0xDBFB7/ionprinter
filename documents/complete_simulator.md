It is important to note that the author is not an expert, nor a trained physicist. Much of this document is written from memory, and should certainly not be used as an authoritative source.





The goal

Accurately simulate the electron and heavy ion motion in the beam, to rapidly test designs.

Simulate diffusion pump nozzle.

Optionally, compute stresses, 

The great part is that once this simulation is complete, it can be used to validate the control system, and ensure that prints will complete safely.



To do this, the following physics must be simulated:

- Ionization
- Electric field
- Vaporization (print material, contaminants, graphite from bowtie structure, and diffusion pump oil)
- Joule heating and heat transfer
- Particle deposition, condensation, and crystalline structure formation
- Forces and collisions between ions and ions, ions and electrons, electrons and ions, and the trace background gas in the chamber.
- Optionally, a magnetohydrodynamic system.

The whole system must also be usable on common hardware; in the spirit of open-source, owning only an Ionolith and a reasonably-powerful gaming computer must be sufficient to simulate the beam, since the end-user may wish to make improvements, use untested materials and alloys, etc.

As we'll see later, this eliminates many options outright. CUDA-only frameworks like AmgX, and PICOnGPU.



There are many ways to simulate a beam of charged particles. In order of computational complexity,

Unfortunately, closed-form equations don't exist for electric fields; you can't just plug numbers into a formula and get a particle trajectory. 

The problem is equivalent to the fringe-fields you may have learned about in high-school physics, where the simple capacitor equations break down.

#### Transfer function codes

##### Examples: TRACE-3D, TRANSPORT

Certain simple beam devices (such as Einzel lenses Gillespie, 1997, Rashid, 2011) can be represented by an equally simple analytical function that alters the velocity and position of particles accordingly. 

These are analogous to geometric optics, where light is assumed to travel in straight lines. Unfortunately, these models fail miserably with the complex, low-energy beams used in the ionolith, and also restrict the geometry of electrodes to a limited number of shapes.

#### Envelope or 'slicing' codes

Assuming a homogeneous, cylindrical beam, 



#### Vlasov codes

##### Examples: IBSimu

These generally converge the entire beam trajectory at once, forming a probabilistic model of the beam. It is difficult to integrate collisions with 



[note: research Vlasov more]

#### Particle-In-Cell codes







##### Examples: Warp, WarpX, PIConGPU, QuickPIC, AlaDyn, SMiLEi, Piccante





##### Examples: Pic-Las, V-sim



### Discretization





Many codes (such as SIMION) support many of the simulation modes described above.



Sympletic methods RK4 periodic correction

### Finite-difference-time-domain

##### Examples: gprMax

In conventional 3d electrostatics, we discretize in 3 dimensions, solve, apply changes to electrodes and move particles etc, then solve again, ad infinitum. This works for most time-invariant fields; electrostatic fields, magnetic fields, etc.

Magnetic fields can be found by solving for the current in conductors in each direction (x,y,z), then solving three seperate poisson equations for the same.

It's even possible to simulate time-varying electrostatic systems (like capacitor impedance) in steady-state by supplying assumptions about frequency and amplitude in the boundaries. See [] for more on that.

Likewise, it's possible to simulate the time behavior of heat transfer, 

However, coupled *electromagnetic* fields involve time-varying quantities proper; it's impossible to calculate 'a single step' of the field. For instance, see Maxwell's equations, like
$$
V = -\frac{dB}{dt}
$$
You can't usefully apply this to a mesh without time.

Designing inductively-coupled plasma coils requires the above equation. The time-varying magnetic field induces an axial electric field in the vacuum, which accelerates electrons to bombard neutral gas. This is the main reason why a second electron gun is used for ionization in the first-gen designs; it's ostensibly steady-state. 

This actually constrains our hardware design solution somewhat. If we can't usefully simulate an ICP coil, for instance, we can't deduce the nuances of how the particles will react. They'll twirl and dance in spectacular fashion, a dance for the real world alone.

> Similar compromises can be seen in other fields. For instance, SpaceX uses Methane in their Raptor engines for a number of reasons, including efficiency, etc. However, a significant benefit can be found in combustion simulation. 'cut' fuels like the Merlin's RP-1 fuel undergo thousands of forward and reverse sub-reactions between all the myriad hydrocarbon species that comprise it. With moderate-performance engines like the Merlin, these small-scale effects are apparently less critical. Using a simple fuel like methane allows much simpler, more accurate simulation.



However, the 3D stencils we've used so far can be abstracted into the fourth dimension. We can discretize in 4D, where our mesh contains all time states of all spacial mesh points, and apply the stencils to this much larger set of points.

Unfortunately, these meshes grow as N^4, so FDTD methods are stupendously computationally expensive. Plus, more arithmetic is required at each point.

#### Wavelet methods

One of the more interesting papers is the Wavelet+CG solver.



### Relativistic effects

There are several aspects that simplify this problem. Wakefield solvers such as FBPIC must contend with the relativistic effects that are introduced at high particle speeds; we can neglect these issues entirely.

BigDFT, gprMax, hard to apply boundaries

#### History

The first cathode-ray tubes were in use decades before computing power became sufficient for any of these techniques. These were often designed using electrolytic tank analogs (Staggs, 1965), which mechanically move a probe in a tank of conductive liquid.

In 1977, for instance, [] described using taut wires excited by RF to measure the field. 



Of course, computing all space around the beam seems ridiculous. Many beam solvers restrict their 

SLPIC



There are two ways to implement this: either as a



It is important that the restriction and prolongation operators be mirror images of each other.

For instance, the standard 7-point stencil used in 



<http://viennacl.sourceforge.net/viennacl-benchmarks-amg.html>



5.5 Transfer of boundary residuals









For more, see *An Introduction to
the Conjugate Gradient Method
Without the Agonizing Pain*, Shewchuk et al.



FPGA miners





### Data structures

In general, data structures seem to be about finding a balance between efficiently representing the object and the computational complexity of accessing the data.

As discussed in field solver, 

Fully unstructured meshes sit on one end of the scale. An unstructured mesh can conform itself perfectly to the geometry at hand, requiring few points to build an accurate model.

However, actually operating with this data requires an exhaustive search for neighbors, position lookups, etc. 

In contrast, indexing neighbors in a fully structured mesh requires nothing more than i+1. 

Unstructured meshes (tetrahedral) are usually used for finite-element / finite-volume methods, but can also be used for finite-difference [].



The 'nested' data structures are particularly interesting because multigrid happens naturally.



















The author now wishes to bring certain mistakes to attention.

- Irrational attachment to ineffective solutions.

  After the performance of the CPU and cluster was exhausted, alternative computing solutions were sought.

  Initial estimates showed that FPGAs would be 

  It is important to be committed to a problem, of course, for the detached researcher's attention may wander and the solutions they produce become substandard. However, it is important that any specific solution be abandoned if fundamental flaws are found, and that the chosen course of action be continuously reexamined.

  The fact that the DOD has recently replaced almost all FPGAs wholesale with GPGPUs should have hinted at their effectiveness.

- Misunderstanding of base concepts.

  Residual tweet.

- Mindless 'cycling' through the solution space.

  At a ce

- A single analytic expression can tell you far more about the nature of a problem than countless simulations.

  Test





#### Magnetic fields aren't useful for this problem.



After many trials with IBSimu, I discovered beam rigidity.

> Lesson learned:
>
> A single analytical equation, no matter how oversimplified its description of reality may be, can provide a tremendous amount of understanding about the *character* of a problem.
>
> <https://blogs.sciencemag.org/pipeline/archives/2019/11/13/simulation-and-understanding> Simulation is not understanding.



#### Lessons learned:

- *Briggs' Multigrid* has a wonderful section on troubleshooting codes. 

  -   Visualize the output. With high-dimensional codes, it's easy to miss a corner test case; inspecting the mesh by eye can make issues obvious. 

- Practice the algorithms you're going to implement. Get a feel for how they behave, what sort of convergence and timing you can expect. Jumping to a full-size 16M-DOF problem in 3D in a nonlinear language like OpenCL without first getting an intuition as to how these techniques operate was not a great idea. 

  For one, all the operators are become fiendishly complex as dimensionality increases, making it easier to make silly mistakes. The test cycle time of larger problems becomes huge, and waiting for GPU setup fills, kernel compilation times, etc.

  An unfamiliar algorithm should be gradually prototyped, first in 1D in as high-level a language as possible, then in 2D with some added complexity, then in 3D. At this point, you should have a slow but complete reference implementation from which you can develop a fast one.

  As horrifying as it may sound, TDD seemed to be beyond useless during this project. The complexity of the functions were not sufficient to demand it, and requirements changed too quickly. The whole API would be re-written hourly, data structures would change as performance issues became apparent, etc. It may be helpful to pause test coverage while prototyping.

  Note also that the character of many algorithms can change drastically as the problem size increases. 

  Premature optimization isn't necessarily the root of all evil in scientific computation. 

  Visualize 
