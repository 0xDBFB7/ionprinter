"""
Example Pierce diode with subsequest solenoid transport.
Hot plate source emitting singly ionized potassium.
"""
from warp import *
#x y z vx vy vz
sparta_particles = [x.replace('\n','').split(' ') for x in open("../../sparta/bowtie_nozzle/warp.dump").read().splitlines()]
#filter out some particles
print(len(sparta_particles))

# sparta_particles = [x for x in sparta_particles if len(x) == 6]
new_particles = []
for i in range(0,len(sparta_particles)):
    try:
        new_particles.append([float(x) for x in sparta_particles[i] if x])
    except:
        pass
sparta_particles = new_particles
sparta_particles = [x for x in sparta_particles if len(x) == 6]

sparta_particles = [x for x in sparta_particles if x[2] > 0.0015]

print(len(new_particles))
# --- Set four-character run id, comment lines, user's name.
top.pline2   = "Pierce diode with solenoid transport example"
top.pline1   = "Injected beam. Semi-Gaus."
top.runmaker = "DPG"

# --- Invoke setup routine for the plotting
setup()

# --- Set the dimensionality
w3d.solvergeom = w3d.XYZgeom

# # --- Basic parameters
# channel_radius = 1.*mm
#
diode_voltage = 0.05*kV
#
# # --- Setup source plate
source_radius = 0.5*mm
source_temperature = 0.1 # in eV
# source_curvature_radius = 30.*cm # --- radius of curvature of emitting surface
# pierce_angle = 67.
#
# # --- Setup diode aperture plate
# zplate = 1.*mm # --- plate location
# rplate = 0*mm # --- aperture radius
# plate_width = 1*mm # --- thickness of aperture plate

# --- Setup simulation species
beam = Species(type=Potassium, charge_state=1, name='beam')
elec = Species(type=Electron, name='electrons')
# --- Child-Langmuir current between parallel plates
# j = 4./9.*eps0*sqrt(2.*echarge*beam.charge_state/beam.mass)*diode_voltage**1.5/zplate**2
# diode_current = pi*source_radius**2*j
#
# print("Child-Langmuir current density = ", j)
# print("Child-Langmuir current = ", diode_current)

# --- Set basic beam parameters
beam.a0       = source_radius
beam.b0       = source_radius
beam.ap0      = .0e0
beam.bp0      = .0e0
beam.ibeam    = 1
beam.vthz     = sqrt(source_temperature*jperev/beam.mass)
beam.vthperp  = sqrt(source_temperature*jperev/beam.mass)
derivqty()

# --- Length of simulation box
runlen = 0.2

# --- Variables to set symmetry, when using 3D
#w3d.l4symtry = true
#w3d.l2symtry = false


sim_x_dim = 0.001
sim_y_dim = 0.001

x_center_offset = 0.001
y_center_offset = 0.001 #The sparta sim goes from 0 to 2mm.
                        #wanna have the same center so that particles are added in the right place.
#TooHardError - oh, that's whimsical

w3d.bound0 = dirichlet #boundary at z=0
w3d.boundnz = neumann
w3d.boundxy = neumann

top.pbound0 = absorb
top.pboundnz = absorb

w3d.xmmin = 0
w3d.xmmax = 0.002
w3d.ymmin = 0
w3d.ymmax = 0.002
w3d.zmmin = 0.0
w3d.zmmax = 0.01

w3d.nx = w3d.ny = 38
w3d.nz = 250
# --- Set the time step size. This needs to be small enough to satisfy the Courant limit.
dz = (w3d.zmmax - w3d.zmmin)/w3d.nz
vzfinal = sqrt(2.*diode_voltage*jperev/beam.mass)
top.dt = 0.4*(dz/vzfinal)

# --- Specify injection of the particles
# top.inject = 1 # 2 means space-charge limited injection
# top.rinject = source_curvature_radius # Source radius of curvature
# top.npinject = 15 # Approximate number of particles injected each step
# top.vinject = diode_voltage
# w3d.l_inj_exact = true

def createmyplasma():
    beam.addparticles([x[0] for x in sparta_particles],
                        [x[1] for x in sparta_particles],
                        [x[2] for x in sparta_particles],
                        [x[3] for x in sparta_particles],
                        [x[4] for x in sparta_particles],
                        [x[5] for x in sparta_particles])
    #beam.add_uniform_cylinder(10000,0.001,0,0.001)
  # for i in range(1000):
  #     beam.addparticles(0,0,0,0,0,1000000)

installparticleloader(createmyplasma)

# def createmybeam():
#     for i in range(1000):
#         beam.addparticles(0,0.1,0,0,0,1000)
#
# installparticleloader(createmybeam)

# def input_neutrals():
#     for i in range(0,100):
#         beam.addparticles(0,0,0,1000,1000,1000)
#
#installuserinjection(createmyplasma)

def getfielddata(time):
    return fieldamplitude*sin(time/fieldperiod*2*pi)

# addnewsolenoid(zi=0,
#                zf=1,
#                ri=0,
#                maxbz=0.05)

# def plotsolenoids():
#     cc = array([10],dtype=ubyte)
#     rr = [solenoid_radius, solenoid_radius, solenoid_radius+1.*cm, solenoid_radius+1.*cm]
#     zz = zplate+solendoid_halfgap + array([0., match_solenoid_length, match_solenoid_length, 0.])
#     plfp(cc, rr, zz, [4])
#
#     z0 = zplate + solendoid_halfgap + match_solenoid_length + solendoid_halfgap
#     zz = array([0., solenoid_length, solenoid_length, 0.])
#     for i in range(nsolenoids):
#         plfp(cc, rr, z0 + solendoid_halfgap+i*solenoid_period + zz, [4])

# --- Set up fieldsolver
f3d.mgtol = 1.e-1 # Multigrid solver convergence tolerance, in volts

solver = EM3D()
registersolver(solver)

# child1 = solver.addchild(mins=[xmin1,ymin1,zmin1],
                         # maxs=[xmax1,ymax1,zmax1],refinement=[2,2,2])
# piercezlen = (channel_radius - source_radius)*tan((90.-pierce_angle)*pi/180.)
# piercezlen = 0.04
# rround = plate_width/2.
#
# # --- Create source conductors
#
# # --- Outer radius of Pierce cone
# rpierce = source_radius + piercezlen*tan(pierce_angle*pi/180.)
#
# # --- Depth of curved emitting surface
# sourcezlen = (source_radius**2/(source_curvature_radius + sqrt(source_curvature_radius**2 - source_radius**2)))
#
# # --- the rsrf and zsrf specify the line in RZ describing the shape of the source and Pierce cone.
# # --- The first segment is an arc, the curved emitting surface.
# source = ZSrfrv(rsrf=[0., source_radius, rpierce, channel_radius, channel_radius],
#                 zsrf=[0., sourcezlen, sourcezlen + piercezlen, sourcezlen + piercezlen, 0.],
#                 zc=[source_curvature_radius, None, None, None, None],
#                 rc=[0., None, None, None, None],
#                 voltage=diode_voltage)
#
# # installconductor(source, dfill=largepos)
#
# # --- Create aperture plate
# plate = ZRoundedCylinderOut(radius=rplate, length=plate_width, radius2=rround, voltage=0., zcent=zplate)

# installconductor(plate,dfill=largepos)
#
# --- Pipe in the solenoid transport
# pipe = ZCylinderOut(radius=solenoid_radius, zlower=zplate+plate_width/2., zupper=zplate+nsolenoids*solenoid_period)

# installconductor(pipe)

# --- Setup the particle scraper
# scraper = ParticleScraper([source, plate, pipe])

# --- Set pline1 to include appropriate parameters
top.pline1 = ("Injected beam. Semi-Gaus. %dx%d. npinject=%d, dt=%d"%(w3d.nx, w3d.nz, top.npinject, top.dt))

# --- Generate the PIC code (allocate storage, load ptcls, t=0 plots, etc.)
package("w3d")
generate()

# --- Open up plotting windows
# winon(1, suffix='current')
winon()

def beamplots():
    window(0)
    fma()
    pfzx(plotsg=1, cond=0, titles=False)
    # source.draw(filled=150, fullplane=False)
    # plate.draw(filled=100, fullplane=False)
    # pipe.draw(filled=100, fullplane=False)
    # plotsolenoids()
    ppzx(titles=False)
    limits(w3d.zmminglobal, w3d.zmmaxglobal, 0., 0.002)
    ptitles('Hot plate source into solenoid transport', 'Z (m)', 'R (m)')
    refresh()

    # window(1)
    # fma()
    # pzcurr()
    # limits(w3d.zmminglobal, w3d.zmmaxglobal, 0., diode_current*1.5)
    # refresh()

# --- Call beamplots after every 20 steps
@callfromafterstep
def makebeamplots():
    # if top.it%20 == 0:
    beamplots()

step(10000)

# --- Make sure that last plot frames get sent to the cgm file
window(0)
hcp()
window(1)
hcp()
