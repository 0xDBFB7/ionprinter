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

beam_radius = 0.002

particle_mass = 26.0*amu
initial_energy = 1000 #eV
initial_velocity = math.sqrt((2.0*initial_energy*constants.electron_volt)/(particle_mass))

beam_sim_length = 0.01

e_field_sim_height = 0.005
e_field_sim_length = 0.005

mesh_x = 50
mesh_y = 50

mesh_scale_x = mesh_x/e_field_sim_length
mesh_scale_y = mesh_y/e_field_sim_height

timestep = (beam_sim_length/500.0)/initial_velocity

beam_current = 0.001

beam_count = 5
# def box(x1,y1,x2,y2):
#     for x in range(1,mesh_x-1):
#         for y in range(1,mesh_x-1):
#             if()
#

start = time.time()

def create_boundary(potentials):
    '''
    Takes a (mostly zero) array with the desired geometry potentials
    '''
    BC = np.zeros_like(potentials, dtype=bool)
    BC[potentials.nonzero()] = True #build a boundary condition array from initial Dirchlet conditions

    #edges wreak all sorts of havoc
    return BC

def jacobi_relax_laplace(potentials,BC,tolerance):
    '''
    Computes electric field from points using a simple Laplace relaxation.
    '''
    BC[:,0] = BC[:,-1] = BC[0,:] = BC[-1,:] = True #thanks uhoh! https://physics.stackexchange.com/a/363868/101785
    BC_inv = np.invert(BC) #get free grid points

    kernel = 0.25*np.array([[0, 1, 0], [1, 0, 1], [0, 1, 0]], dtype=float) #we want convolve to add adjacent potentials
    convergence = 0
    iterations = 0
    while(True):
        potentials[BC_inv] = convolve(potentials, kernel)[BC_inv]
        new_convergence = np.sum(np.absolute(potentials)) #
        if(math.fabs(convergence-new_convergence) < tolerance):
            break
        convergence = new_convergence
        iterations += 1
    return potentials



potentials = np.zeros(shape=(mesh_x,mesh_y))
potentials[10,10] = 1000.0

BC = create_boundary(potentials)
potentials = jacobi_relax_laplace(potentials,BC.copy(),1.0)

diagnostic_count = 7
#
diagnostics = [([[]]*diagnostic_count)]*beam_count #beam count, variable
# #variable can be
X_POSITIONS = 0
Y_POSITIONS = 1
ENERGY = 2
SPACE_CHARGE = 3
E_X = 4
E_Y = 5
COLLISION = 6

# for beam_index, current_beam_radius in enumerate(np.linspace(beam_radius,beam_radius/beam_count,beam_count)):
current_beam_radius = beam_radius
beam_index = 0
beam_iteration = 0
particle_position = [0.0,current_beam_radius]
print(timestep)
particle_velocity = [initial_velocity,0.0]

while(particle_position[0] < beam_sim_length):

    particle_position = np.add(particle_position,np.multiply(particle_velocity,timestep))

    diagnostics[beam_index][X_POSITIONS].append(particle_position[0])
    diagnostics[beam_index][Y_POSITIONS].append(particle_position[1])

    diagnostics[beam_index][COLLISION].append(False)

    if((particle_position[X]*mesh_scale_x)+1 < mesh_x and (particle_position[Y]*mesh_scale_y)+1 < mesh_y):#make sure that we're within the e-field region

        #extract a 2x2 array around the particle
        sub_array = potentials[int(particle_position[X]*mesh_scale_x):int(particle_position[X]*mesh_scale_x)+2,
                                int(particle_position[Y]*mesh_scale_y):int(particle_position[Y]*mesh_scale_y)+2]

        grad = np.gradient(sub_array)[0]
        electric_field_x = grad[0][0]*mesh_scale_x #convert to volts/meter
        electric_field_y = grad[1][0]*mesh_scale_y

        #check between bottom of plot and current beam envelope
        if(BC[int(particle_position[X]*mesh_scale_x),0:int(particle_position[Y]*mesh_scale_y)].any()):
            diagnostics[beam_index][COLLISION][diagnostic_count] = True
    else:
        electric_field_x = 0
        electric_field_y = 0


    initial_beam_radius = diagnostics[0][Y_POSITIONS][beam_iteration] #fails if first particle doesn't travel as far? Can't really happen
    scharge_efield_y = scharge_efield(beam_current,particle_velocity[X],initial_beam_radius,particle_position[Y])
    acceleration_y = ((electric_field_y + scharge_efield_y)*constants.e)/particle_mass
    acceleration_x = (electric_field_x*constants.e)/particle_mass

    acceleration_y = 0
    acceleration_x = 0

    particle_velocity[X] += acceleration_x*timestep
    particle_velocity[Y] += acceleration_y*timestep


    diagnostics[beam_index][SPACE_CHARGE].append(scharge_efield_y)
    diagnostics[beam_index][E_X].append(electric_field_x)
    diagnostics[beam_index][E_Y].append(electric_field_y)

    particle_energy = (0.5*particle_mass*(np.linalg.norm(particle_velocity)**2.0))/constants.electron_volt
    diagnostics[beam_index][ENERGY].append(particle_energy)



stop = time.time()
print(stop-start)

labels = (beam_index == 0)
plt.subplot(4, 4, 1)
plt.title('Beam envelope')
plt.plot(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][Y_POSITIONS])
# plt.scatter(diagnostic_collision,[0.0025]*len(diagnostic_collision))

plt.ylim((0,0.005))
plt.subplot(4, 4, 2)
plt.title('Electric field')
plt.plot(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][E_X], label='Ex' if labels else "")
plt.plot(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][E_Y], label='Ey' if labels else "")
plt.plot(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][SPACE_CHARGE], label='Scharge')
plt.legend()

plt.subplot(4, 4, 3)
plt.title('Particle energy')
plt.plot(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][ENERGY], label='energy')
# plt.plot(position_history_x,history_ax_distance, label='Ax')
plt.legend()
plt.subplot(4, 4, 5)
plt.title('Electrode potential')
plt.imshow(potentials,origin='lower')

    # plt.scatter(position_history_x,position_history_y)
    # ax = fig.add_subplot(224, projection='3d')
    # plt.show()
plt.pause(0.05)

plt.show()
