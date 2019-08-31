import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math

SIZE_X = 128


u = numpy.zeros((SIZE_X))
b = numpy.zeros((SIZE_X))

# for x in range(10,20):
#     u[x] = 1.0
#     b[x] = 1.0

u[64] = 1.0
b[64] = 1.0

def jacobi(u,b,f):
    T = u.copy()
    for x in range(1, (u.shape[0] - 2)):
        if(b[x] == 0):
            T[x] = (u[x+1] + u[x-1] + f[x])/2.0
    return T

def residual(u,b,f):
    rows = u.shape[0]

    h = 1.0/(rows-1)
    h2 = 1.0/(h*h)

    R = numpy.zeros(rows)
    for x in range(0,rows-1): #careful with the edge boundaries; they must be included!
        if(b[x] == 0):
            R[x] = f[x] - (u[x+1] + u[x-1] - 2.0*u[x])
        else:
            R[x] = 0
    return R

f = numpy.zeros((SIZE_X))
r = -residual(u,b,f)

plt.figure()
plt.subplot(2, 3, 1)
plt.gca().set_title('Potentials')
plt.plot(u)


plt.draw()
plt.show()
