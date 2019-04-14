import matplotlib.pyplot as plt
from mendeleev import element
from thermo.chemical import Chemical
import sympy.physics.units as units
import scipy.constants as constants
amu = constants.physical_constants["atomic mass unit-kilogram relationship"][0]
import math
import sys
import numpy as np
import unittest

def ring_of_charge():
    '''
    Equations from @ mandre2007
    Electric field radial and axial components for the off-axis electric field of
    a ring of charge.
    '''

class TestAll(unittest.TestCase):

    def test_scharge_efield(self):
        self.assertAlmostEqual(ef/10**6, 36.0032524, places=3)
