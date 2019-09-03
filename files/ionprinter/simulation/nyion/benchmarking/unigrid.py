import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
from itertools import cycle
SIZE_X = 128

u = numpy.zeros(SIZE_X)
b = numpy.zeros(SIZE_X)

# for x in range(40,50):
#     u[x] = 1
#     b[x] = 1


#
# dH = [0]*10
# dH[i] = numpy.identity(H)
# for i in range(1,10):
#     H=2**i
#     for k in range(1,SIZE_X-1):
#         dH[i] = 0.5*dH[i-1][2k-1] + dH[i-1][2k+1] + 0.5*dH[i-1][2k+1]  //

def FND(I3, J3):
    return (M1-math.fabs(I-I3))*(M1-math.fabs(J-J3))/(M1*M1)

I1 = 64
J1 = 64
H = 1/8##?

U = numpy.zeros((I1,J1))
F = numpy.zeros((I1,J1))
B = numpy.zeros((I1,J1))
for I in range(0,I1):
    for J in range(0,J1):
#         U[I,J] = 10
#         U[I+15,J] = -10
# #
#         B[I,J] = 1
#         B[I+15,J] = 1
        U[I,J] = math.cos(3.0*(I+J-2)*H)

for I in range(20,30):
    for J in range(20,30):
        U[I,J] = 100
        B[I,J] = 1

N=4
prev_E = 1
while True:

    #
    # for root_level in range(0,N+1):
    for k in list(range(0,N+1)): #levels
        M1 = 2**(N-k)
        print(M1)
        for relaxes in range(0,1): #Number of relaxations; 1 usually suffices
            E=0

            T = U.copy()
            for I in range(M1,I1-M1,M1):
                for J in range(M1,J1-M1,M1):
                    A1=0
                    R1=0
                    for I3 in range(I-M1+1,I+M1): #fix ranges
                         for J3 in range(J-M1+1,J+M1):
                            D = 4
                            F[I3,J3] = 0
                            if(k < 2): # add a layer of boundaries
                                current_val = (U*(1.0-B))[I3-1:I3+1,J3-1:J3+1].max()/6
                            else:
                                current_val = U[I3,J3]
                            R = (D*current_val) - U[I3,J3-1] - U[I3,J3+1] - U[I3-1,J3] - U[I3+1,J3] - F[I3,J3] #compute residual
                            A3 = D*FND(I3,J3) - FND(I3,J3+1) - FND(I3,J3-1) - FND(I3+1,J3) - FND(I3-1,J3)
                            R1 = R1 + FND(I3,J3)*R
                            A1 = A1 + FND(I3,J3)*A3

                    S=R1/A1
                    E=E+R1*R1
                    for I3 in range(I-M1+1,I+M1):
                        for J3 in range(J-M1+1,J+M1):
                            if(not B[I3,J3]):
                                T[I3,J3] = U[I3,J3] - 0.8*S*FND(I3,J3)
            numpy.copyto(U,T)

        E=math.sqrt(E)/M1/H
        print(E)
        # #FND COMPUTES THE UNIGRID DIRECTIONS

    plt.subplot(2, 3, 2)
    plt.gca().set_title('Potentials')
    plt.imshow(U)
    plt.subplot(2, 3, 3)
    plt.gca().set_title('Boundaries')
    plt.imshow(B)
    print("Converge: {}".format(E/prev_E))
    prev_E = E
    #
    plt.draw()
    plt.pause(0.001)
    plt.cla()
