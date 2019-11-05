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
        new_convergence = np.sum(np.absolute(potentials)) #could be sped up by only checking convergence every 100 or so operations
        if(math.fabs(convergence-new_convergence) < tolerance):
            break
        convergence = new_convergence
        iterations += 1
    return potentials

mesh_x = 20
mesh_y = 20

desired_potentials = np.zeros(shape=(mesh_x,mesh_y))

desired_potentials[3,3] = 10
desired_potentials[7,7] = -10

desired_BC = np.zeros_like(desired_potentials, dtype=bool)
desired_BC[3,3] = True
desired_potentials[7,7] = True

#total potential difference integral
# while(True):
test_potential = 30

#start with coarse mesh, then refine gradually?

found_potentials = []

lowest = 0
lowest_point = [0,0]
for x in range(0,mesh_x):
    for y in range(0,mesh_y):
        if(desired_BC[y,x]):
            continue
        potentials = np.zeros(shape=(mesh_x,mesh_y))
        potentials[y,x] = test_potential
        BC = create_boundary(potentials)
        jacobi_relax_laplace(potentials,BC,5)

        # sub_array = potentials[int(particle_position[Y]*mesh_scale_y):int(particle_position[Y]*mesh_scale_y)+2,
        #                         int(particle_position[X]*mesh_scale_x):int(particle_position[X]*mesh_scale_x)+2]
        #
        # grad = np.gradient(sub_array)[0]
        # electric_field_x = grad[0][0]*mesh_scale_x #convert to volts/meter
        # electric_field_y = grad[1][0]*mesh_scale_y

        difference = np.sum(np.absolute(np.subtract(potentials[desired_BC],desired_potentials[desired_BC])))
        if(not lowest or difference < lowest):
            lowest = difference
            lowest_point = [y,x]
        # plt.clf()
        # plt.imshow(potentials,origin='lower')
        # plt.pause(0.05)
print(lowest)
print(lowest_point)
