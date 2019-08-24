import numpy
import matplotlib.pyplot as plt

SIZE_X = 64
SIZE_Y = 64

u = numpy.zeros((SIZE_X, SIZE_Y))
b = numpy.zeros((SIZE_X, SIZE_Y))

u[5,5] = 10
b[5,5] = 1

def gauss_seidel(U,b,theta):
    rows = U.shape[0]
    cols = b.shape[1]

    for x in range(1, cols - 2):
        for y in range(1, rows - 2):
            U[x,y] = (U[x+1,y] +
                        U[x-1,y] +
                        U[x,y+1] +
                        U[x,y-1] + b[x,y])/4.0

# def prolongate(potentials, theta):
#     rows = potentials.shape[0]
#     cols = boundaries.shape[1]
#     for x in range(1, cols - 2,theta):
#         for y in range(theta, rows - 2*theta,theta):

# Precondition.
for i in range(0,10):
    gauss_seidel(u,b)

# Step 1: Residual Calculation.
v1 = u.copy()
gauss_seidel(u,b)
r = u - v1
# Step 2: Restriction.
v = numpy.zeros((SIZE_X, SIZE_Y))
for i in range(0,10):
    gauss_seidel(v,r)



print(numpy.linalg.norm(r))
plt.subplot(2, 1, 1)
plt.imshow(u)
plt.subplot(2, 1, 2)

plt.imshow(r)
plt.show()
