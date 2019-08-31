import numpy
import matplotlib.pyplot as plt

def gauss_seidel(U,B,F):
    h=1.0/(U.shape[0]-1);
    h2=h*h;
    T = U.copy()
    for i in range(1,U.shape[0]-1):
        if(B[i] == 0):
            T[i] = 0.5*(U[i+1] + U[i-1]) - h2*F[i]

    return T

def residual(U,B,F):
    h=1.0/(len(U)-1);
    h2=1.0/(h*h);
    R = numpy.zeros_like(U)
    for i in range(1,U.shape[0]-1):
        if(B[i] == 0):
            R[i] = F[i] - h2*(U[i+1] + U[i-1] - 2.0*U[i])

    return R

def restriction(X): #boundary points are handled differently: see "Multigrid Methods for Boundary Value Problems"
    rows = X.shape[0]

    O = numpy.zeros((int(rows/2)))

    for x in range(0,int(rows/2)-1):
        O[x] = (X[x*2] + X[x*2+1])/2.0

    return O


def prolongate(X):
    rows = X.shape[0]

    O = numpy.zeros((rows*2))
    for x in range(0,rows-1):
        O[2*x] = X[x]
        O[2*x+1] = (X[x]+X[x+1])/2.0

    return O

SIZE = 256

u = numpy.zeros(SIZE)
b = numpy.zeros(SIZE)
u[int(SIZE/2)] = 1
b[int(SIZE/2)] = 1

while True:
    f = numpy.zeros(SIZE)
    for i in range(0,10):
        u = gauss_seidel(u,b,f)

    r = residual(u,b,f)

    print(numpy.linalg.norm(r))


    #r1 = r.copy()[1::2]
    r1 = restriction(r)
    v = numpy.zeros_like(r1)
    #b1 = b.copy()[1::2]
    b1 = restriction(b)
    v = gauss_seidel(v,b1,r1)
    v = gauss_seidel(v,b1,r1)

    v1 = prolongate(v)*(1.0-b)

    plt.subplot(2, 3, 1)
    plt.gca().set_title('Potentials')
    plt.plot(u)
    plt.subplot(2, 3, 2)
    plt.gca().set_title('Residual')
    plt.plot(r)
    plt.subplot(2, 3, 3)
    plt.gca().set_title('Restricted residual')
    plt.plot(r1)
    plt.subplot(2, 3, 4)
    plt.gca().set_title('Correction')
    plt.plot(v)
    plt.subplot(2, 3, 5)
    plt.gca().set_title('Prolongated Correction')
    plt.plot(v1)

    plt.draw()
    plt.pause(1)

    u += v1
