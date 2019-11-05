epsilon = 8.85*(10**-12.0) #vacuum permittivity

def electron_charge():
    return 1.602*10**-19

def scharge_efield(beam_current,beam_velocity,beam_radius,sample_radius=None):
    """Calculate the electric field at the edge of a beam
    Eq. 44 at https://arxiv.org/pdf/1401.3951.pdf
    returns one value: V/m
    Beam velocity is in m/s
    """
    if(sample_radius == None):
        beam_radius=sample_radius
    return ((beam_current/(2.0*(math.pi)*epsilon*beam_velocity)) * (sample_radius/(beam_radius**2.0)))

def scharge_force(beam_current,beam_velocity,beam_radius):
    scharge_force = scharge_efield(beam_current,beam_velocity,beam_radius)*electron_charge()
    return scharge_force

def scharge_bfield(beam_current,beam_velocity,beam_radius):
    """Calculate the minimum B field required to counteract scharge beam dispersion
    Return value is in teslas
    """
    required_bfield = scharge_force(beam_current,beam_velocity,beam_radius)/(electron_charge()*beam_velocity)
    return required_bfield, scharge_force
