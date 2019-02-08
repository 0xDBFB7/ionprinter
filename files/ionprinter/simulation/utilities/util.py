import math
import unittest

epsilon = 8.85*(10**-12.0) #vacuum permittivity

def electron_charge():
    return 1.602*10**-19

def scharge_efield(beam_current,beam_velocity,beam_radius):
    """Calculate the electric field at the edge of a beam
    Eq. 44 at https://arxiv.org/pdf/1401.3951.pdf
    returns one value: V/m
    Beam velocity is in m/s
    """
    return ((beam_current/(2.0*(math.pi)*epsilon*beam_velocity)) * (beam_radius/(beam_radius**2.0)))

def scharge_force(beam_current,beam_velocity,beam_radius):
    scharge_force = scharge_efield(beam_current,beam_velocity,beam_radius)*electron_charge()
    return scharge_force

def scharge_bfield(beam_current,beam_velocity,beam_radius):
    """Calculate the minimum B field required to counteract scharge beam dispersion
    Return value is in teslas
    """
    required_bfield = scharge_force(beam_current,beam_velocity,beam_radius)/(electron_charge()*beam_velocity)
    return required_bfield, scharge_force

#
# def einzel_focus_efield(V_0,V_1,focusing_period,focus_geometry_radius,y_position):
#     """Calculate
#     from Page 7 of https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19650023758.pdf
#     """
#     focusing_period_v = focusing_period*2.0*(math.pi)
#     K = V_1/(V_0*math.cosh(focusing_period_v))
#     return -0.5*electron_charge()*V_0*(K**2.0)*(focusing_period_v**2.0)*y_position

print(scharge_efield(0.178,9990,0.005)/10**6)
#0.00001963495
# print(einzel_focus_efield(20000,10000,0.001,0.001,0.0005)/10**6)

class TestAll(unittest.TestCase):

    def test_scharge_efield(self):
        ef = scharge_efield(0.05,9990,0.0025)
        self.assertAlmostEqual(ef/10**6, 36.0032524, places=3)

    # def test_einzel_efield(self):
    #     ef = einzel_focus_efield(20000,10000,0.001,0.001,0.0005)
    #     self.assertAlmostEqual(ef/10**6, 36.0032524, places=3)



    #
    # def scharge_efield_test(self):
    #     assertAlmostEqual(, second, places=4, msg=None, delta=None)
    #     assertAlmostEqual(, second, places=4, msg=None, delta=None)
    #     assertAlmostEqual(, second, places=4, msg=None, delta=None)

# unittest.main()
