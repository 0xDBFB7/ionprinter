import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math

SIZE_X = 128

# epsilon_0 = 8.85×10^−12

u = numpy.zeros((SIZE_X))
b = numpy.zeros((SIZE_X))

# for x in range(10,20):
#     u[x] = 1.0
#     b[x] = 1.0

u[64] = 1.0
b[64] = 1.0


b[0] = 1.0
b[SIZE_X-1] = 1.0

def residual(u,b,f):
    rows = u.shape[0]

    h = 1.0/(rows-1)
    h2 = 1.0/(h*h)

    R = numpy.zeros(rows)
    for x in range(0,rows-1): #careful with the edge boundaries; they must be included!
        if(b[x] == 0):
            R[x] = f[x] - (u[x+1] + u[x-1] - 2.0*u[x])
        else:
            R[x] = 0
    return R

# void resid(double **res, double **u, double **rhs, int n)
# Returns minus the residual for the model problem. Input quantities are u[1..n][1..n] and
# rhs[1..n][1..n], while res[1..n][1..n] is returned.
# {
#     int i,j;
#     double h,h2i;
#     h=1.0/(n-1);
#     h2i=1.0/(h*h);
# for (j=2;j<n;j++) Interior points.
#     for (i=2;i<n;i++)
#     res[i][j] = -h2i*(u[i+1][j]+u[i-1][j]+u[i][j+1]+u[i][j-1]- 4.0*u[i][j])+rhs[i][j];
#     for (i=1;i<=n;i++) Boundary points.
#         res[i][1]=res[i][n]=res[1][i]=res[n][i]=0.0;
# }

# def residual(u, f, b):
#     """
#     f - A u
#     """
#     n = len(f)
#     r = numpy.zeros(len(u))
#     r[1:-1] = f[1:-1] - ((n+1)**2) * (2 * u[1:-1] - u[2:] - u[:-2])
#     r[0]    = f[0]    - ((n+1)**2) * (2 * u[0]    - u[1])
#     r[-1]   = f[-1]   - ((n+1)**2) * (2 * u[-1]   - u[-2])
#     return r


def restriction(X):
    rows = X.shape[0]

    O = numpy.zeros((int(rows/2)))

    for x in range(1,int(rows/2)-2):
        O[x] = 0.25*(X[x*2-1] + X[x*2] + X[x*2+1])

    return O


def prolongate(X):
    rows = X.shape[0]

    O = numpy.zeros((rows*2))
    for x in range(0,rows-1):
        O[2*x] = X[x]
        O[2*x+1] = (X[x]+X[x+1])/2.0

    return O

def jacobi(u,b,f):
    T = u.copy()
    for x in range(1, (u.shape[0] - 2)):
        if(b[x] == 0):
            T[x] = (u[x+1] + u[x-1] + f[x])/2.0
    return T





def V_cycle(phi,b,f,h):
    u = phi.copy()
    u1 = phi.copy()
    u = jacobi(u,b,f)
    r = -residual(u,b,f)

    r1 = restriction(r)

    v = numpy.zeros_like(r1) #correction
    # b1 = restriction(
    b1 = numpy.zeros_like(r1)
    b1[0] = 1.0
    b1[b1.shape[0]-1] = 1.0
    print(numpy.linalg.norm(r),h)
    if(h == 32):
        for i in range(0,100):
            v = jacobi(v,b1,r1)
    else:
        v = V_cycle(v,b1,r1,2*h)

    v1 = prolongate(v)
    #
    # plt.figure()
    # plt.subplot(2, 3, 1)
    # plt.gca().set_title('Potentials')
    # plt.plot(u)
    # plt.subplot(2, 3, 2)
    # plt.gca().set_title('Root residual')
    # plt.plot(r)
    #
    # plt.subplot(2, 3, 3)
    # plt.gca().set_title('Restricted residual')
    # plt.plot(r1)
    # plt.plot(b1)
    # plt.subplot(2, 3, 4)
    # plt.gca().set_title('Correction')
    # plt.plot(v)
    # plt.plot(b1)
    # plt.subplot(2, 3, 5)
    # plt.gca().set_title('Prolongated correction')
    # plt.plot(v1)
    #
    # plt.draw()
    # plt.pause(0.1)

    u += v1

    return u


# f = numpy.zeros_like(u) #no space charge
# u = V_cycle(u,b,f,1)
# f = numpy.zeros_like(u) #no space charge
# u = V_cycle(u,b,f,1)
# input()

prev_r = 1
while True:


    u1 = u.copy()
    f = numpy.zeros_like(u) #no space charge
    u = V_cycle(u,b,f,1)
    r = u - u1.copy()

    prev_r = numpy.linalg.norm(r)
    # input()
    #
    #
    # prev_r = numpy.linalg.norm(r)
    #
    #
    # plt.subplot(2, 3, 1)
    # plt.gca().set_title('Potentials')
    # plt.plot(u)
    # plt.subplot(2, 3, 2)
    # plt.gca().set_title('Root residual')
    # plt.plot(r)
    #
    #
    # r1 = restriction(r)
    # v = numpy.zeros_like(r1) #correction
    # b1 = restriction(b) #no boundaries on inner levels?
    # gauss_seidel(v,b1,r1)
    # gauss_seidel(v,b1,r1)
    #
    # v1 = prolongate(v)
    #
    # plt.subplot(2, 3, 3)
    # plt.gca().set_title('Restricted residual')
    # plt.plot(r1)
    # plt.plot(b1)
    # plt.subplot(2, 3, 4)
    # plt.gca().set_title('Correction')
    # plt.plot(v)
    # plt.plot(b1)
    # plt.subplot(2, 3, 5)
    # plt.gca().set_title('Prolongated correction')
    # plt.plot(v1)
    #
    #
    # u += v1*(1.0-b)
    #
    # plt.draw()
    # plt.pause(0.1)
    # plt.clf()
    # plt.cla()
