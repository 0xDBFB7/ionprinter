import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math

SIZE_X = 32
SIZE_Y = 32


u = numpy.zeros((SIZE_X, SIZE_Y))
b = numpy.zeros((SIZE_X, SIZE_Y)) #boundaries, not solution

for x in range(10,15):
    for y in range(10,15):
        u[x,y] = 1
        b[x,y] = 1

def jacobi_with_boundaries(U,b,f,theta):
    rows = U.shape[0]
    cols = U.shape[1]

    T = U.copy()
    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            if(b[x,y] == 0):
                T[x,y] = (U[x+theta,y] +
                            U[x-theta,y] +
                            U[x,y+theta] +
                            U[x,y-theta] + f[x,y])/4.0
    numpy.copyto(U,T)


def jacobi(U,f,theta):
    rows = U.shape[0]
    cols = U.shape[1]

    T = U.copy()
    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            T[x,y] = (U[x+theta,y] +
                        U[x-theta,y] +
                        U[x,y+theta] +
                        U[x,y-theta] + f[x,y])/4.0
    numpy.copyto(U,T)

def restriction(X, theta):
    rows = X.shape[0]
    cols = X.shape[1]

    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            sum = 0
            for i in range(0,theta):
                for j in range(0,theta):
                    sum+=X[x+i,y+j]
            X[x,y] = sum

def prolongation(X, theta):
    rows = X.shape[0]
    cols = X.shape[1]

    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            V00 = X[x,y]
            V01 = X[x,y+theta]
            V10 = X[x+theta,y]
            V11 = X[x+theta,y+theta]
            for i in range(0,theta,theta):
                for j in range(0,theta,theta):
                    f_x = float(i)/(theta)
                    f_y = float(j)/(theta)
                    X[x+i,y+j] = 0
                    X[x+i,y+j] += V00*(1.0-f_x)*(1.0-f_y)
                    X[x+i,y+j] += V01*(f_x)*(1.0-f_y)
                    X[x+i,y+j] += V10*(1.0-f_x)*(f_y)
                    X[x+i,y+j] += V11*(f_x)*(f_y)



t = 0
c1 = 1

while True:
    #Step 1


    r2 = u.copy()
    f = numpy.zeros_like(u) #no space charge for now
    jacobi_with_boundaries(u,b,f,1)
    r = u - r2

    print("Residual: {} Convergence factor: {} Step: {}".format(numpy.linalg.norm(r),numpy.linalg.norm(r)/c1,t))
    c1 = numpy.linalg.norm(r)
    t+=1
    #
    # #Step 2
    restriction(r,2)
    v = numpy.zeros((SIZE_X, SIZE_Y)) #correction
    jacobi(v,r,2)
    prolongation(v,2)

    u += v



    # for x in range(1, SIZE_X-1,1):
    #     for y in range(1, SIZE_Y-1,1):
    #         r[x,y] = u[x,y] - v1[x,y]
# convergence.append(numpy.linalg.norm(r))
#
# plt.subplot(2, 2, 1)
# plt.gca().set_title('Potentials')
# plt.imshow(u[:,:,8])
# plt.subplot(2, 2, 2)
# plt.gca().set_title('Residual')
# plt.imshow(r[:,:,8])
# plt.subplot(2, 2, 3)
# plt.gca().set_title('Correction')
# plt.imshow(v[:,:,8])
# plt.subplot(2, 2, 4)
# plt.yscale('log')
# plt.gca().set_title('Convergence')
# plt.plot(convergence)
# # plt.savefig(str(t) + '.png')
