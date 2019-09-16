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
- Particle deposition and condensation
- Forces and collisions between ions and ions, ions and electrons, and the trace background gas in the chamber.

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

Say we've got some arbitrary arrangement of electrodes with specific voltages, and now we'd like to know what the force would be on some particles nearby. Well, the force on a particle is proportional to $\frac{voltage}{distance}$, so we need the voltage potential field.

This is Poisson's equation. 

$$
\nabla \cdotp \nabla\phi = \frac{q}{\epsilon_0}
$$


This may look quite daunting to the uninitiated, but rest assured that after we do away with notation, the concepts involved are very simple.

$\phi$ (phi) refers to the potential field. Taking $\nabla \phi$ (the gradient of the potential field) means asking "which way is uphill, and how steep is it?". If a ball placed at a point on $\phi$  wouldn't roll, $|\nabla \phi| = 0$. [^1]

$\nabla \cdotp$ (the divergence of the gradient) means, in essence, that 

- the potential is always smooth, except when forced otherwise by electrodes, and
- always decreasing towards zero, except when forced otherwise by electrodes or space charge.

You may find that this makes intuitive sense - it'd be pretty weird if the field jumped around in the middle of nowhere.



The right-hand-side $\frac{q}{\epsilon_0}$ refers to any charges hanging in the air. This is quite convenient, since we can therefore use this single equation to solve not only the force from the electrode field, but also between free-floating charged particles (the space charge).

It also allows us to find the effects of the particles on the electrodes.

[^2]: You may also hear about the Laplace equation: this is merely Poisson without the right-hand-side, where space charge is assumed to be zero.



Unfortunately, this equation can't simply be worked in the manner you may be used to. So-called 'direct' methods exist, but they become computationally infeasible for more than a few dozen points. 

CG 'updates all values in the direction of a solution'.

Recall that the potential field must be smooth. What if we took the neighboring points, averaged them, then updated the middle point? This is an iterative solver known as Jacobi. If the new values are updated immediately (such that the adjacent stencils will use the updated value), this is known as a Gauss-Seidel smoother; if the new values are put into a second array, this is a Jacobi smoother.

Many different stencils can be used, from simple 7-point 19-point high-order stencils.

However, there's a problem. Because each point's value depends on each other point, distant corrections will take many thousands of iterations to propagate, causing the convergence to stall. G-S and Jacobi work marvelously for small meshes, but become asymptotically ineffective as convergence progresses. 

--

This is known as Geometric Multigrid. If we do away with the 'stencil' concepts, we can define a more abstract form of the same operations called Algebraic Multigrid.


$$
\text{T}\phi = \text{F}
$$


In 1-D, that looks like this:

$$
\begin{bmatrix}
       \frac{5}{6} & \frac{1}{6} & 0           \\[0.3em]
       \frac{5}{6} & 0           & \frac{1}{6} \\[0.3em]
       0           & \frac{5}{6} & \frac{1}{6}
       
     \end{bmatrix}
\begin{bmatrix}
        0           \\[0.3em]
        \frac{1}{6} \\[0.3em]
        \frac{1}{6}
     \end{bmatrix}
= \begin{bmatrix}
       \frac{5}{6} & \frac{1}{6} & 0           \\[0.3em]
       \frac{5}{6} & 0           & \frac{1}{6} \\[0.3em]
       0           & \frac{5}{6} & \frac{1}{6}
     \end{bmatrix}
$$

All the complexity 

Now, if we wanted to solve for the field at every possible position in some region, we would need to solve an infinite number of these equations, requiring infinite computing power. Moore's law notwithstanding. Fortunately, we can instead solve a reasonable, representative number of points by discretization. A common rule of thumb is 10 cells across the diameter of a beam.



It is also helpful to define the 'residual'.
$$

$$
(confusingly, some appear to define the residual as F-Ab.)

Now, how are electrodes applied to this grid? Simple: the points within the electrode are not updated after a smoothing sweep. Adjacent points still use these boundary values in the arms of their stencils. The residuals must also be zeroed in the boundaries.



<https://people.eecs.berkeley.edu/~demmel/cs267/lecture24/lecture24.html>



How do we tell when we're done? Well, we don't know what the true solution is, so there isn't a way to check if our solution is correct. However, we can measure the change in the field after each solver iteration, or take the magnitude of the residual, or the maximum value.



Multigrid island boundaries



[^1]: || in this case refers to the magnitude or 2-norm.






##### Examples: Warp, WarpX, PIConGPU, QuickPIC, AlaDyn, SMiLEi, Piccante





##### Examples: Pic-Las, V-sim



Many codes (such as SIMION) support many of the simulation modes described above.





#### History

The first cathode-ray tubes were in use decades before computing power became sufficient for any of these techniques. These were often designed using electrolytic tank analogs (Staggs, 1965), which mechanically move a probe in a tank of conductive liquid.

In 1977, for instance, [] described using taut wires excited by RF to measure the field. 



Of course, computing all space around the beam seems ridiculous. Many beam 

SLPIC



These operators can be thought of either in terms of local operations (known as stencils), or in terms of the core linear algebra. 

For instance, the standard 7-point stencil used in 



<http://viennacl.sourceforge.net/viennacl-benchmarks-amg.html>



5.5 Transfer of boundary residuals





```
def conjugate_grad(A, b, x=None):
    """
    Description
    -----------
    Solve a linear equation Ax = b with conjugate gradient method.
    Parameters
    ----------
    A: 2d numpy.array of positive semi-definite (symmetric) matrix
    b: 1d numpy.array
    x: 1d numpy.array of initial point
    Returns
    -------
    1d numpy.array x such that Ax = b
    """
    n = len(b)
    if not x:
        x = np.ones(n)
    r = np.dot(A, x) - b
    p = - r
    r_k_norm = np.dot(r, r)
    for i in xrange(2*n):
        Ap = np.dot(A, p)
        alpha = r_k_norm / np.dot(p, Ap)
        x += alpha * p
        r += alpha * Ap
        r_kplus1_norm = np.dot(r, r)
        beta = r_kplus1_norm / r_k_norm
        r_k_norm = r_kplus1_norm
        if r_kplus1_norm < 1e-5:
            print 'Itr:', i
            break
        p = beta * p - r
    return x
```

<https://gist.github.com/sfujiwara/b135e0981d703986b6c2#file-cg-py-L51>



For more, see *An Introduction to
the Conjugate Gradient Method
Without the Agonizing Pain*, Shewchuk et al.



FPGA miners









The author now wishes to bring certain mistakes to attention.

- Irrational attachment to ineffective solutions.

  After the performance of the CPU and cluster was exhausted, alternative computing solutions were sought.

  Initial estimates showed that 

  It is important to be committed to a problem, of course, for the detached researcher's attention may wander, and the solutions they produce may be substandard. However, it is important that any specific solution be abandoned if fundamental flaws are found, and that the chosen course of action be periodically reexamined.

- Misunderstanding of base concepts.

  Residual tweet.

- Mindless 'cycling' through the solution space.

  At a ce







#### Magnetic fields aren't useful for this problem.

After many trials with IBSimu, I discovered beam rigidity.

