import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
from itertools import cycle


I1 = 32
J1 = 32
H = 1/8##?

U = numpy.zeros((I1,J1))
F = numpy.zeros((I1,J1))
B = numpy.zeros((I1,J1),dtype=numpy.int8)
for I in range(0,I1):
    for J in range(0,J1):
        U[I,J] = math.cos(3.0*(I+J-2)*H)

for I in range(15,20):
    for J in range(15,20):
        U[I,J] = 1
        B[I,J] = 0

def residual(U):
    

def conjugate_grad(U, boundaries, F):
    """
    Description
    -----------
    Solve a linear equation Ax = b with conjugate gradient method.
    Parameters
    ----------
    A: 2d numpy.array of positive semi-definite (symmetric) matrix
    b: 1d numpy.array
    x: 1d numpy.array of initial point
    Returns
    -------
    1d numpy.array x such that Ax = b

    Thanks
    <https://gist.github.com/sfujiwara/b135e0981d703986b6c2#file-cg-py-L51>
    """
    A = numpy.zeros((32,32,32)) #construct the diagonal solver matrix
    i,j,k = numpy.indices(A.shape)
    A[i==j and i==k] = 4
    A[(i==j-1 and i == k-1)] = -1
    A[(i==j+1 and i == k+1)] = -1
    A[(i==j-1 and i == k-1)] = -1
    A[(i==j+1 and i == k+1)] = -1

    print(A)
    r = numpy.dot(A, U) - F
    p = - r
    r_k_norm = numpy.dot(r, r)
    for i in range(2): #iteration count
        Ap = numpy.dot(A, p)
        alpha = r_k_norm / numpy.dot(p, Ap)
        U += alpha * p
        r += alpha * Ap
        r_kplus1_norm = numpy.dot(r, r)
        beta = r_kplus1_norm / r_k_norm
        r_k_norm = r_kplus1_norm
        if r_kplus1_norm.max() < 1e-5:
            break
        p = beta * p - r
    return U

E = 1
prev_E = 1
while True:
    T = U.copy()
    U = conjugate_grad(U,B,F)

    T = U - T
    E = numpy.linalg.norm(T)
    plt.subplot(2, 3, 2)
    plt.gca().set_title('Potentials')
    plt.imshow(U)
    plt.subplot(2, 3, 3)
    plt.gca().set_title('Boundaries')
    plt.imshow(B)
    print("Converge: {}".format(E/prev_E))
    print("a: {}".format(numpy.linalg.norm(T)))
    prev_E = E
    #
    plt.draw()
    plt.pause(0.001)
    plt.cla()
