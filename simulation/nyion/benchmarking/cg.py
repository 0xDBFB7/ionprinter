import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
from itertools import cycle


I1 = 64
J1 = 64

U = numpy.ones((I1,J1))
F = numpy.zeros((I1,J1))
B = numpy.zeros((I1,J1),dtype=numpy.int8)

# U[0,2] = 0.25
F[10,10] = 10
# for I in range(0,I1):
#     for J in range(0,J1):
#         U[I,J] = math.cos(3.0*(I+J-2))

# for I in range(15,20):
#     for J in range(15,20):
#         U[I,J] = 1
#         B[I,J] = 1

def residual(u,boundaries,f):
    r = numpy.zeros_like(u)
    for x in range(1,u.shape[0]-1):
        for y in range(1,u.shape[1]-1):
            if(not boundaries[x,y]):
                r[x,y] = f[x,y] - (4.0*u[x,y] - u[x+1,y] - u[x-1,y] - u[x,y+1] - u[x,y-1]) #may have to be flipped
                # R = (D*U[I3,J3]) - U[I3,J3-1] - U[I3,J3+1] - U[I3-1,J3] - U[I3+1,J3]
                # R -= F[I3,J3] #compute residual
    return r

def conjugate_grad(u, boundaries, f):
    r = numpy.zeros_like(u)
    #########r = b - A*x################
    for x in range(1,u.shape[0]-1):
        for y in range(1,u.shape[1]-1):
            r[x,y] = f[x,y] - (4.0*u[x,y] - u[x+1,y] - u[x-1,y] - u[x,y+1] - u[x,y-1]) #may have to be flipped
    ####################################
    p = r.copy()



    rsold = numpy.dot(r,r)
    print(rsold)
    for i in range(0,1000):
        if(numpy.linalg.norm(numpy.dot(numpy.transpose(r),r)) < 1e-5):
            break
        #############v = A*p#################
        v = numpy.zeros_like(u)
        for x in range(1,u.shape[0]-1):
            for y in range(1,u.shape[1]-1):
                v[x,y] = (4.0*p[x,y] - p[x+1,y] - p[x-1,y] - p[x,y+1] - p[x,y-1])

        if(not i == 0):
            alpha = rsold / numpy.dot(numpy.transpose(p), v)
        else:
            alpha = 1

        u += numpy.dot(alpha, p)

        r -= numpy.dot(alpha,v)

        rsnew = numpy.transpose(r)*r

        p = r + numpy.dot((rsnew / rsold),p)

        rsold = rsnew
        print(numpy.linalg.norm(r))

        plt.subplot(2, 3, 2)
        plt.gca().set_title('Potentials')
        plt.imshow(r)
        #
        plt.draw()
        plt.pause(0.001)
        plt.cla()

    return u

# def conjugate_grad(u, boundaries, f):
#     """
#     Description
#     -----------
#     Solve a linear equation Ax = b with conjugate gradient method.
#     Parameters
#     ----------
#     A: 2d numpy.array of positive semi-definite (symmetric) matrix
#     b: 1d numpy.array
#     x: 1d numpy.array of initial point
#     Returns
#     -------
#     1d numpy.array x such that Ax = b
#
#     Thanks
#     <https://gist.github.com/sfujiwara/b135e0981d703986b6c2#file-cg-py-L51>
#     """
#
#     r = residual(u,boundaries,f)
#     p = r.copy() #initial search direction
#
#     r_k_norm = numpy.dot(r, r)
#     Ap = numpy.zeros_like(u)
#     for i in range(0,100):
#         for x in range(1,u.shape[0]-1):
#             for y in range(1,u.shape[1]-1):
#                 if(not boundaries[x,y]):
#                     Ap[x,y] = 4.0*p[x,y] - p[x-1,y] - p[x+1,y] - p[x-1,y] - p[x+1,y]
#         alpha = numpy.zeros_like(r)
#         for x in range(1,u.shape[0]-1):
#             for y in range(1,u.shape[1]-1):
#                 if(not boundaries[x,y]):
#                     alpha[x,y] = r_k_norm[x,y] / (p[x,y] * Ap[x,y])
#
#         u += alpha * p
#         for x in range(1,u.shape[0]-1):
#             for y in range(1,u.shape[1]-1):
#                 if(not boundaries[x,y]):
#                     u[x,y] += alpha[x,y] * p[x,y]
#         r += alpha * Ap
#
#         r_kplus1_norm = numpy.dot(r, r)
#         beta = numpy.zeros_like(r)
#         for x in range(1,u.shape[0]-1):
#             for y in range(1,u.shape[1]-1):
#                 if(not boundaries[x,y]):
#                     beta[x,y] = r_kplus1_norm[x,y] / r_k_norm[x,y]
#         r_k_norm = r_kplus1_norm
#         if numpy.linalg.norm(r_kplus1_norm) < 1e-5:
#             break
#         p = beta * p - r
#         print("It1")
#
#     return u

E = 1
prev_E = 1

# T = U.copy()
U = conjugate_grad(U,B,F)

# T = U - T
# E = numpy.linalg.norm(T)
plt.subplot(2, 3, 2)
plt.gca().set_title('Potentials')
plt.imshow(U)
# plt.subplot(2, 3, 3)
# plt.gca().set_title('Boundaries')
# plt.imshow(B)
print("Converge: {}".format(E/prev_E))
# print("a: {}".format(numpy.linalg.norm(T)))
prev_E = E
#
plt.show()
