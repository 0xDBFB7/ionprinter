#### Hubris note

T



Particle-in-cell codes all have one thing in common: they require the electric field to be known in some volume of space. 

There are a few ways that one could do this. 

#### Meshless methods

Methods exist to solve Green's function. 

These aren't 



The really fantastic thing is that both heat transfer, electrostatics in dielectrics, and magnetostatics  share the same mathematical relationships; so once we find a solution for one, 

The definition of the derivative





Say we've got some arbitrary arrangement of electrodes with specific voltages, and now we'd like to know what the force would be on some particles nearby. Well, the force on a particle is proportional to $\frac{voltage}{distance}$, so we need the voltage potential field.

This is Poisson's equation. 

$$
\nabla \cdotp \nabla\phi = \frac{q}{\epsilon_0}
$$


This may look quite daunting to the uninitiated, but rest assured that after we do away with notation, the concepts involved are very simple.

$\phi$ (phi) refers to the potential field. Taking $\nabla \phi$ (the gradient of the potential field) means asking "which way is uphill, and how steep is it?". If a ball placed at a point on $\phi$  wouldn't roll, $|\nabla \phi| = 0$. [^1]

$\nabla \cdotp$ (the divergence of the gradient) 

Div 

The electrostatic field has the following intuitive properties: 

- the potential is always smooth, except when forced otherwise by electrodes, and
- always decreasing towards zero, except when forced otherwise by electrodes or space charge.

You may find that this makes intuitive sense - it'd be pretty weird if the field jumped around in the middle of nowhere.



The right-hand-side $\frac{q}{\epsilon_0}$ refers to any charges hanging in the vacuum. This is quite convenient, since we can therefore use this single equation to solve not only the force from the electrode field, but also between free-floating charged particles (the all-important space charge). 

Collisions within a cell (coulomb collisions) have to be handled

It also allows us to find the effects of the particles on the electrodes.

[^2]: You may also hear about the Laplace equation: this is merely Poisson without the right-hand-side, where space charge is assumed to be zero.


Unfortunately, this equation can't simply be worked in the manner you may be used to. So-called 'direct' methods exist, but they become computationally infeasible for more than a few dozen points. 

CG 'updates all values in the direction of a solution'. It requires the matrix to be 'positive definite' - I don't understand what this means, but it distinctly does *not* mean BiCG includes a second subspace for negative definite.

Recall that the potential field must be smooth. What if we took the neighboring points, averaged them, then updated the middle point? This is an iterative solver known as Jacobi. If the new values are updated immediately (such that the adjacent stencils will use the updated value), this is known as a Gauss-Seidel smoother; if the new values are put into a second array, this is a Jacobi smoother.

Many different stencils can be used, from simple 7-point 19-point high-order stencils.

However, there's a problem. Because each point's value depends on each other point, distant corrections will take many thousands of iterations to propagate, causing the convergence to stall. G-S and Jacobi work marvelously for small meshes, but become asymptotically ineffective as convergence progresses. 

--

This is known as Geometric Multigrid. If we do away with the 'stencil' concepts, we can define a more abstract form of the same operations called Algebraic Multigrid. Algebraic multigrid doesn't care about the structure of the data; unfortunately, this means it is typically slower.



$$
\text{T}\phi = \text{F}
$$


In 1-D, that looks like this:

$$
\begin{bmatrix}
       4 & 1 & 0           \\[0.3em]
       1 & 4           & 1 \\[0.3em]
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



Or, in a more manageable stencil representation,
$$
\phi[x,y] - \phi[x+1,y] - \phi[x-1,y] - \phi[x,y+1] - \phi[x,y-1] = q/e0
$$

All the complexity 

Now, if we wanted to solve for the field at every possible position in some region, we would need to solve an infinite number of these equations, requiring infinite computing power. Moore's law notwithstanding. Fortunately, we can instead solve a reasonable, representative number of points by discretization. A common rule of thumb is 10 cells across the diameter of a beam.



It is also helpful to define the 'residual'.

$$

$$

This is somewhat like the R^2 value in statistics: it describes how well a certain mesh is described by the equation.

If you just want a fast stopping criteria, you can cheat by subtracting each iteration of the solver from the previous one; the largest difference is a fast stopping criteria on vector processors.

(confusingly, some appear to define the residual backwards, as F-Ab. This is equivalent, it just means the corrections have to be applied with -).

Now, how are electrodes applied to this grid? Simple: the points within the electrode are not updated after a smoothing sweep. Adjacent points still use these boundary values in the arms of their stencils. The residuals must also be zeroed in the boundaries.

Electrode shapes are known as island boundaries. Applying multigrid to island boundaries can become tricky, especially in corners:

You can see that the corner has been lost when restricting to a coarser grid, preventing rapid convergence. There are two ways to fix this:

- A modified boundary restriction (by none other than walt disney studios!) 

- Running a large number of iterations of a smoother on just the outside of the boundaries.



  <https://people.eecs.berkeley.edu/~demmel/cs267/lecture24/lecture24.html>

Gauss-Seidel's convergence rate is far superior to Jacobi for these problems. Unfortunately, 

Jacobi can be accelerated by turning it into Successive Over Relaxation - adding a term between 1 and 2. The ideal term can be exactly calculated, but generally it's just tuned. This can make the solution much less stable.



How do we tell when we're done? Well, we don't know what the true solution is, so there isn't a way to check if our solution is correct. However, we can measure the change in the field after each solver iteration, or take the magnitude of the residual, or the maximum value. 

This isn't foolproof, and there's some contention regarding stopping criteria for numerical methods.



#### Preconditioning





#### Conjugate gradient

The way CG breaks is quite amusing. One step involves dividing by a matrix dotted with its transpose; if the matrix is not diagonally dominant, you divide by zero. BiCGSTAB splits the problem into two triangles, avoiding this issue. It's usually the solver of choice for these sorts of systems.

One can also use "preconditioners" to make the matrix into a form that another solver can handle. This doesn't involve 

This can be 



> The smoother used in the upstroke of the V-Cycle (Algorithm 1, line 11) performs the operations of the smoother
> used for the downstroke (line 4) in reverse order. E.g.,
> if Gauss-Seidel iteration is used, opposite traversal orders
> should be used when descending or ascending the V-Cycle,
> respectively. For Jacobi smoothers, no reversal is necessary
> as the result is independent of the traversal order



#### Multigrid island boundaries

Dirichlet BCs set the value; Neumann BCs set the derivative of the value. (Robin BCs set both the value and its derivative). A Neumann BC might be used in thermal simulations - to force a steady heat flux, for instance. We won't make much use of it in electrostatics, however.

Surrounding the simulation mesh are the wall boundary conditions. For our purposes, these walls are used to simulate the infinite vacuum that envelopes the domain over which we reign; a layer of Dirichlet BC ghost points are added at 0v. It is important that the walls of the simulation not be too close, or else the field will be distorted. To correctly represent 

In other simulation programs, periodic walls are introduced, perhaps to simulate a long self-similar beam-pipe 

Electrodes are variously called "immersed, embedded, or island" boundaries. They interfere with the coarsening 

<https://amrex-codes.github.io/amrex/docs_html/EB_Chapter.html>



[McAdams 2010]: 	"A parallel multigrid Poisson solver for fluids simulation on large grids, McAdams et al. (2010)"



[^1]: || in this case refers to the magnitude or 2-norm - 





#### Determining electrode properties

It's quite useful to know the capacitance and charge on an electrode; not only is this necessary to predict the slew performance of the amplifiers, but the beam modulation detectors function by 

Fortunately, once the electrostatic field is known, determining the charge on an arbitrary shape is a simple matter of applying gauss's law.



This is the integral form of Gauss' law.

surf int  E dot dA = q/e_0

The sum of the electric field / area normal to any closed shape is equal to the enclosed charge.

From this we can determine the capacitance and voltage of the electrodes.



The surface over which all the charge is summed need not be the electrode itself; it can be a box around the electrode. 