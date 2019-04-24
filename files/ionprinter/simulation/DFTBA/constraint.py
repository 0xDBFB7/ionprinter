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
        new_convergence = np.sum(np.absolute(potentials)) #
        if(math.fabs(convergence-new_convergence) < tolerance):
            break
        convergence = new_convergence
        iterations += 1
    return potentials
