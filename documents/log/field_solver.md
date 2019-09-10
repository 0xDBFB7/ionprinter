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

The whole system must also be usable on common hardware; owning only an Ionolith and a reasonably-powerful gaming computer must be sufficient to simulate the beam, since the end-user may wish to make improvements, use untested materials and alloys, etc.

As we'll see later, this eliminates many options outright. CUDA-only frameworks like AmgX, and PICOnGPU.



There are many ways to simulate a beam of charged particles. In order of computational complexity,

Unfortunately, closed-form equations don't exist for electric fields; you can't just plug numbers into a formula and get a particle trajectory. 

#### Transfer function codes

##### Examples: TRACE-3D, TRANSPORT

Certain simple beam devices (such as Einzel lenses Gillespie, 1997, Rashid, 2011) can be represented in terms of 

These are analogous to geometric optics, where light is assumed to travel in straight lines. Unfortunately, these models fail miserably with low-energy beams, and 

#### Envelope or 'slicing' codes

Assuming a homogeneous, cylindrical beam, 



#### Vlasov codes

##### Examples: IBSimu

These generally converge the entire beam trajectory at once, forming a probabilistic model of the beam. It is difficult to integrate collisions with 



[note: research Vlasov more]

#### Particle-In-Cell codes

This is Poisson's equation.
$$
\grad 
$$


##### Examples: Warp, PIConGPU, QuickPIC, AlaDyn, SMiLEi, Piccante,





##### Examples: Pic-Las, Vorpal



Many codes (such as SIMION) support many of the simulation modes described above.





#### History

The first cathode-ray tubes were in use decades before computing power became sufficient for any of these techniques. These were often designed using electrolytic tank analogs (Staggs, 1965), which mechanically move a probe in a tank of conductive liquid.

In 1977, for instance, [] described using taut wires to measure the field. This is akin to  



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

<https://people.eecs.berkeley.edu/~demmel/cs267/lecture24/lecture24.html>









The author now wishes to bring certain mistakes to attention.

- Irrational attachment to ineffective solutions.

  After the performance of the CPU and cluster was exhausted, alternative computing solutions were sought.

  Initial estimates showed that 

  It is important to be committed to a problem, of course, for the detached researcher's attention may wander, and the solutions they produce may be substandard. However, it is important that any specific solution be abandoned if fundamental flaws are found, and that the chosen course of action be periodically reexamined.

- Misunderstanding of base concepts.

  Residual tweet.

- Mindless 'cycling' through the solution space.

  At a ce

