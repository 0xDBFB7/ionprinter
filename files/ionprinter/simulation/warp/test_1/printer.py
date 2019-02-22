"""
Printer ICP test
"""
from warp import *

top.pline2   = "Pierce diode with solenoid transport example"
top.pline1   = "Injected beam. Semi-Gaus."
top.runmaker = "DPG"



#########Boundaries#########
setup()

sim_x_dim = 0.004
sim_y_dim = 0.004

x_center_offset = 0.001
y_center_offset = 0.001 #The sparta sim goes from 0 to 2mm.
                        #wanna have the same center so that particles are added in the right place.
#TooHardError - oh, that's whimsical

w3d.bound0 = dirichlet #boundary at z=0
w3d.boundnz = neumann
w3d.boundxy = neumann

top.pbound0 = absorb
top.pboundnz = absorb

w3d.xmmin = sim_x_dim-x_center_offset
w3d.xmmax = sim_x_dim+x_center_offset
w3d.ymmin = sim_x_dim-y_center_offset
w3d.ymmax = sim_x_dim+y_center_offset
w3d.zmmin = 0.0
w3d.zmmax = 0.01

w3d.nx = w3d.ny = 38
w3d.nz = 250


focus_voltage = 10.*kV

##########Input beam#########
input_beam_temperature = 0.361 # in eV
input_beam_radius = 1*mm

beam = Species(type=Aluminium, charge_state=0, name='beam')

beam.a0       = input_beam_radius #initial beam width in x
beam.b0       = input_beam_radius #initial beam width in y
beam.ap0      = .0e0 #initial beam envelope vx/vz? Wat?
beam.bp0      = .0e0
beam.ibeam    = 10
beam.vthz     = sqrt(input_beam_temperature*jperev/beam.mass)
beam.vthperp  = sqrt(input_beam_temperature*jperev/beam.mass)
derivqty()

# def input_neutrals():
#   beam.addparticles(0,0,0,0,0,1000)
#
# installuserinjection(input_neutrals)

def createmyplasma():
  beam.add_uniform_cylinder(10000,0.001,0,0.001)

installparticleloader(createmyplasma)

#https://sites.google.com/a/lbl.gov/warp/home/how-to-s/lattice/time-dependent-lattice-elements

#########Timestep#########
dz = (w3d.zmmax - w3d.zmmin)/w3d.nz
vzfinal = sqrt(2.*focus_voltage*jperev/beam.mass)
top.dt = 0.4*(dz/vzfinal)
##########################

# --- Field grid dimensions - note that nx and ny must be even.
w3d.nx = w3d.ny = 38
w3d.nz = 250

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
# winon(1, suffix='current')
winon()

plot_current_limit = 1

def beamplots():
    window(0)
    fma()
    pfzr(plotsg=0, cond=0, titles=False)
    ppzr(titles=False)
    limits(w3d.zmminglobal, w3d.zmmaxglobal, 0., 10)
    ptitles('Hot plate source into solenoid transport', 'Z (m)', 'R (m)')
    refresh()

    window(1)
    fma()
    pzcurr()
    limits(w3d.zmminglobal, w3d.zmmaxglobal, 0., 10000*1.5)
    refresh()

# --- Call beamplots after every 20 steps
@callfromafterstep
def makebeamplots():
    if top.it%20 == 0:
        beamplots()

step(10000)

# --- Make sure that last plot frames get sent to the cgm file
window(0)
hcp()
# window(1)
# hcp()
