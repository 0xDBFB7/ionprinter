"""
Printer ICP test
"""
from warp import *

top.pline2   = "Pierce diode with solenoid transport example"
top.pline1   = "Injected beam. Semi-Gaus."
top.runmaker = "DPG"



#########Geometry#########
sim_radius = 0

#########Boundaries#########
setup()

w3d.solvergeom = w3d.RZgeom #using axisymmetric geometry

w3d.bound0 = dirichlet #boundary at z=0
w3d.boundnz = neumann
w3d.boundxy = neumann

top.pbound0 = absorb
top.pboundnz = absorb
top.prwall = sim_radius

w3d.xmmin = -sim_radius
w3d.xmmax = +sim_radius
w3d.ymmin = -sim_radius
w3d.ymmax = +sim_radius
w3d.zmmin = 0.
w3d.zmmax = 1.0

w3d.nx = w3d.ny = 38
w3d.nz = 250

#########Timestep#########
dz = (w3d.zmmax - w3d.zmmin)/w3d.nz
vzfinal = sqrt(2.*diode_voltage*jperev/beam.mass)
top.dt = 0.4*(dz/vzfinal)
##########################

diode_voltage = 10.*kV
##########Input beam#########
input_beam_temperature = 0.361 # in eV
input_beam_radius = 1*mm

beam = Species(type=Aluminium, charge_state=0, name='beam')

beam.a0       = input_beam_radius #initial beam width in x
beam.b0       = input_beam_radius #initial beam width in y
beam.ap0      = .0e0 #initial beam envelope vx/vz? Wat?
beam.bp0      = .0e0
beam.ibeam    = 0.15
beam.vthz     = sqrt(input_beam_temperature*jperev/beam.mass)
beam.vthperp  = sqrt(input_beam_temperature*jperev/beam.mass)
derivqty()


def createmybeam():
  beam.addparticles(...)
installparticleloader(createmybeam)

#https://sites.google.com/a/lbl.gov/warp/home/how-to-s/lattice/time-dependent-lattice-elements



# --- Field grid dimensions - note that nx and ny must be even.
w3d.nx = w3d.ny = 38
w3d.nz = 250

# --- Set the time step size. This needs to be small enough to satisfy the Courant limit.

# --- Specify injection of the particles
top.inject = 1 # 2 means space-charge limited injection
top.rinject = source_curvature_radius # Source radius of curvature
top.npinject = 15 # Approximate number of particles injected each step
top.vinject = diode_voltage
w3d.l_inj_exact = true

# --- If using the RZ geometry, set so injection uses the same geometry
w3d.l_inj_rz = (w3d.solvergeom == w3d.RZgeom)


# --- Set up fieldsolver
f3d.mgtol = 1.e-1 # Multigrid solver convergence tolerance, in volts

solver = MultiGrid2D()
registersolver(solver)

# --- Set pline1 to include appropriate parameters
top.pline1 = ("Injected beam. Semi-Gaus. %dx%d. npinject=%d, dt=%d"%(w3d.nx, w3d.nz, top.npinject, top.dt))

# --- Generate the PIC code (allocate storage, load ptcls, t=0 plots, etc.)
package("w3d")
generate()

# --- Open up plotting windows
winon(1, suffix='current')
winon()

def beamplots():
    window(0)
    fma()
    pfzr(plotsg=0, cond=0, titles=False)
    source.draw(filled=150, fullplane=False)
    plate.draw(filled=100, fullplane=False)
    pipe.draw(filled=100, fullplane=False)
    plotsolenoids()
    ppzr(titles=False)
    limits(w3d.zmminglobal, w3d.zmmaxglobal, 0., channel_radius)
    ptitles('Hot plate source into solenoid transport', 'Z (m)', 'R (m)')
    refresh()

    window(1)
    fma()
    pzcurr()
    limits(w3d.zmminglobal, w3d.zmmaxglobal, 0., diode_current*1.5)
    refresh()

# --- Call beamplots after every 20 steps
@callfromafterstep
def makebeamplots():
    if top.it%20 == 0:
        beamplots()

step(1000)

# --- Make sure that last plot frames get sent to the cgm file
window(0)
hcp()
window(1)
hcp()
