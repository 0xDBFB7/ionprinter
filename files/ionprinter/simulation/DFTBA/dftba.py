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


MEAN_TOLERANCE = 1.e-10


def ag_mean(a,b,tolerance):
    '''
    Computes arithmetic-geometric mean of A and B
    https://scipython.com/book/chapter-2-the-core-python-language-i/questions/the-arithmetic-geometric-mean/
    '''
    while abs(a-b) > tolerance:
        a, b = (a + b) / 2.0, math.sqrt(a * b)
    return b

def first_kind_elliptic_integral(k):
    '''
    Calculate K(k) - okay?
    https://dlmf.nist.gov/19
    §19.8(i) Gauss’s Arithmetic-Geometric Mean (AGM)
    '''
    return (math.pi/2.0)/ag_mean(1,math.sqrt(1.0-(k**2.0)), MEAN_TOLERANCE)

def compute_big_M(m):
    '''
    Prerequisite for E(m)
    '''
    return (math.pi/(2.0*first_kind_elliptic_integral(m)*math.sqrt(1-m**2.0)))

def compute_delta(m):
    '''
    Prerequisite for E(m)
    '''
    return (1.0/math.sqrt(1.0-(m**2.0)))

def differentiate_delta_over_M(m):
    '''
    Another prerequisite for E(m)
    Numerically differentiate the quotient delta/M
    '''
    dm=1.0e-6
    return (((compute_delta(m+dm)/compute_big_M(m+dm))-(compute_delta(m)/compute_big_M(m)))/dm)

def second_kind_elliptic_integral(m):
    '''
    Calculate E(m)inem

    E says blow, we increase emittance
    receive remittance
    percieve:
    society's been enhanced
    oh we got them in a trance
    science's now advanced
    in common parlance:
    dance

    From @ kurzweg2012

    Efficiency could be improved by eliminating duplicate calls of big_M and compute_delta(m)
    '''
    return (math.pi/2.0)/((1.0/(compute_big_M(m)*compute_delta(m))) + (m*differentiate_delta_over_M(m)))

# def e_field_ring_of_charge(radial_distance,axial_distance,ring_radius,):
#     '''
#     Equations from @ mandre2007
#     Electric field radial and axial components for the off-axis electric field of
#     a ring of charge.
#     Takes
#     '''
#     little_q_ = radial_distance**2.0
#     mu_ = 4.0



class TestAll(unittest.TestCase):

    def test_ag_mean(self):
        self.assertAlmostEqual(ag_mean(24,6,1e-7), 13.458, places=3)

    def test_first_kind_elliptic_integral(self):
        self.assertAlmostEqual(first_kind_elliptic_integral(0.5), 1.685751, places=3)
        self.assertAlmostEqual(first_kind_elliptic_integral(0), 1.57079632, places=3)

    def test_big_M(self):
        self.assertAlmostEqual(compute_big_M(0.5), 1.07595, places=3)

    def test_differentiate_delta_over_M(self):
        self.assertAlmostEqual(differentiate_delta_over_M(0), 1.6, places=3)
        self.assertAlmostEqual(differentiate_delta_over_M(0.9), 1.6, places=3)

    def test_second_kind_elliptic_integral(self):
        self.assertAlmostEqual(second_kind_elliptic_integral(0), 1.5707963, places=3)
        self.assertAlmostEqual(second_kind_elliptic_integral(0.9), 1.15, places=3)


if __name__ == '__main__':
    unittest.main()
