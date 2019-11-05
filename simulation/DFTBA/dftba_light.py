import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.transforms import Affine2D
from matplotlib.patches import Circle, PathPatch
# from mendeleev import element
# from thermo.chemical import Chemical
import sympy.physics.units as units
import scipy.constants as constants
amu = constants.physical_constants["atomic mass unit-kilogram relationship"][0]
import math
import sys
import numpy as np
import unittest
import time
from scipy import special
from scipy.stats import maxwell

RADIAL = 0
AXIAL = 1
X = 0
Y = 1
Z = 2
VX = 3
VY = 4
VZ = 5
CHARGE = 6
MASS = 7

RING_RADIUS = 6
RING_CHARGE = 7
#this probably isn't an optimal way to do this, but it is fast, and
#can easily be ported to C++ later on
#much faster than dictionary string lookup, anyhow.

def e_field_ring_of_charge(charge,radial_distance,axial_distance,ring_radius):
    '''
    Equations from @ mandre2007
    Electric field radial and axial components for the off-axis electric field of
    a infinitesimally thick ring of charge.
    '''
    little_q = ((radial_distance**2.0) + (ring_radius**2.0) + (axial_distance**2.0) + (2.0*radial_distance*ring_radius))
    mu = (4.0*radial_distance*ring_radius)/little_q
    s = (charge/(4.0*math.pi*constants.epsilon_0))*(2.0/(math.pi*(little_q**(3.0/2.0))*(1.0-mu)))

    if(mu != 0): #ignore the singularity at radial_distance=0
        radial_electric_field = s*((1.0/mu)*((2.0*ring_radius*special.ellipk(math.sqrt(mu))*(1-mu))- special.ellipe(math.sqrt(mu))*
                                (2.0*ring_radius-(mu*(radial_distance+ring_radius)))))
    else:
        radial_electric_field = 0

    axial_electric_field = s*axial_distance*special.ellipe(math.sqrt(mu))

    return radial_electric_field,axial_electric_field


def circular_distribution(position,direction,energy,inner_radius,outer_radius,axial_temperature,radial_temperature,number,charge,mass):
    '''
    Returns a list with a circle of particles.
    Position is a 3-vector (x,y,z).
    Direction is a 3-vector (x,y,z) of unit length.
    All units are SI with the exception of energy eV.
    '''
    particles = []
    for i in range(0,number):

        length = np.sqrt(np.random.uniform(inner_radius**2.0, outer_radius**2.0)) #sqrt to get a uniform particle distribution within the circle
        angle = math.pi * np.random.uniform(0, 2)

        x,y,z = np.cross([length * math.cos(angle),length * math.sin(angle),0],direction) #rotate vector to correct position
        x += position[X]
        y += position[Y]
        z += position[Z] #and offset to the correct position

        #print_species.mass*amu

        axial_v = math.sqrt((2.0*energy*constants.electron_volt)/(mass)) # determine base velocity

        axial_a = math.sqrt((constants.Boltzmann*axial_temperature)/mass) #Maxwell-Boltzmann scale parameter
        axial_r = maxwell.rvs(loc=axial_v,scale=axial_a) #then add the axial temperature

        #todo: add radial temperature here!

        vx = axial_v*direction[X]
        vy = axial_v*direction[Y]
        vz = axial_v*direction[Z]

        particles.append([x,y,z,vx,vy,vz,charge,mass])

    return particles

def compute_particles_axis(particles,center):
    '''
    Cross, norm, then average particle vectors pairwise to determine a central axis
    This may be perpendicular to the particle's motion.
    Needed to determine which way the ring should point.
    '''

    if(not particles): #sanity checking
        return center

    average_direction = [0,0,0]
    for idx,particle in enumerate(particles[0:-1]):
        v1 = np.subtract(center,particles[idx][0:3]) #get vector from center to point 1
        v2 = np.subtract(center,particles[idx+1][0:3]) # vector from center to point 2
        cross = np.cross(v1,v2)
        average_direction = np.add(average_direction,cross)
    average_direction = np.divide(average_direction,len(particles))
    return np.divide(average_direction,np.linalg.norm(average_direction)) #convert to unit vector

def compute_particles_center(particles):
    '''
    Simple center-of-mass computation
    3-vector or particles
    '''
    center = [0,0,0]

    if(not particles): #sanity checking
        return center

    for particle in particles:
        center[X] += particle[X]
        center[Y] += particle[Y]
        center[Z] += particle[Z]

    center[X] /= len(particles)
    center[Y] /= len(particles)
    center[Z] /= len(particles)

    return center

def projected_radial_distance(vector_origin, vector_direction, point):
    '''
    Given a vector position, direction, and a point, determine the "radial" distance
    All are 3-vectors, not particles.
    https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
    x=a+tn
    ((a-p) dot n)n
    '''
    #compute the vector projection to determine "radial" distance
    # return np.linalg.norm(np.subtract(i,np.multiply(np.dot(i,vector_direction),vector_direction)))
    point = np.subtract(point,vector_origin)
    distance = np.linalg.norm(np.subtract(point,(np.multiply(np.dot(point,vector_direction),vector_direction))))
    return distance

def projected_axial_distance(vector_origin, vector_direction, point):
    '''
    Given a vector position, direction, and a point, determine the "axial" distance.
    All are 3-vectors, not particles.
    '''
    i = np.subtract(point,vector_origin)
    return np.linalg.norm(np.multiply(np.dot(i,vector_direction),vector_direction))


def beam_envelope(particles,direction,position):
    '''
    Determines inner_radius and outer_radius of beam along a certain axis
    Returns all zeros if no particles are given.
    Direction and position are 3-vectors.
    '''
    if(not particles): #sanity checking
        return 0, 0

    outer_radius = 0
    inner_radius = 0

    # if(len(particles) > 20):
    #     #no use doing an expensive vector projection for particles that are nearby
    #     #sort based on simple euclidean distance from center
    #     #one of the first few items should be the furthest
    #     #
    #     #turns out that this only saves us 0.0016-0.00153995 = 0.11 ms, since sorting is also costly
    #     #oh well
    #     euclid_distances = [math.sqrt(x[0]**2.0 + x[1]**2.0 + x[2]**2.0) for x in particles]
    #     sorted_distance_indices = sorted(range(len(euclid_distances)), key=lambda i: euclid_distances[i])[int(len(euclid_distances)/10):-1]
    #     sorted_particles = [particles[i] for i in sorted_distance_indices]
    # else:
    #     #if there aren't many particles, we don't want to sort them
    #     sorted_particles = particles
    # sorted_particles = particles

    for particle in particles:
        new_radius = projected_radial_distance(position,direction,particle[0:3])
        if(new_radius > outer_radius):
            outer_radius = new_radius
        if(new_radius < inner_radius or inner_radius == 0):
            inner_radius = new_radius

    return inner_radius, outer_radius

def compute_ring_charge(beam_current, particle_average_charge, particle_average_velocity, ring_radial_step, ring_axial_step, ring_radius):
    '''
    Given a beam current in amps, compute charge density of a virtual volume 1 velocity long
    then multiply by actual ring volume
    '''
    charge_per_second = beam_current*constants.e #charge per second in Coulombs
    beam_area = (math.pi*((ring_radius+(ring_radial_step/2.0))**2.0))-(math.pi*((ring_radius-(ring_radial_step/2.0))**2.0))
    space_charge_density = charge_per_second / (beam_area*particle_average_velocity)
    ring_virtual_volume = beam_area*ring_axial_step
    ring_charge = ring_virtual_volume*space_charge_density

    return ring_charge

def average_particle_velocity(particles):
    return sum([np.linalg.norm(x[VX],x[VY],x[VZ]) for x in particles])/len(particles)

def average_particle_charge(particles):
    return np.sum([x[CHARGE] for x in particles])/len(particles)

def particle_acceleration(particle,ring):
    axial_distance = projected_axial_distance(ring[0:3],ring[3:6],particle[0:3])
    radial_distance = projected_radial_distance(ring[0:3],ring[3:6],particle[0:3])
    radial_electric_field, axial_electric_field = e_field_ring_of_charge(ring[RING_CHARGE],radial_distance,axial_distance,ring[6])
    #F=Eq
    radial_acceleration = radial_electric_field*particle[CHARGE] # radial and axial w.r.t. ring axis

    axial_acceleration = axial_electric_field*particle[CHARGE] #this is easy - axial must be along ring[3:6].

    axial_acceleration = np.multiply(ring[3:6],axial_acceleration)
    #but radial is the vector through the ring origin and the particle
    radial_vector = np.subtract(particle[0:3],ring[0:3])
    radial_vector = radial_vector/(np.linalg.norm(radial_vector)) #we want a unit radial vector
    radial_vector = np.multiply(radial_vector,radial_acceleration)



    return [,axial_acceleration] #fixme! currently relative to direction

def cloud_acceleration(particles,rings):
    '''
    Computes new positions and velocities of particles
    given current particle cloud and ring positions
    '''
    for particle in particles:
        total_acceleration = [0,0,0]
        for concentric_rings in rings:
            for ring in concentric_rings:
                total_acceleration = np.add(total_acceleration,particle_acceleration(particle,concentric_rings))
        #collision probability functions can go here


beams = [] #particle histories [], rings []
plt.ion()
plt.show()
ring_radial_step = 0.0001
ring_axial_step = 0.001
beam_length = 0.05
beam_steps = int(beam_length/ring_axial_step)
convergence_number = 10
#rings use:
#[x,y,z,dirx,diry,dirz,radius,charge]
#pretty stupid (concatenated vectors? inane), but I didn't want to deal with multidimensional vectors in C++
position = (0,0,0)
direction = (0,0,1)
beam_current = 0.002
heavy_beam = circular_distribution(position, #position
                        direction, #direction
                        10.0 #energy, eV
                        ,0.0 #inner radius, 0 meters
                        ,1.0,#outer radius, 1 meter
                        2000, #2000 Kelvin
                        100,
                        100, #1 particle
                        1.0*constants.e,26.0*amu)
particles = [[]]*beam_steps
particles[0]=heavy_beam
rings = [[]]*beam_steps
for convergence_step in range(0,convergence_number):
    for beam_step in range(0,beam_steps-1):

        inner_radius, outer_radius = beam_envelope(particles[beam_step],direction,position)

        rings[beam_step] = [] # remove all rings
        new_ring_position = np.add(position,np.multiply(direction,beam_step))
        cloud_average_charge = average_particle_charge(particles[beam_step])
        cloud_average_velocity = average_particle_velocity(particles[beam_step])

        for concentric_ring_radius in np.arange(inner_radius,outer_radius,ring_radial_step): #move outward
            new_ring_charge = compute_ring_charge(beam_current,cloud_average_charge,cloud_average_velocity,ring_radial_step,ring_axial_step,concentric_ring_radius)
            new_ring = new_ring_position + direction + [concentric_ring_radius] + [new_ring_charge]
            rings[beam_step].append(new_ring)


        cloud_acceleration = particle_acceleration()



    fig = plt.figure()
    ax = Axes3D(fig)
    # p = Circle((5, 5), 3)
    # ax.add_patch(p)
    # art3d.pathpatch_2d_to_3d(p, z=0, zdir="x")
