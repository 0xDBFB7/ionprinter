import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math

SIZE_X = 256

# epsilon_0 = 8.85×10^−12

u = numpy.zeros((SIZE_X))
b = numpy.zeros((SIZE_X))

for x in range(10,20):
    b[x] = 1.0



def residual(u, f):
    """
    f - A u
    """
    n = len(f)
    r = numpy.zeros(len(u))
    r[1:-1] = f[1:-1] - ((n+1)**2) * (2 * u[1:-1] - u[2:] - u[:-2])
    r[0]    = f[0]    - ((n+1)**2) * (2 * u[0]    - u[1])
    r[-1]   = f[-1]   - ((n+1)**2) * (2 * u[-1]   - u[-2])
    return r


while True:
    for i in range(0,100):
        for x in range(1, (u.shape[0] - 2)):
            if(b[x] == 0):
                u[x] = (u[x+1] + u[x-1])/2.0
            else:
                u[x] = b[x]


    plt.subplot(2, 3, 1)
    plt.gca().set_title('Potentials')
    plt.plot(u)

    s = numpy.zeros_like(b)
    r = residual(u,s)

    plt.subplot(2, 3, 2)
    plt.gca().set_title('Potentials')
    plt.plot(r)


    # plt.subplot(2, 3, 2)
    # plt.gca().set_title('Correction')
    # plt.plot(r)
    #
    # plt.subplot(2, 3, 3)
    # plt.gca().set_title('Prolongated')
    # plt.plot(v)

    plt.draw()
    plt.pause(0.1)
    plt.clf()
    plt.cla()
