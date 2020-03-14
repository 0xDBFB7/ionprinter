#first attempt at incorporating spatial data into the stencil.
#failure, no convergence.

import numpy
import matplotlib.pyplot as plt

N = 10

potential = numpy.zeros(N)
delta = numpy.zeros(N)
coefficients = numpy.arange(1,1.5,0.05)
#coefficients = numpy.ones(N)
prev = numpy.zeros(N)

convergence = []
t=0

potential[0] = 10
for i in range(0,100):

    for i in range(1, N-1):
        potential[i] = ((potential[i+1]+potential[i-1])/2.0)

    for i in range(1, N-1):
        potential[i] *= coefficients[i]
    delta = potential-prev
    convergence.append(numpy.linalg.norm(delta))

    print(numpy.linalg.norm(delta))
    
    plt.subplot(2,1,1)
    plt.gca().set_title('Convergence')
    plt.plot(convergence)
    
    plt.subplot(2,1,2)
    plt.gca().set_title('Potential')
    plt.plot(potential)

    t+=1

    prev = potential.copy();
    
    plt.cla()
    plt.draw()
    plt.pause(0.001)
    
#monte carlo raytrace 1/r for initial condition?