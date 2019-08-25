import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation

SIZE_X = 128
SIZE_Y = 128

u = numpy.zeros((SIZE_X, SIZE_Y))
b = numpy.zeros((SIZE_X, SIZE_Y))

for x in range(40,50):
    for y in range(40,50):
        b[x,y] = 1

def gauss_seidel(U,b,theta):
    rows = U.shape[0]
    cols = U.shape[1]

    for x in range(theta, (rows - theta)-1,theta):
        for y in range(theta, (cols - theta)-1,theta):
            U[x,y] = (U[x+theta,y] +
                        U[x-theta,y] +
                        U[x,y+theta] +
                        U[x,y-theta] + b[x,y])/4.0

def restriction(X, theta):
    rows = X.shape[0]
    cols = X.shape[1]

    for x in range(theta, (rows - theta)-1,theta):
        for y in range(theta, (cols - theta)-1,theta):
            sum = 0
            for i in range(0,theta):
                for j in range(0,theta):
                    sum += X[x+i,y+j]
            X[x,y] = sum


def prolongate(X, theta):
    rows = X.shape[0]
    cols = X.shape[1]

    for x in range(theta, (rows - theta)-1,theta):
        for y in range(theta, (cols - theta)-1,theta):
            V00 = X[x,y]
            V01 = X[x,y+theta]
            V10 = X[x+theta,y]
            V11 = X[x+theta,y+theta]
            for i in range(0,theta):
                for j in range(0,theta):
                    f_x = float(i)/theta
                    f_y = float(j)/theta
                    X[x+i,y+j] = 0
                    X[x+i,y+j] += V00*(1.0-f_x)*(1.0-f_y)
                    X[x+i,y+j] += V01*(f_x)*(1.0-f_y)
                    X[x+i,y+j] += V10*(1.0-f_x)*(f_y)
                    X[x+i,y+j] += V11*(f_x)*(f_y)

# Precondition.
for i in range(0,10):
    gauss_seidel(u,b,1)

convergence = []
ims = []
t = 0
while True:

    # Step 1: Residual Calculation.
    v1 = u.copy()
    gauss_seidel(u,b,1)
    r = u - v1
    # Step 2: Restriction.
    v = numpy.zeros((SIZE_X, SIZE_Y))
    for level in range(4,-1,-1):
        resolution = 2**level
        r1 = r.copy()
        restriction(r1,resolution)
        for i in range(0,2**level):
            gauss_seidel(v,r1,resolution)
            gauss_seidel(v,r1,resolution)

        prolongate(v,resolution)

    convergence.append(numpy.linalg.norm(r))
    print(numpy.linalg.norm(r))

    plt.subplot(2, 2, 1)
    plt.gca().set_title('Potentials')
    plt.imshow(u)

    u = u + v

    plt.subplot(2, 2, 2)
    plt.gca().set_title('Residual')
    plt.imshow(r)
    plt.subplot(2, 2, 3)
    plt.gca().set_title('Correction')
    plt.imshow(v)
    plt.subplot(2, 2, 4)
    plt.yscale('log')
    plt.gca().set_title('Convergence')
    plt.plot(convergence)
    plt.savefig(str(t) + '.png')
    t+=1
    # plt.draw()
    # plt.pause(0.001)
