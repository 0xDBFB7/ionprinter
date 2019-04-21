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
from scipy.interpolate import griddata
from scipy import ndimage
from scipy.signal import convolve2d
from scipy.ndimage import convolve
import findiff


def scharge_efield(beam_current,beam_velocity,beam_radius,sample_radius=None):
    """Calculate the electric field at the edge of a beam
    Eq. 44 at https://arxiv.org/pdf/1401.3951.pdf
    returns one value: V/m
    Beam velocity is in m/s
    """
    if(sample_radius == None):
        sample_radius=beam_radius
    return ((beam_current/(2.0*(math.pi)*constants.epsilon_0*beam_velocity)) * (sample_radius/(beam_radius**2.0)))





X=0
Y=1


RING_CHARGE = 2
RING_X = 0
RING_RADIUS = 1


beam_radius = 0.002
particle_position = [0.0,beam_radius]

particle_mass = 26.0*amu
initial_energy = 100 #eV
initial_velocity = math.sqrt((2.0*initial_energy*constants.electron_volt)/(particle_mass))

particle_velocity = [initial_velocity,0.0]

beam_sim_length = 0.01

e_field_sim_height = 0.005
e_field_sim_length = 0.005

mesh_x = 50
mesh_y = 50

potentials = np.zeros(shape=(mesh_x,mesh_y))
BC = np.zeros(shape=(mesh_x,mesh_y), dtype=bool)

potentials[10,10] = 1000.0
# potentials[20,10] = -60000.0

mesh_scale_x = mesh_x/e_field_sim_length
mesh_scale_y = mesh_y/e_field_sim_height

BC[10,10] = True

BC[:,0] = BC[:,-1] = BC[0,:] = BC[-1,:] = True #thanks uhoh! https://physics.stackexchange.com/a/363868/101785

timestep = (beam_sim_length/500.0)/initial_velocity

beam_current = 0.01

position_history_x = []
position_history_y = []
force_history_y = []
history_electric_field_x = []
history_electric_field_y = []
history_energy = []

# def box(x1,y1,x2,y2):
#     for x in range(1,mesh_x-1):
#         for y in range(1,mesh_x-1):
#             if()
#

start = time.time()

BC_inv = np.invert(BC)
kernel = 0.25*np.array([[0, 1, 0], [1, 0, 1], [0, 1, 0]], dtype=float)
convergence = 0
iterations = 0
while(True):
    potentials[BC_inv] = convolve(potentials, kernel)[BC_inv]
    new_convergence = np.sum(np.absolute(potentials))
    if(math.fabs(convergence-new_convergence) < 1.0):
        break
    convergence = new_convergence
    iterations += 1
print(iterations)

for beam_index in range(0,5):

    while(particle_position[0] < beam_sim_length):

        particle_position = np.add(particle_position,np.multiply(particle_velocity,timestep))

        if((particle_position[X]*mesh_scale_x)+1 < mesh_x and (particle_position[Y]*mesh_scale_y)+1 < mesh_y):

            sub_array = potentials[int(particle_position[X]*mesh_scale_x):int(particle_position[X]*mesh_scale_x)+2,
                                    int(particle_position[Y]*mesh_scale_y):int(particle_position[Y]*mesh_scale_y)+2]
            print(sub_array)
            print(int(particle_position[X]*mesh_scale_x))
            print(int(particle_position[Y]*mesh_scale_y))
            grad = np.gradient(sub_array)[0]
            print(grad)
            electric_field_x = grad[0][0]*mesh_scale_x #convert to volts/meter
            electric_field_y = grad[1][0]*mesh_scale_y
            print(electric_field_x)

            if(BC[int(particle_position[X]*mesh_scale_x),0:int(particle_position[Y]*mesh_scale_y)].any()):
                print("collision")
        else:
            electric_field_x = 0
            electric_field_y = 0

        new_scharge_efield = scharge_efield(beam_current,particle_velocity[X],particle_position[Y])*constants.e

        acceleration_y = (electric_field_y*constants.e + new_scharge_efield)/particle_mass
        acceleration_x = (electric_field_x*constants.e)/particle_mass
        particle_velocity[X] += acceleration_x*timestep
        particle_velocity[Y] += acceleration_y*timestep


        position_history_x.append(particle_position[0])
        position_history_y.append(particle_position[1])
        force_history_y.append(new_scharge_efield/constants.e)
        history_electric_field_x.append(electric_field_x)
        history_electric_field_y.append(electric_field_y)

        # history_rad_distance.append(radial_distance)
        # history_ax_distance.append(axial_distance)

        particle_energy = (0.5*particle_mass*(np.linalg.norm(particle_velocity)**2.0))/constants.electron_volt
        history_energy.append(particle_energy)

    stop = time.time()
    print(stop-start)

    plt.subplot(4, 4, 1)
    plt.title('Beam envelope')
    plt.plot(position_history_x,position_history_y)
    plt.ylim((0,0.005))
    plt.subplot(4, 4, 2)
    plt.title('Electric field')
    plt.plot(position_history_x,history_electric_field_x, label='Ex')
    plt.plot(position_history_x,history_electric_field_y, label='Ey')
    plt.plot(position_history_x,force_history_y, label='Scharge')
    plt.legend()

    plt.subplot(4, 4, 3)
    plt.plot(position_history_x,history_energy, label='energy')
    # plt.plot(position_history_x,history_ax_distance, label='Ax')
    plt.legend()
    plt.subplot(4, 4, 5)
    plt.imshow(potentials,origin='lower')

    # plt.scatter(position_history_x,position_history_y)
    # ax = fig.add_subplot(224, projection='3d')
    # plt.show()
plt.pause(0.05)
# plt.pause(10)

plt.show()
