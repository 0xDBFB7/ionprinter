import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import time
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm


plt.ion()
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_zlim(-1.01, 1.01)


def draw_plot(x, y, U):
    ax.clear()
    ax.set_zlim(-1.01, 1.01)
    ax.plot_surface(x, y, U, rstride=1, cstride=1, cmap=cm.coolwarm,
                    linewidth=0, antialiased=True)
    plt.pause(1e-5)


# Create 21x21 mesh grid
m = 21
mesh_range = np.arange(-1, 1, 2/(m-1))
x, y = np.meshgrid(mesh_range, mesh_range)

# Initial condition
U = np.exp(-5 * (x**2 + y**2))

draw_plot(x, y, U)

n = list(range(1, m-1)) + [m-2]
e = n
s = [0] + list(range(0, m-2))
w = s


def pde_step(U):
    """ PDE calculation at a single time step t  """
    return (U[n, :]+U[:, e]+U[s, :]+U[:, w])/4.


k = 5
U_step = U

for it in range(500):
    U_step = pde_step(U_step)

    # Every k steps, draw the graphics
    if it % k == 0:
        draw_plot(x, y, U_step)

while True:
    plt.pause(1e-5)
