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

convergence = 0
while(math.fabs(convergence-potentials[10,40])):
    convergence = potentials[10,40]
    for x in range(1,mesh_x-1):
        for y in range(1,mesh_x-1):
            if(not BC[x,y]):
                potentials[x,y] = potentials[x-1,y] + \
                                    potentials[x+1,y] + \
                                    potentials[x,y-1] + \
                                    potentials[x,y+1]
                potentials[x,y] /= 4.0
    print(potentials[10,40], potentials[10,40]-convergence)

plt.imshow(potentials)
plt.show()
