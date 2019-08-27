import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math

SIZE_X = 64
SIZE_Y = 64
SIZE_Z = 64


u = numpy.zeros((SIZE_X, SIZE_Y, SIZE_Z))
b = numpy.zeros((SIZE_X, SIZE_Y, SIZE_Z))

for x in range(40,50):
    for y in range(40,50):
        b[x,y,8] = 1

def gauss_seidel(U,b,theta):
    rows = U.shape[0]
    cols = U.shape[1]

    for x in range(theta, (rows - theta)-1,theta):
        for y in range(theta, (cols - theta)-1,theta):
            for z in range(theta, (cols - theta)-1,theta):
                U[x,y,z] = (U[x+theta,y,z] +
                            U[x-theta,y,z] +
                            U[x,y+theta,z] +
                            U[x,y-theta,z] +
                            U[x,y,z+theta] +
                            U[x,y,z-theta] + b[x,y,z])/6.0

def restriction(X, theta):
    rows = X.shape[0]
    cols = X.shape[1]

    for x in range(theta, (rows - theta)-1,theta):
        for y in range(theta, (cols - theta)-1,theta):
            for z in range(theta, (cols - theta)-1,theta):
                sum = 0
                for i in range(0,theta):
                    for j in range(0,theta):
                        for k in range(0,theta):
                            sum += X[x+i,y+j,z+k]
                X[x,y,z] = sum


def prolongate(X, theta):
    rows = X.shape[0]
    cols = X.shape[1]

    for x in range(theta, (rows - theta)-1,theta):
        for y in range(theta, (cols - theta)-1,theta):
            for z in range(theta, (cols - theta)-1,theta):
                V000 = X[x,y,z]
                V001 = X[x,y,z+theta]
                V010 = X[x,y+theta,z]
                V100 = X[x+theta,y,z]
                V101 = X[x+theta,y,z+theta]
                V110 = X[x+theta,y+theta,z]
                V111 = X[x+theta,y+theta,z+theta]
                for i in range(0,theta):
                    for j in range(0,theta):
                        for k in range(0,theta):
                            f_x = float(i)/theta
                            f_y = float(j)/theta
                            f_z = float(k)/theta

                            X[x+i,y+j,z+k] = 0
                            X[x+i,y+j,z+k] += V000*(1.0-f_x)*(1.0-f_y)*(1.0-f_z)
                            X[x+i,y+j,z+k] += V001*(1.0-f_x)*(1.0-f_y)*(f_z)
                            X[x+i,y+j,z+k] += V010*(1.0-f_x)*(f_y)*(1.0-f_z)
                            X[x+i,y+j,z+k] += V100*(f_x)*(1.0-f_y)*(1.0-f_z)
                            X[x+i,y+j,z+k] += V101*(f_x)*(1.0-f_y)*(f_z)
                            X[x+i,y+j,z+k] += V110*(f_x)*(f_y)*(1.0-f_z)
                            X[x+i,y+j,z+k] += V111*(f_x)*(f_y)*(f_z)


# Precondition.
# for i in range(0,10):
#     gauss_seidel(u,b,1)

convergence = []
ims = []
t = 0
c1 = 1
while True:

    # Step 1: Residual Calculation.
    v1 = u.copy()
    gauss_seidel(u,b,1)
    print(numpy.linalg.norm(b))

    r = u - v1
    # Step 2: Restriction.
    res = [32,16,8,16,8,4,8,4,2,4,2,1]
    v = numpy.zeros((SIZE_X, SIZE_Y, SIZE_Z))
    for level in range(0,len(res),1):
        resolution = res[level]
        r1 = r.copy()
        if(level != 0):
            restriction(r1,resolution)
        for i in range(0,2*int(math.sqrt(res[level]))):
            gauss_seidel(v,r1,resolution)
        if(level != 0):
            prolongate(v,resolution)

    u = u + v

    convergence.append(numpy.linalg.norm(r))

    plt.subplot(2, 2, 1)
    plt.gca().set_title('Potentials')
    plt.imshow(u[:,:,8])
    plt.subplot(2, 2, 2)
    plt.gca().set_title('Residual')
    plt.imshow(r[:,:,8])
    plt.subplot(2, 2, 3)
    plt.gca().set_title('Correction')
    plt.imshow(v[:,:,8])
    plt.subplot(2, 2, 4)
    plt.yscale('log')
    plt.gca().set_title('Convergence')
    plt.plot(convergence)
    plt.savefig(str(t) + '.png')
    t+=1
    print("Residual: {} convergence factor: {} Step: {}".format(numpy.linalg.norm(r),numpy.linalg.norm(r)/c1,t))
    c1 = numpy.linalg.norm(r)
    #
    # plt.draw()
    # plt.pause(0.001)
