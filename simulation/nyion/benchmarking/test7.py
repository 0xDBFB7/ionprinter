import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
SIZE_X = 128

u = numpy.zeros(SIZE_X)
b = numpy.zeros(SIZE_X)

for x in range(40,50):
        u[x] = 1
        b[x] = 1

# b[8,8] = 1

def gauss_seidel(U,b,theta):
    rows = U.shape[0]
    cols = U.shape[1]

    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            U[x,y] = (U[x+theta,y] +
                        U[x-theta,y] +
                        U[x,y+theta] +
                        U[x,y-theta] + b[x,y])/4.0

def gauss_seidel(U,b,theta):
    rows = U.shape[0]
    cols = U.shape[1]

    for x in range(theta, (rows - theta),theta):
        for y in range(theta, (cols - theta),theta):
            U[x,y] = (U[x+theta,y] +
                        U[x-theta,y] +
                        U[x,y+theta] +
                        U[x,y-theta] + b[x,y])/4.0

def restriction(X):
    rows = X.shape[0]
    cols = X.shape[1]

    O = numpy.zeros((int(rows/2), int(cols/2)))

    for x in range(0,int(rows/2)-1):
        for y in range(0,int(cols/2)-1):
            O[x,y] = X[x*2,y*2]
    return O

def prolongate(X):
    rows = X.shape[0]
    cols = X.shape[1]

    O = numpy.zeros((rows*2, cols*2))
    for x in range(0,rows-1):
        for y in range(0,cols-1):
            O[2*x,2*y] = X[x,y]
            O[2*x+1,2*y] = 0.5*(X[x,y]+X[x+1,y])
            O[2*x,2*y+1] = 0.5*(X[x,y]+X[x,y+1])
            O[2*x+1,2*y+1] = 0.25*(X[x,y]+X[x,y+1]+X[x+1,y]+X[x+1,y+1])

            # for i in range(0,2):
            #     for j in range(0,2):


    return O

# it's not the root residual you idiot
# it's each residual recursively

# def V_Cycle(u,f,h):
#     phi = u.copy()
#
#     v1 = phi.copy()
#     gauss_seidel(phi,f,1)
#     r = phi - v1
#
#     rhs = restriction(r)
#     #
#     eps = numpy.zeros_like(rhs)
#     #
#     if(h == 32):
#         for i in range(0,10):
#             gauss_seidel(eps,rhs,1)
#     else:
#         eps = V_Cycle(eps,rhs,2*h)
#
#     T = prolongate(eps)
#
#     if(h==1):
#         print("Residual: {} Step: {}".format(numpy.linalg.norm(r),h))

    # plt.subplot(2, 3, 1)
    # plt.gca().set_title('Potentials')
    # plt.imshow(phi)
    #
    # plt.subplot(2, 3, 2)
    # plt.gca().set_title('Residuals')
    # plt.imshow(r)
    #
    # plt.subplot(2, 3, 3)
    # plt.gca().set_title('Restricted')
    # plt.imshow(rhs)
    #
    # plt.subplot(2, 3, 4)
    # plt.gca().set_title('Correction')
    # plt.imshow(eps)
    #
    # plt.subplot(2, 3, 5)
    # plt.gca().set_title('Prolongated')
    # plt.imshow(T)
    #
    # plt.draw()
    # plt.pause(0.1)
    # plt.clf()
    # plt.cla()

    # return phi + T

convergence = []
ims = []
t = 0
c1 = 1

while True:

    # u = restriction(b)
    # u = restriction(u)
    #
    # plt.subplot(2, 2, 1)
    # plt.gca().set_title('Potentials')
    # plt.imshow(b)
    #
    # plt.subplot(2, 2, 2)
    # plt.gca().set_title('Potentials')
    # plt.imshow(u)
    #
    # T = prolongate(u)
    #
    # plt.subplot(2, 2, 3)
    # plt.gca().set_title('Potentials')
    # plt.imshow(T)

    u = V_Cycle(u,b,1)

    #
    #
    # convergence.append(numpy.linalg.norm(r))
    #

    #
    # u = u + v
    #
    #
    # plt.subplot(2, 2, 2)
    # plt.gca().set_title('Residual')
    # plt.imshow(r)
    # plt.subplot(2, 2, 3)
    # plt.gca().set_title('Correction')
    # plt.imshow(v)
    # plt.subplot(2, 2, 4)
    # plt.yscale('log')
    # plt.gca().set_title('Convergence')
    # plt.plot(convergence)
    # # plt.savefig(str(t) + '.png')
    # t+=1
    # print("Residual: {} convergence factor: {} Step: {}".format(numpy.linalg.norm(r),numpy.linalg.norm(r)/c1,t))
    # c1 = numpy.linalg.norm(r)
    #

    # plt.close()
