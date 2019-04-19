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

def scharge_efield(beam_current,beam_velocity,beam_radius,sample_radius=None):
    """Calculate the electric field at the edge of a beam
    Eq. 44 at https://arxiv.org/pdf/1401.3951.pdf
    returns one value: V/m
    Beam velocity is in m/s
    """
    if(sample_radius == None):
        sample_radius=beam_radius
    return ((beam_current/(2.0*(math.pi)*constants.epsilon_0*beam_velocity)) * (sample_radius/(beam_radius**2.0)))

def e_field_ring_of_charge(charge,radial_distance,axial_distance,ring_radius):
    '''
    Equations from @ mandre2007
    Electric field radial and axial components for the off-axis electric field of
    a infinitesimally thick ring of charge.
    '''
    if(axial_distance == 0):
        axial_distance = 1e-10
    little_q = ((radial_distance**2.0) + (ring_radius**2.0) + (axial_distance**2.0) + (2.0*radial_distance*ring_radius))
    mu = (4.0*radial_distance*ring_radius)/little_q
    s = (charge/(4.0*math.pi*constants.epsilon_0))*(2.0/(math.pi*(little_q**(3.0/2.0))*(1.0-mu)))

    if(mu != 0): #ignore the singularity at radial_distance=0
        radial_electric_field = s*((1.0/mu)*((2.0*ring_radius*special.ellipk(math.sqrt(mu))*(1-mu))- special.ellipe(math.sqrt(mu))*
                                (2.0*ring_radius-(mu*(radial_distance+ring_radius)))))
    else:
        radial_electric_field = 0

    axial_electric_field = s*axial_distance*special.ellipe(math.sqrt(mu))

    return radial_electric_field, axial_electric_field


X=0
Y=1

beam_radius = 0.002
particle_position = [0.0,beam_radius]

particle_mass = 26.0*amu
initial_energy = 100 #eV
initial_velocity = math.sqrt((2.0*initial_energy*constants.electron_volt)/(particle_mass))

particle_velocity = [initial_velocity,0.0]

sim_length = 0.01

timestep = (sim_length/1000.0)/initial_velocity

beam_current = 0.01

position_history_x = []
position_history_y = []
force_history_y = []
history_electric_field_x = []
history_electric_field_y = []

RING_CHARGE = 2
RING_X = 0
RING_RADIUS = 1
rings = [[0.005,0.1,10000*constants.e]]#x,radius,charge

while(particle_position[0] < sim_length):
    particle_position = np.add(particle_position,np.multiply(particle_velocity,timestep))



    electric_field_x = 0
    electric_field_y = 0
    for ring in rings:
        radial_distance = particle_position[1]
        axial_distance = math.fabs(particle_position[0]-ring[RING_X])
        # radial_distance = particle_position[1]
        radial_electric_field, axial_electric_field = e_field_ring_of_charge(ring[RING_CHARGE],radial_distance,axial_distance,ring[RING_RADIUS])
        electric_field_x+=axial_electric_field
        electric_field_y+=radial_electric_field

    electric_field_x /= len(rings)
    electric_field_y /= len(rings)

    scharge_radial_acceleration = (scharge_efield(beam_current,particle_velocity[X],particle_position[Y])*constants.e)/particle_mass
    particle_velocity[Y] += scharge_radial_acceleration*timestep
    print(radial_acceleration)
    print(particle_velocity)

    position_history_x.append(particle_position[0])
    position_history_y.append(particle_position[1])
    force_history_y.append(radial_acceleration)
    history_electric_field_x.append(electric_field_x)
    history_electric_field_y.append(electric_field_y)



fig = plt.figure()
plt.subplot(2, 2, 1)
plt.plot(position_history_x,position_history_y)
# plt.scatter(position_history_x,position_history_y, s=2.0)
# for i in range(0,len(position_history_x)):
#     plt.plot((position_history_x[i],position_history_x[i]),(0,position_history_y[i]))

for ring in rings:
    plt.scatter([ring[RING_X]],[ring[RING_RADIUS]])

plt.subplot(2, 2, 2)
plt.plot(position_history_x,force_history_y)
plt.subplot(2, 2, 3)
plt.plot(position_history_x,history_electric_field_x)
plt.plot(position_history_x,history_electric_field_y)

# plt.scatter(position_history_x,position_history_y)
ax = fig.add_subplot(224, projection='3d')
plt.show()
plt.pause(10)