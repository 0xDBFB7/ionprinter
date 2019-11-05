import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math

SIZE_X = 32
SIZE_Y = 32
SIZE_Z = 32


u = numpy.zeros((SIZE_X, SIZE_Y, SIZE_Z))
b = numpy.zeros((SIZE_X, SIZE_Y, SIZE_Z))
f = numpy.zeros((SIZE_X, SIZE_Y, SIZE_Z))

for x in range(10,20):
    for y in range(10,20):
        u[x,y,8] = 100
        b[x,y,8] = 1

def gauss_seidel(U,B,F,theta):
    rows = U.shape[0]
    cols = U.shape[1]
    z_size = U.shape[2]

    for x in range(theta, (rows - theta)-1,theta):
        for y in range(theta, (cols - theta)-1,theta):
            for z in range(theta, (z_size - theta)-1,theta):
                if(not B[x,y,z]):
                    U[x,y,z] = (U[x+theta,y,z] +
                                U[x-theta,y,z] +
                                U[x,y+theta,z] +
                                U[x,y-theta,z] +
                                U[x,y,z+theta] +
                                U[x,y,z-theta] + F[x,y,z])/6.0



def jacobi(U,T,B,F,theta):
    rows = U.shape[0]
    cols = U.shape[1]
    z_size = U.shape[2]

    for x in range(theta, (rows - theta)-1,theta):
        for y in range(theta, (cols - theta)-1,theta):
            for z in range(theta, (z_size - theta)-1,theta):
                if(not B[x,y,z]):
                    T[x,y,z] = (U[x+theta,y,z] +
                                U[x-theta,y,z] +
                                U[x,y+theta,z] +
                                U[x,y-theta,z] +
                                U[x,y,z+theta] +
                                U[x,y,z-theta] +
                                U[x,y-theta,z-theta] +
                                U[x,y+theta,z+theta] +  F[x,y,z])/8.0


def restriction(X, theta):
    rows = X.shape[0]
    cols = X.shape[1]
    z_size = X.shape[2]

    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            for z in range(theta, (z_size - theta),theta):
                sum = 0
                for i in range(0,theta):
                    for j in range(0,theta):
                        for k in range(0,theta):
                            sum += X[x+i,y+j,z+k]
                if(not b[x,y,z]):
                    X[x,y,z] = sum


# def residual(U,R,F):
#     rows = U.shape[0]
#     cols = U.shape[1]
#     z_size = U.shape[2]
#     theta = 1
#     for x in range(theta, (rows - theta),theta):
#         for y in range(theta, (cols - theta),theta):
#             for z in range(theta, (z_size - theta),theta):
#                 if(not b[x,y,z]):
#                     R[x,y,z] = F[x,y,z] + (U[x+1,y,z] + U[x-1,y,z] + U[x,y+1,z] + U[x,y-1,z] + U[x,y,z+1] + U[x,y,z-1] - 6.0*U[x,y,z])

def prolongate(X, theta):
    rows = X.shape[0]
    cols = X.shape[1]
    z_size = X.shape[2]

    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            for z in range(theta, (z_size - theta),theta):
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
                            if(not b[x+i,y+j,z+k]):
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
pos = 15
while True:
    for i in range(0,10):
        T1 = u.copy()
        jacobi(u,T1,b,f,1)
        u = T1.copy()
    # Step 1: Residual Calculation.
    v1 = u.copy()
    T1 = u.copy()
    jacobi(u,T1,b,f,1)
    u = T1.copy()
    # u=T.copy()
    r = u - v1
    # r = numpy.zeros((SIZE_X, SIZE_Y, SIZE_Z))
    # residual(u,r,f)
    # Step 2: Restriction.
    res = [32,16,8,4,2,1]
    v=0
    T=0
    T = numpy.zeros((SIZE_X, SIZE_Y, SIZE_Z))
    v = numpy.zeros((SIZE_X, SIZE_Y, SIZE_Z))
    for level in range(0,len(res),1):
        resolution = res[level]
        print(resolution)
        r1 = r.copy()
        # plt.subplot(4, 1, 1)
        # plt.gca().set_title('Original')
        # plt.imshow(r1[:,:,8])
        if(res != 1):
            restriction(r1,2*resolution)
        # plt.subplot(4, 1, 2)
        # plt.gca().set_title('Restricted')
        # plt.imshow(r1[:,:,8])
        for i in range(0,3*int(math.sqrt(res[level]))):
            jacobi(v,T,numpy.zeros_like(v),r1,2*resolution)
            v=T.copy()
        # plt.subplot(4, 1, 3)
        # plt.gca().set_title('Smoothed')
        # plt.imshow(v[:,:,8])
        if(res != 1):
            prolongate(v,2*resolution)

        # plt.subplot(4, 1, 4)
        # plt.gca().set_title('Prolongated')
        # plt.imshow(v[:,:,8])


        # plt.draw()
        # plt.pause(0.001)
        # plt.savefig(str(t) + '.png')

    #
    u = u + (v * (1.0-b))
    convergence.append(numpy.linalg.norm(r))
    #
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
    t+=1
    print("Residual: {} convergence factor: {} Step: {}".format(numpy.linalg.norm(r),numpy.linalg.norm(r)/c1,t))
    c1 = numpy.linalg.norm(r)

    if(numpy.linalg.norm(r) < 1e-3):
        f[25,pos,8] = 10
        pos+=1
    #
    plt.cla()
    plt.draw()
    plt.pause(0.001)
