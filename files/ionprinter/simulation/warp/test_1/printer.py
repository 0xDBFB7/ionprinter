"""
Example Pierce diode with subsequest solenoid transport.
Hot plate source emitting singly ionized potassium.
"""
from warp import *
#x y z vx vy vz
sparta_particles = [x.replace('\n','').split(' ') for x in open("../../sparta/bowtie_nozzle/warp.dump").read().splitlines()]
print(len(sparta_particles))

new_particles = []
for i in range(0,len(sparta_particles)):
    try:
        new_particles.append([float(x) for x in sparta_particles[i] if x])
    except:
        pass
sparta_particles = new_particles

#filter out some particles
sparta_particles = [x for x in sparta_particles if len(x) == 6]

sparta_particles = [x for x in sparta_particles if x[2] > 0.0015]

print(len(sparta_particles))

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
diode_voltage = 0.01*kV
#
# # --- Setup source plate
# source_radius = 1*mm
# source_temperature = 0.1 # in eV
# source_curvature_radius = 30.*cm # --- radius of curvature of emitting surface
# pierce_angle = 67.
#
# # --- Setup diode aperture plate
# zplate = 1.*mm # --- plate location
# rplate = 0*mm # --- aperture radius
# plate_width = 1*mm # --- thickness of aperture plate

# --- Setup simulation species
#ah, that's the key! how many particles would be in the thin slice of time that we've just added?

beam = Species(type=Potassium, charge_state=1, weight=1, name='beam')
elec = Species(type=Electron, name='electrons')

physical_beam_current = 0.0005

# --- Set basic beam parameters
beam.a0       = 0.01 #not actually used - the beam profile is set by sparta.
beam.b0       = 0.01 #Defines where the 1 npinject particle goes.
beam.ap0      = .0e0
beam.bp0      = .0e0
beam.ibeam    = physical_beam_current #total_beam_current* npinject/ actual particles? something like that?
# beam.vthz     = sqrt(source_temperature*jperev/beam.mass)
# beam.vthperp  = sqrt(source_temperature*jperev/beam.mass)
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

w3d.bound0 = neumann #boundary at z=0
w3d.boundnz = neumann #might need fixing!
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
# top.inject = 0 # 2 means space-charge limited injection
# top.rinject = 1*mm # Source radius of curvature

#It appears that we have to trick warp into calculating the particle Scharge field.
#Without npinject, the particles added via neutral_inject are not affected by the field

# top.npinject = npinject_count #number of macroparticles per "real" particle?
# top.vinject = diode_voltage
# w3d.l_inj_exact = true

def neutral_inject():
    beam.addparticles([x[0] for x in sparta_particles],
                        [x[1] for x in sparta_particles],
                        [x[2] for x in sparta_particles],
                        [x[3] for x in sparta_particles],
                        [x[4] for x in sparta_particles],
                        [x[5] for x in sparta_particles])

installparticleloader(neutral_inject)


ICP_field_period = 1e-6 #seconds
ICP_max_field = 1.0

def getfielddata(time):
    return ICP_max_field*sin(time/ICP_field_period*2*pi)

addnewsolenoid(zi=0.00,
               zf=0.01,
               ri=0.003,
               maxbz=0.01,func=getfielddata)

# beam_collision = LangevinCollisions(beam)

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

solver = MultiGrid3D()
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
#2

# pierce_1 = ZAnnulus(0.0003,0.002,0.0001,voltage=100000,xcent=0.001,ycent=0.001,zcent=0.0002)
# installconductor(pierce_1)
# #
# pierce_cone = ZAnnulus(0.0003,0.002,0.001,voltage=0,xcent=0.001,ycent=0.001,zcent=0.0004)
# installconductor(pierce_cone)
#
# rsrf = [0.0003 , 0.001,  0.001  ]
# zsrf = [0.0005 ,  0.0015,  0.0005]
# pierce_cone = ZSrfrv(rsrf=rsrf,zsrf=zsrf,voltage=10000000,xcent=0.001,ycent=0.001,zcent=0.001)


# # --- Create aperture plate
# plate = ZRoundedCylinderOut(radius=rplate, length=plate_width, radius2=rround, voltage=0., zcent=zplate)

# installconductor(plate,dfill=largepos)
#
# --- Pipe in the solenoid transport
# pipe = ZCylinderOut(radius=solenoid_radius, zlower=zplate+plate_width/2., zupper=zplate+nsolenoids*solenoid_period)

# installconductor(pipe)

# --- Setup the particle scraper
# scraper = ParticleScraper([pierce_1,pierce_cone])

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
    pfzx(plotsg=1, cond=0, titles=False)
    # pierce_1.draw(filled=150)
    # pierce_2.draw(filled=150)
    # pierce_cone.draw(filled=150)
    # source.draw(filled=150, fullplane=False)
    # plate.draw(filled=100, fullplane=False)
    # pipe.draw(filled=100, fullplane=False)
    # plotsolenoids()
    ppzx(titles=False)
    limits(w3d.zmminglobal, w3d.zmmaxglobal, 0., 0.002)
    ptitles('Hot plate source into solenoid transport', 'Z (m)', 'R (m)')
    refresh()

    window(1)
    fma()
    # pfzx(plotrho=1)
    #pzrhomax()
    # pzppcell()
    pzvzrms()

    limits(w3d.zmminglobal, w3d.zmmaxglobal, 0., 5000)
    refresh()

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
