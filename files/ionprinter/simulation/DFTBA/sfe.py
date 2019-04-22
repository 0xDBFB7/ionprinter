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
from time import sleep
import pickle
import random

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

beam_radius = 0.0005

particle_mass = 26*amu#26.0*amu
initial_energy = 100 #eV
initial_velocity = math.sqrt((2.0*initial_energy*constants.electron_volt)/(particle_mass))

beam_sim_length = 0.01

e_field_sim_height = 0.01
e_field_sim_length = 0.01

mesh_x = 50
mesh_y = 50

mesh_scale_x = mesh_x/e_field_sim_length
mesh_scale_y = mesh_y/e_field_sim_height

timestep = (beam_sim_length/500.0)/initial_velocity

beam_current = 0.003

beam_count = 5

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


diagnostic_count = 7

X_POSITIONS = 0
Y_POSITIONS = 1
ENERGY = 2
SPACE_CHARGE = 3
E_X = 4
E_Y = 5
V_X = 6
V_Y = 7
COLLISION = 8

einzel_1_voltage = 0
einzel_2_voltage = 0
einzel_3_voltage = 0

drift_distance = 0.3


def beam_waist(diagnostics):
    shortest = len(diagnostics[0][V_Y])
    for beam in diagnostics:
        if(len(beam[V_Y]) < shortest):
            shortest = len(beam[V_Y])

    idx = np.argmin(diagnostics[0][V_Y][0:shortest])
    Vy = 0
    for beam in diagnostics:
        Vy += math.fabs(beam[Y_POSITIONS][idx]/drift_distance + beam[V_Y][idx])

    return Vy

def velocity_always_positive(diagnostics):
    for beam in diagnostics:
        if(np.amin(beam[V_X]) < 0):
            return False
    return True

def final_energy(diagnostics):
    # minimum_energy = 0
    # minimum_energy
    # for beam in diagnostics:
    #     for idx,Vy in enumerate(beam[V_Y]):
    #
    energies = []
    for beam in diagnostics:
        energies.append(np.argmin(beam[ENERGY][np.argmin(beam[V_Y])]))

    return np.sum(energies)


root_iteration = 0

# for beam_current in np.linspace(0.001,0.02,10):
graph = 0

while(True):
    start = time.time()

    potentials = np.zeros(shape=(mesh_x,mesh_y))

    beam_radius = random.uniform(0.0005, 0.001)

    y = random.randint(int(beam_radius*mesh_scale_y),mesh_y)
    x_initial = random.randint(0,10)
    length = random.randint(1,20)
    height = random.randint(1,10)
    gap1 = random.randint(1,5)
    gap2 = random.randint(1,20)
    v1 = random.randint(-40000,40000)
    v2 = random.randint(-40000,40000)
    v3 = random.randint(-40000,40000)

    # 0.0034333921296351095 20 6 13 5 4 12 -27300 24767 -31576
    # beam_radius = 0.00343339212
    # y=20
    # x_initial = 6
    # length = 13
    # height = 5
    # gap1 = 4
    # gap2 = 12
    # v1 = -27300
    # v2 =  24767
    # v3 = -31576
    # if(len(sys.argv) > 4):
    #     beam_radius = float(sys.argv[1])
    #     y = int(sys.argv[2])
    #     x_initial = int(sys.argv[3])
    #     length = int(sys.argv[4])
    #     height = int(sys.argv[5])
    #     gap1 = int(sys.argv[6])
    #     gap2 = int(sys.argv[7])
    #     v1 = float(sys.argv[8])
    #     v2 = float(sys.argv[9])
    #     v3 = float(sys.argv[10])
    #     graph=1
    print(beam_radius,y,x_initial,length,height,gap1,gap2,v1,v2,v3)
    potentials[y:y+height,x_initial:x_initial+length] = v1
    potentials[y:y+height,x_initial+length+gap1:x_initial+(length*2)+gap1] = v2
    potentials[y:y+height,x_initial+(length*2)+(gap1+gap2):x_initial+(length*3)+(gap1+gap2)] = v3

    # potentials[10:11,0:1] = 10000
    #
    # potentials[10:11,3:4] = 1
    #
    # potentials[10:11,7:8] = 1
    #
    # potentials[10:11,8:9] = 1000

    BC = create_boundary(potentials)
    potentials = jacobi_relax_laplace(potentials,BC.copy(),1.0)

    stop = time.time()
    # print("Relaxation took {} seconds".format(stop-start))

    diagnostics = []

    plt.clf()

    collision = False

    for beam_index, current_beam_radius in enumerate(np.linspace(beam_radius,beam_radius/beam_count,beam_count)):
        beam_iteration = 0
        particle_position = [0.0,current_beam_radius]
        particle_velocity = [initial_velocity,0.0]

        beam_diagnostics = [[],[],[],[],[],[],[],[],[],[]] #beam count, variable

        start = time.time()

        while(particle_position[0] < beam_sim_length and particle_position[0] >= 0 and beam_iteration < 1000 and particle_position[1] < 1):

            particle_position = np.add(particle_position,np.multiply(particle_velocity,timestep))

            beam_diagnostics[X_POSITIONS].append(particle_position[0])
            beam_diagnostics[Y_POSITIONS].append(particle_position[1])

            beam_diagnostics[COLLISION].append(False)

            if((particle_position[X]*mesh_scale_x)+1 < mesh_x and (particle_position[Y]*mesh_scale_y)+1 < mesh_y and
                (particle_position[X]*mesh_scale_x)+1 >= 0 and (particle_position[Y]*mesh_scale_y)+1 >= 0):#make sure that we're within the e-field region

                #extract a 2x2 array around the particle
                sub_array = potentials[int(particle_position[Y]*mesh_scale_y):int(particle_position[Y]*mesh_scale_y)+2,
                                        int(particle_position[X]*mesh_scale_x):int(particle_position[X]*mesh_scale_x)+2]

                grad = np.gradient(sub_array)[0]
                electric_field_x = grad[0][0]*mesh_scale_x #convert to volts/meter
                electric_field_y = grad[1][0]*mesh_scale_y

                #check between bottom of plot and current beam envelope

                # print(BC[int(beam_diagnostics[X_POSITIONS][beam_iteration-1]*mesh_scale_x):int(particle_position[X]*mesh_scale_x),
                #         0:int(particle_position[Y]*mesh_scale_y)])
                if(BC[0:int(particle_position[Y]*mesh_scale_y),
                        int(beam_diagnostics[X_POSITIONS][beam_iteration-1]*mesh_scale_x):int(particle_position[X]*mesh_scale_x)].any()):
                    beam_diagnostics[COLLISION][beam_iteration] = True
                    collision = True
                    # print("collision at ", int(particle_position[X]*mesh_scale_x))
            else:
                electric_field_x = 0
                electric_field_y = 0

            if(beam_index):
                #not accurate if beam velocities diverge significantly!
                #Fixme!
                try:
                    initial_beam_radius = diagnostics[0][Y_POSITIONS][beam_iteration] #fails if first particle doesn't travel as far? Can't really happen
                except IndexError:
                    initial_beam_radius = diagnostics[0][Y_POSITIONS][-1]
            else:
                initial_beam_radius = particle_position[Y]

            scharge_efield_y = scharge_efield(beam_current,particle_velocity[X],initial_beam_radius,particle_position[Y])
            acceleration_y = ((electric_field_y + scharge_efield_y)*constants.e)/particle_mass
            acceleration_x = (electric_field_x*constants.e)/particle_mass


            particle_velocity[X] += acceleration_x*timestep
            particle_velocity[Y] += acceleration_y*timestep

            beam_diagnostics[SPACE_CHARGE].append(scharge_efield_y)
            beam_diagnostics[E_X].append(electric_field_x)
            beam_diagnostics[E_Y].append(electric_field_y)

            beam_diagnostics[V_X].append(particle_velocity[X])
            beam_diagnostics[V_Y].append(particle_velocity[Y])

            particle_energy = (0.5*particle_mass*(np.linalg.norm(particle_velocity)**2.0))/constants.electron_volt
            beam_diagnostics[ENERGY].append(particle_energy)

            if(beam_diagnostics[COLLISION][beam_iteration]):
                break

            beam_iteration += 1

        diagnostics.append(beam_diagnostics)

        stop = time.time()
        # print("Single beam sim took {} seconds".format(stop-start))

        if(graph):
            labels = (beam_index == 0)
            plt.subplot(3, 2, 1)
            plt.title('Beam envelope')
            plt.plot(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][Y_POSITIONS], label=str(current_beam_radius))
            # plt.scatter(diagnostic_collision,[0.0025]*len(diagnostic_collision))
            plt.xlabel("X (m)")
            plt.ylabel("Y (m)")
            plt.legend()

            plt.ylim((0,e_field_sim_height))
            plt.subplot(3, 2, 2)
            plt.title('Electric field')
            plt.plot(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][E_X], label='Ex' if labels else "")
            plt.plot(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][E_Y], label='Ey' if labels else "")
            plt.plot(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][SPACE_CHARGE], label='Scharge' if labels else "")
            plt.xlabel("X (m)")
            plt.ylabel("E V/m")
            plt.legend()

            plt.subplot(3, 2, 3)
            plt.title('Particle energy')
            plt.plot(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][ENERGY], label='energy' if labels else "")
            plt.xlabel("X (m)")
            plt.ylabel("Energy (eV)")
            # plt.plot(position_history_x,history_ax_distance, label='Ax')
            plt.legend()
            plt.subplot(3, 2, 4)
            plt.title('Electrode potential')
            plt.imshow(potentials,origin='lower')
            plt.xlabel("X (m)")


            plt.tight_layout()
            plt.subplot(3, 2, 5)

            plt.scatter(diagnostics[beam_index][X_POSITIONS],diagnostics[beam_index][COLLISION])
            # ax = fig.add_subplot(224, projection='3d')
            # plt.show()
            plt.pause(0.05)

    if(not collision and beam_waist(diagnostics) < 100 and final_energy(diagnostics) < 100 and velocity_always_positive(diagnostics)):
        print("Success!")
        pickle.dump(potentials,root_iteration + ".dump")
        break
    print(root_iteration)
    root_iteration += 1

    # if(graph):
    #     plt.show()
