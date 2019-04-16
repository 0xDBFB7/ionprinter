import matplotlib.pyplot as plt
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

RADIAL = 0
AXIAL = 1
X = 0
Y = 1
Z = 2
VX = 3
VY = 4
VZ = 5
MASS = 6
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


# def compute_ring_thickness():
#     return


def circular_distribution(position,direction,energy,inner_radius,outer_radius,temperature,number,charge,mass):
    '''
    Returns a list with a circle of particles.
    Position is a 3-vector (x,y,z).
    Direction is a 3-vector (x,y,z) of unit length.
    Energy is in eV.
    '''
    particles = []
    for i in range(0,number):

        length = np.sqrt(np.random.uniform(inner_radius**2.0, outer_radius**2.0)) #sqrt to get a uniform particle distribution
        angle = math.pi * np.random.uniform(0, 2)

        x,y,z = np.cross([length * math.cos(angle),length * math.sin(angle),0],direction) #rotate vector to correct position
        x += position[X]
        y += position[Y]
        z += position[Z] #and offset to the correct position

        #print_species.mass*amu
        v = math.sqrt((2.0*energy*constants.electron_volt)/(mass))
        vx = v*direction[X]
        vy = v*direction[Y]
        vz = v*direction[Z]

        particles.append([x,y,z,vx,vy,vz,charge,mass])

    return particles

def beam_envelope():
    return

# def compute_beam(beam_position,beam_particles,beam_current,particle_charge,timestep):
#     '''
#     Root-level beam convergence.
#
#     Takes beam parameters as seperate lists so that porting to C++ will be simple
#     '''
#     beams = []
#     current_particles
#     distance(math.sqrt())

# def plot_beam(particles,rings):


class TestAll(unittest.TestCase):

    def test_e_field_ring_of_charge(self):

        #Compare to point charge at sufficient axial distance
        self.assertAlmostEqual(e_field_ring_of_charge(1e10,0,1e10,1e-10)[AXIAL],((8.987551878368e9)*1e10)/((1e10)**2.0), places=3)
        #Compare to point charge at sufficient radial distance
        self.assertAlmostEqual(e_field_ring_of_charge(1e10,1e10,0,1e-10)[RADIAL],((8.987551878368e9)*1e10)/((1e10)**2.0), places=3)

    def test_circular_distribution(self):
        p = circular_distribution((0,0,0),(0,0,1),10,inner_radius,outer_radius,temperature,number,charge)
        self.assertTrue(p[X])
        # self.assertAlmostEqual(p[X],, places=3)


    # def test_compute_beam(self):



if __name__ == '__main__':
    unittest.main()
