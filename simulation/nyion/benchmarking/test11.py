import numpy
import matplotlib.pyplot as plt

def jacobi(U,B,F):
    h=1.0/(U.shape[0]-1);
    h2=h*h;
    T = U.copy()
    for i in range(1,U.shape[0]-1):
        # if(B[i] == 0):
        T[i] = 0.5*(U[i+1] + U[i-1]) - F[i]

    return T

def residual(U,B,F):
    h=1.0/(U.shape[0]-1);
    h2=1.0/(h*h);
    R = numpy.zeros_like(U)
    for i in range(1,U.shape[0]-1):
        if(B[i] == 0):
            R[i] = F[i]*h2 - (U[i+1] + U[i-1] - 2.0*U[i])



    return R

def prolongate(X):
    rows = X.shape[0]

    O = numpy.zeros((rows*2))
    for x in range(1,rows-2):
        O[2*x-1] = (X[x]+X[x-1])/2.0
        O[2*x] = X[x]
        O[2*x+1] = (X[x]+X[x+1])/2.0

    return O

def V_Cycle(U,B,F,h):
    for i in range(0,10):
        U = jacobi(U,B,F)
        U = (1.0-B)*U
        U += B

    r = residual(U,B,F)

    print(numpy.linalg.norm(r),h)

    r_restricted = r.copy()[1::2]
    # r1 = restriction(r)
    v = numpy.zeros_like(r_restricted)
    b1 = B.copy()[1::2]
    # b1 = restriction(B)
    if(h == 64):
        for i in range(0,100):
            v = jacobi(v,b1,r_restricted)
    else:
        v = V_Cycle(v,b1,r_restricted,2*h)

    v1 = prolongate(v)

    # plt.figure()
    # plt.subplot(2, 3, 1)
    # plt.gca().set_title('Potentials')
    # plt.plot(u)
    # plt.subplot(2, 3, 2)
    # plt.gca().set_title('Residual')
    # plt.plot(r)
    # plt.subplot(2, 3, 3)
    # plt.gca().set_title('Restricted residual')
    # plt.plot(r_restricted)
    # plt.subplot(2, 3, 4)
    # plt.gca().set_title('Correction')
    # plt.plot(v)
    # plt.subplot(2, 3, 5)
    # plt.gca().set_title('Prolongated Correction')
    # plt.plot(v1)
    #
    # plt.draw()
    # plt.pause(1)

    U += v1

    return U

SIZE = 256

u = numpy.zeros(SIZE)
b = numpy.zeros(SIZE)
u[int(SIZE/2)] = 1
b[int(SIZE/2)] = 1

while True:
    u = V_Cycle(u,b,-1.0*b,1)
