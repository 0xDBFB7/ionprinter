import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
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

I1 = 97
J1 = 65
H = 1/32##?

U = numpy.zeros((I1,J1))
F = numpy.zeros((I1,J1))

# for I in range(0,I1):
#     for J in range(0,J1):
#         U[I,J] = math.cos(3.0*(I+J-2)*H)
# U[10,10] = 10

N=6
prev_E = 1
while True:
    for k in range(0,N): #levels
        M1 = 2**(N-k)
        for relaxes in range(0,1): #Number of relaxations; 1 usually suffices
            E=0
            for I in range(1+M1,I1-M1,M1):
                for J in range(1+M1,J1-M1,M1):
                    A1=0
                    R1=0
                    for I3 in range(I-M1+1,I+M1): #fix ranges
                        for J3 in range(J-M1+1,J+M1):
                            D = 4 + math.exp((I3+J3)*H)*H*H
                            F[I3,J3] = math.sin(3*(I3+J3-2)*H)*H*H
                            R = (D*U[I3,J3]) - U[I3,J3-1] - U[I3,J3+1] - U[I3-1,J3] - U[I3+1,J3] - F[I3,J3] #compute residual
                            A3 = D*FND(I3,J3) - FND(I3,J3+1) - FND(I3,J3-1) - FND(I3+1,J3) - FND(I3-1,J3)
                            R1 = R1 + FND(I3,J3)*R
                            A1 = A1 + FND(I3,J3)*A3
                    S=R1/A1
                    E=E+R1*R1
                    for I3 in range(I-M1+1,I+M1):
                        for J3 in range(J-M1+1,J+M1):
                            U[I3,J3] = U[I3,J3] - S*FND(I3,J3)

        E=math.sqrt(E)/M1/H
        print(E)
        #FND COMPUTES THE UNIGRID DIRECTIONS

    plt.subplot(2, 3, 2)
    plt.gca().set_title('Potentials')
    plt.imshow(U)
    print("Converge: {}".format(E/prev_E))
    prev_E = E
    #
    plt.draw()
    plt.pause(0.001)
    plt.cla()