import numpy
import matplotlib.pyplot as plt
import math
mesh_x = 50
mesh_y = 50

potentials = numpy.zeros(shape=(mesh_x,mesh_y))
BC = numpy.zeros(shape=(mesh_x,mesh_y))

potentials[10,10] = 100.0
potentials[20,10] = -100.0

BC[10,10] = 1
BC[20,10] = 1

convergence = 1
previous_convergence = 1
count = 0
while(convergence > 0.001):
    sum = 0
    R = potentials.copy()
    for x in range(1,mesh_x-1):
        for y in range(1,mesh_x-1):
            if(not BC[x,y]):
                potentials[x,y] = potentials[x-1,y] + \
                                    potentials[x+1,y] + \
                                    potentials[x,y-1] + \
                                    potentials[x,y+1]
                potentials[x,y] /= 4.0
    R = R - potentials
    convergence = (numpy.linalg.norm(R))
    print(convergence)
    print(count)
    count+=1
BC[11,11] = 1
BC[21,11] = 1
potentials[11,11] = 100.0
potentials[21,11] = -100.0

convergence = 1
previous_convergence = 1

count = 0
while(convergence > 0.001):
    sum = 0
    R = potentials.copy()
    for x in range(1,mesh_x-1):
        for y in range(1,mesh_x-1):
            if(not BC[x,y]):
                potentials[x,y] = potentials[x-1,y] + \
                                    potentials[x+1,y] + \
                                    potentials[x,y-1] + \
                                    potentials[x,y+1]
                potentials[x,y] /= 4.0
    R = R - potentials
    convergence = (numpy.linalg.norm(R))
    print(convergence)
    print(count)
    count+=1
plt.imshow(potentials)
plt.show()
