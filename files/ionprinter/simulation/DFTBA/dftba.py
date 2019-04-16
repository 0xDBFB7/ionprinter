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


def e_field_ring_of_charge(charge,radial_distance,axial_distance,ring_radius):
    '''
    Equations from @ mandre2007
    Electric field radial and axial components for the off-axis electric field of
    a ring of charge.
    Takes
    '''
    little_q = ((radial_distance**2.0) + (ring_radius**2.0) + (axial_distance**2.0) + (2.0*radial_distance*ring_radius))
    mu = (4.0*radial_distance*ring_radius)/little_q
    s = (charge/(4.0*math.pi*constants.epsilon_0))*(2.0/(math.pi*(little_q**(3.0/2.0))*(1.0-mu)))

    radial_electric_field = s*((1.0/mu)*((2.0*ring_radius*special.ellipk(math.sqrt(mu))*(1-mu))- special.ellipe(math.sqrt(mu))*
                            (2.0*ring_radius-(mu*(radial_distance+ring_radius)))))

    axial_electric_field = s*axial_distance*special.ellipe(math.sqrt(mu))

    return radial_electric_field,axial_electric_field

class TestAll(unittest.TestCase):

    def test_e_field_ring_of_charge(self):
        self.assertAlmostEqual(e_field_ring_of_charge(1.0,0,1e10,1e-10),(constants.Coulomb*1)/((1e10)**2.0), places=3)

if __name__ == '__main__':
    unittest.main()
