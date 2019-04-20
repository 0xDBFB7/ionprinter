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
sum = 0
previous_potentials = 0
while(math.fabs(sum-previous_potentials) > 0.01 or abs(previous_potentials) < 0.02):
    previous_potentials = sum
    sum = 0
    for x in range(1,mesh_x-1):
        for y in range(1,mesh_x-1):
            if(not BC[x,y]):
                potentials[x,y] = potentials[x-1,y] + \
                                    potentials[x+1,y] + \
                                    potentials[x,y-1] + \
                                    potentials[x,y+1]
                potentials[x,y] /= 4.0
                sum += potentials[x,y]
    sum = sum/(mesh_x*mesh_y)
    print(sum)
plt.imshow(potentials)
plt.show()
