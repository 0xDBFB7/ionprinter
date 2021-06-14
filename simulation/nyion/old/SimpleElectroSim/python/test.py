from pylab import *
from scipy import *
from numpy import *
from matplotlib import *

N1=50 # number of points along x and y
N=2*N1+1 # number of points in total
# in cm.
xc1=4
xc2=9
yc1=0
yc2=0
R1=1.75
R2=9
ecart = 1



a, b = linspace(-1, 19, N), linspace(-10, 10, N)
xa, ya = meshgrid(a, b)
V = zeros_like(xa)


for i in range(N):
    for j in range(N):
        x, y = xa[i,j], ya[i,j]
        if (((x-xc1)**2/(R1**2))+((y-yc1)**2/(R1**2)))<=1 : # potential in the central conductor
            V[i,j] = 30
        if (((x-xc2)**2/(R2**2))+((y-yc2)**2/(R2**2)))>=1 : # potential in the outer conductor
            V[i,j] =0

#draws the potential along X along the axis of symmetry.


Vnew = V.copy()

while ecart > 5*10**-2:

  for i in range(1,N-1):

   for j in range(1,N-1):

    Vnew[i,j] = 0.25*(V[i-1,j] + V[i+1,j] + V [i,j-1] + V[i,j+1])

  # convergence criterion

  ecart = np.max(np.abs(V - (Vnew))/np.max(V))

  print(ecart)
  # save in the grid V of the calculated grid
  for i in range(N):
        for j in range(N):
            x, y = xa[i,j], ya[i,j]
            if (((x-xc1)**2/(R1**2))+((y-yc1)**2/(R1**2))) > 1 and (((x-xc2)**2/(R2**2))+((y-yc2)**2/(R2**2))) < 1 :
                    V[i,j] = Vnew[i,j]
