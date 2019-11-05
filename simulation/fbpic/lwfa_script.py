# -------
# Imports
# -------
import numpy as np
from scipy.constants import c, e, m_e, m_p
# Import the relevant structures from fbpic
from fbpic.main import Simulation
from fbpic.lpa_utils.laser import add_laser
from fbpic.openpmd_diag import FieldDiagnostic, \
    ParticleDiagnostic, ParticleChargeDensityDiagnostic, \
    set_periodic_checkpoint, restart_from_checkpoint
import math
# ----------
# Parameters
# ----------

# Whether to use the GPU
use_cuda = True

# Order of the stencil for z derivatives in the Maxwell solver.
# Use -1 for infinite order, i.e. for exact dispersion relation in
# all direction (adviced for single-GPU/single-CPU simulation).
# Use a positive number (and multiple of 2) for a finite-order stencil
# (required for multi-GPU/multi-CPU with MPI). A large `n_order` leads
# to more overhead in MPI communications, but also to a more accurate
# dispersion relation for electromagnetic waves. (Typically,
# `n_order = 32` is a good trade-off.)
# See https://arxiv.org/abs/1611.05712 for more information.
n_order = -1

# The simulation box
Nz = 1000         # Number of gridpoints along z
zmax = 1e-10     # Right end of the simulation box (meters)
zmin = 0   # Left end of the simulation box (meters)
Nr = 300          # Number of gridpoints along r

#Dan's note: as distortion is typically present around the axis, Nr should be
#sufficiently high for the smallest beam radius to be described by >5 cells

rmax = 1e-10    # Length of the box along r (meters)
Nm = 2           # Number of modes used

# The simulation timestep
dt = 1e-15   # Timestep (seconds)
simulation_length = 0.000001
# The particles
p_zmin = 0.e-6   # Position of the beginning of the plasma (meters)
n_He = 2.e24     # Density of Helium atoms
n_N = 1.e24      # Density of Nitrogen atoms
p_nz = 1         # Number of particles per cell along z
p_nr = 2         # Number of particles per cell along r
p_nt = 4         # Number of particles per cell along theta

v_window = c       # Speed of the window

# The diagnostics and the checkpoints/restarts
diag_period = 50         # Period of the diagnostics in number of timesteps
checkpoint_period = 100  # Period for writing the checkpoints
track_electrons = False  # Whether to track and write particle ids

input_neutral_beam_radius = 0.0025
input_neutral_beam_radius = 0.0025

ramp_length = 0.0005
def input_beam( z, r ) :
    """Returns relative density at position z and r"""
    # Allocate relative density
    n = np.ones_like(z)
    # # Make sine-like ramp
    n = np.where( z<ramp_length, np.sin(np.pi/2*z/ramp_length)**2, n )
    # # Supress density before the ramp
    n = np.where( r>input_neutral_beam_radius, 0., n )
    return(n)


# The interaction length of the simulation (meters)
# L_interact = 50.e-6 # increase to simulate longer distance!
# # Interaction time (seconds) (to calculate number of PIC iterations)
# T_interact = ( L_interact + (zmax-zmin) ) / v_window
# (i.e. the time it takes for the moving window to slide across the plasma)

# def field_func( F, x, y, z, t , amplitude, length_scale ):
#     return( F + amplitude * math.cos( 2*np.pi*z/length_scale ) )


# ---------------------------
# Carrying out the simulation
# ---------------------------

if __name__ == '__main__':

    # Initialize the simulation object
    sim = Simulation( Nz, zmax, Nr, rmax, Nm, dt,
        zmin=zmin, boundaries='periodic', initialize_ions=False,
        n_order=n_order, use_cuda=use_cuda)
    # By default the simulation initializes an electron species (sim.ptcl[0])
    # Because we did not pass the arguments `n`, `p_nz`, `p_nr`, `p_nz`,
    # this electron species does not contain any macroparticles.
    # It is okay to just remove it from the list of species.
    sim.ptcl = []

    # Add the Helium ions (pre-ionized up to level 1),
    # the Nitrogen ions (pre-ionized up to level 5)
    # and the associated electrons (from the pre-ionized levels)
    atoms = sim.add_new_species( q=-e, m=4.*m_p, n=n_He,
        dens_func=input_beam, p_nz=p_nz, p_nr=p_nr, p_nt=p_nt, p_zmin=p_zmin,continuous_injection=True)

    #created electrons
    elec = sim.add_new_species( q=-e, m=m_e)

    # Activate ionization of He ions (for levels above 1).
    # Store the created electrons in the species `elec`
    atoms.make_ionizable('Al', target_species=elec, level_start=0 )

    # if track_electrons:
    elec.track( sim.comm )
    atoms.track( sim.comm )

    # Configure the moving window
    #sim.set_moving_window( v=v_window )

    # Add diagnostics
    sim.diags = [
                FieldDiagnostic( diag_period, sim.fld, comm=sim.comm ),
                ParticleDiagnostic( diag_period,
                    {"electrons": elec,"atoms": atoms},
                    comm=sim.comm ),
                # Since rho from `FieldDiagnostic` is 0 almost everywhere
                # (neutral plasma), it is useful to see the charge density
                # of individual particles
                ParticleChargeDensityDiagnostic( diag_period, sim,
                    {"electrons": elec,"atoms": atoms} )
                ]

    # Number of iterations to perform
    N_step = int(simulation_length/sim.dt)

    ### Run the simulation
    sim.step( N_step )
    print('')
