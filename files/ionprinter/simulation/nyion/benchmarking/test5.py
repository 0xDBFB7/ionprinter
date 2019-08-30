
import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math

SIZE_X = 128

u = numpy.zeros(SIZE_X)
b = numpy.zeros(SIZE_X)

for x in range(40,50):
    b[x] = 1.0

# b[8,8] = 1

# def gauss_seidel(U,b,theta):
#     rows = U.shape[0]
#     T = numpy.zeros_like(U)
#     for x in range(theta, (rows - theta),theta):
#         T[x] = (U[x+theta] +
#                     U[x-theta] + b[x])/2.0
#     numpy.copyto(U,T)
#     return T

def residual(u, f):
    """
    f - A u
    """
    n = len(f)
    r = numpy.zeros(len(u))
    r[1:-1] = f[1:-1] - ((n+1)**2) * (2 * u[1:-1] - u[2:] - u[:-2])
    r[0]    = f[0]    - ((n+1)**2) * (2 * u[0]    - u[1])
    r[-1]   = f[-1]   - ((n+1)**2) * (2 * u[-1]   - u[-2])
    return r

# def residual(phi,f):
#     r = numpy.zeros_like(phi)
#     rows = phi.shape[0]
#
#     for x in range(1, (rows - 1),1): #(((rows**2.0) / 1.0)*
#         r[x] = (phi[x+1] + phi[x-1] - 2.0*phi[x])/2.0
#     return r

# def construct_T(U):
#     rows = U.shape[0]
#     T = numpy.zeros((rows,rows))
#
#     for x in range(1, (rows - 1)):
#         T[x,x-1] = 1
#         T[x,x] = -2
#         T[x,x+1] = 1
#
#
#
#     return T

#
def restriction(X):
    rows = X.shape[0]

    O = numpy.zeros((int(rows/2)))

    for x in range(0,int(rows/2)-1):
        O[x] = X[x*2]

    return O

def prolongate(X):
    rows = X.shape[0]

    O = numpy.zeros((rows*2))
    for x in range(1,rows-1):
        O[2*x] = X[x]
        O[2*x+1] = (X[x]+X[x+1])/2.0

    return O

# it's not the root residual you idiot
# it's each residual recursively

def V_Cycle(u,f,h):
    phi = u.copy()

    gauss_seidel(phi,f,1)

    r = residual(phi,f)

    rhs = restriction(r)
    #
    eps = numpy.zeros_like(rhs)
    #
    if(h == 32):
        for i in range(0,10):
            gauss_seidel(eps,rhs,1) #this is the error or correction equation.
    else:
        eps = V_Cycle(eps,rhs,2*h)

    T = prolongate(eps)

    if(h==1):
        print("Residual: {} Step: {}".format(numpy.linalg.norm(r)/(len(phi)**2),h))

        plt.subplot(2, 3, 1)
        plt.gca().set_title('Potentials')
        plt.plot(phi)

        plt.subplot(2, 3, 2)
        plt.gca().set_title('Residuals')
        plt.plot(r)

        plt.subplot(2, 3, 3)
        plt.gca().set_title('Restricted')
        plt.plot(rhs)

        plt.subplot(2, 3, 4)
        plt.gca().set_title('Correction')
        plt.plot(eps)

        plt.subplot(2, 3, 5)
        plt.gca().set_title('Prolongated')
        plt.plot(T)

        plt.draw()
        plt.pause(0.1)
        plt.clf()
        plt.cla()

    return phi + T

convergence = []
ims = []
t = 0
c1 = 1

v1 = u.copy()

r1 = u-v1

while True:
# for i in range(0,5):
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

    # u = V_Cycle(u,b,1)

    # gauss_seidel(u,b,1)
    # r = residual(u,b)
    # r = restriction(r)
    #
    # v = numpy.zeros_like(r)
    # gauss_seidel(v,r,1)
    # print(numpy.linalg.norm(r)/(len(u)**2))
    # print(numpy.linalg.norm(v))

    T = numpy.zeros_like(u)
    for x in range(1, (u.shape[0] - 2)):
        T[x] = (u[x+1] + u[x-1] + b[x])/2.0

    u = T.copy()
    plt.subplot(2, 3, 1)
    plt.gca().set_title('Potentials')
    plt.plot(u)

    # plt.subplot(2, 3, 2)
    # plt.gca().set_title('Correction')
    # plt.plot(r)
    #
    # plt.subplot(2, 3, 3)
    # plt.gca().set_title('Prolongated')
    # plt.plot(v)

    plt.draw()
    plt.pause(0.1)
    plt.clf()
    plt.cla()

    # u += prolongate(v)

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
