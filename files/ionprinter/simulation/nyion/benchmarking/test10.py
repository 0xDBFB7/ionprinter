import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
SIZE_X = 256
SIZE_Y = 256

u = numpy.zeros((SIZE_X, SIZE_Y))
b = numpy.zeros((SIZE_X, SIZE_Y))

for x in range(120,128+8):
    for y in range(120,128+8):
        u[x,y] = 1.0
        b[x,y] = 1.0

def jacobi(U,b,f,theta):
    rows = U.shape[0]
    cols = U.shape[1]
    T = U.copy()

    dx=1.0/(rows//theta)
    dy=1.0/(cols//theta)

    Ax=1.0/dx**2
    Ay=1.0/dy**2
    Ap=1.0/(2.0*(1.0/dx**2+1.0/dy**2))

    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            if(b[x,y] == 0):
                T[x,y] = Ap*(Ax*(U[x+theta,y] + U[x-theta,y]) +
                          Ay*(U[x,y+theta] + U[x,y-theta]) + f[x,y])

    return T

#
# def gauss_seidel(U,b,f,theta):
#     rows = U.shape[0]
#     cols = U.shape[1]
#     h2 = 1.0/((1/rows)*(1/cols))
#
#     for x in range(theta, (rows - theta)-1,theta):
#         for y in range(theta, (cols - theta)-1,theta):
#             if(b[x,y] == 0):
#                 U[x,y] = (U[x+theta,y] +
#                             U[x-theta,y] +
#                             U[x,y+theta] +
#                             U[x,y-theta] + h2*f[x,y])/4.0

def residual(U,b,f,theta):
    rows = U.shape[0]
    cols = U.shape[1]

    dx=1.0/(rows//theta)
    dy=1.0/(cols//theta)

    Ax=1.0/dx**2
    Ay=1.0/dy**2
    Ap=1.0/(2.0*(1.0/dx**2+1.0/dy**2))

    R = numpy.zeros_like(U)
    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            if(b[x,y] == 0):
                R[x,y] = f[x,y] - (Ax*(U[x+theta,y] + U[x-theta,y]) +

                                    Ay*(U[x,y+theta] + U[x,y-theta])
                                     - 2.0*(Ax+Ay)*U[x,y])

    return R

def restriction(B,X, theta):
    rows = X.shape[0]
    cols = X.shape[1]
    O = X.copy()
    for x in range(theta, (rows - theta)-1,theta):
        for y in range(theta, (cols - theta)-1,theta):
            # sum = 0
            # for i in range(0,theta):
            #     for j in range(0,theta):
            #         sum += X[x+i,y+j]
            if(not B[x,y]):
                O[x,y] = X[x,y]*0.5
                O[x,y] += X[x+theta,y]*0.125
                O[x,y] += X[x-theta,y]*0.125
                O[x,y] += X[x,y+theta]*0.125
                O[x,y] += X[x,y-theta]*0.125

    numpy.copyto(X,O)


def prolongate(B, X, theta):
    rows = X.shape[0]
    cols = X.shape[1]

    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            V00 = X[x,y]
            V01 = X[x,y+theta]
            V10 = X[x+theta,y]
            V11 = X[x+theta,y+theta]
            for i in range(0,theta,theta//2):
                for j in range(0,theta,theta//2):
                    if(not B[x+i,y+j]):
                        f_x = float(i)/theta
                        f_y = float(j)/theta
                        X[x+i,y+j] = 0
                        X[x+i,y+j] += V00*(1.0-f_x)*(1.0-f_y)
                        X[x+i,y+j] += V01*(1.0-f_x)*(f_y)
                        X[x+i,y+j] += V10*(f_x)*(1.0-f_y)
                        X[x+i,y+j] += V11*(f_x)*(f_y)

# Precondition.
# for i in range(0,10):
#     gauss_seidel(u,b,1)

convergence = []
ims = []
t = 0
c1 = 1

def V_Cycle(U,B,f,h):

    U=jacobi(U,B,f,h)
    U=jacobi(U,B,f,h)
    r = residual(U,B,f,h)
    # # # Step 2: Restriction.
    v = numpy.zeros((SIZE_X, SIZE_Y))
    restriction(b,r,2*h)
    if(h == 4):
        for i in range(0,10):
            v=jacobi(v,b,r,h*2)
            v=jacobi(v,b,r,h*2)
    else:
        v = V_Cycle(v,B,r,h*2)

    prolongate(B,v,h*2)

    return U + v

while True:

    #
    # # # Step 1: Residual Calculation.
    # # v1 = u.copy()
    # f = numpy.zeros((SIZE_X, SIZE_Y))
    # u=jacobi(u,b,f,1)
    # u=jacobi(u,b,f,1)
    # r = -residual(u,b,f,1)
    # # # # Step 2: Restriction.
    # # res = [32,16,8,4,2,1]
    # v = numpy.zeros((SIZE_X, SIZE_Y))
    # restriction(b,r,2)
    # v=jacobi(v,b,r,2)
    # v=jacobi(v,b,r,2)
    # r1= residual(v,b,r,2)
    # #
    # # v1 = numpy.zeros((SIZE_X, SIZE_Y))
    # # restriction(b,r1,4)
    # # v1=jacobi(v1,b,r1,4)
    # # v1=jacobi(v1,b,r1,4)
    # # r2= residual(v1,b,r1,4)
    # #
    # # v2 = numpy.zeros((SIZE_X, SIZE_Y))
    # # restriction(b,r2,16)
    # # v2=jacobi(v2,b,r2,16)
    # # v2=jacobi(v2,b,r2,16)
    # #
    # # prolongate(b,v2,16)
    # # v1 += v2
    # #
    # # prolongate(b,v1,4)
    # # v += v1
    #
    # prolongate(b,v,2)
    # u += v
    f = b.copy()
    u = V_Cycle(u,b,b,1)

    r = residual(u,b,f,1)
    # # # # b1 = numpy.zeros((SIZE_X, SIZE_Y))
    # for level in range(0,len(res),1):
    #     resolution = res[level]
    #     if(level != len(res)-1):
    #         restriction(b,u,resolution)
    #     for i in range(0,1):
    #         u=jacobi(u,b,v,resolution)
    #
    #     if(level != len(res)-1):
    #         prolongate(b,u,resolution)




    convergence.append(numpy.linalg.norm(r))

    plt.subplot(2, 3, 1)
    plt.gca().set_title('Potentials')
    plt.plot(u[128,:])


    plt.subplot(2, 3, 2)
    plt.gca().set_title('Residual')
    plt.plot(r[128,:])
    plt.subplot(2, 3, 3)
    # plt.gca().set_title('Correction')
    # plt.plot(v[128,:])
    # plt.subplot(2, 3, 4)

    plt.yscale('log')
    plt.gca().set_title('Convergence')
    plt.plot(convergence)

    # plt.subplot(2, 3, 5)
    # plt.gca().set_title('Residual')
    # plt.plot(r1[128,:])
    # # plt.savefig(str(t) + '.png')
    t+=1
    print("Residual: {} convergence factor: {} Step: {}".format(numpy.linalg.norm(r),numpy.linalg.norm(r)/c1,t))
    c1 = numpy.linalg.norm(r)
    #
    plt.draw()
    plt.pause(0.001)
