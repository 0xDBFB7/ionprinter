import numpy
import matplotlib.pyplot as plt

SIZE_X = 64
SIZE_Y = 64

potentials = numpy.zeros((SIZE_X, SIZE_Y))
boundaries = numpy.zeros((SIZE_X, SIZE_Y))

potentials[5,5] = 10;
boundaries[5,5] = 1;

def gauss_seidel(potentials,boundaries):
    rows = potentials.shape[0]
    cols = boundaries.shape[1]

    for x in range(1, cols - 2):
        for y in range(1, rows - 2):
            if(not boundaries[x,y]):
                potentials[x,y] = (potentials[x+1,y] +
                                    potentials[x-1,y] +
                                    potentials[x,y+1] +
                                    potentials[x,y-1])/4.0



for i in range(0,100):
    gauss_seidel(potentials,boundaries)

potentials_copy = potentials.copy()
gauss_seidel(potentials,boundaries)
residuals = potentials - potentials_copy



print(numpy.linalg.norm(residuals))
plt.subplot(2, 1, 1)
plt.imshow(potentials)
plt.subplot(2, 1, 2)

plt.imshow(residuals)
plt.show()
