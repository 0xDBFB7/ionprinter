"""
This is an example showing how to call the mgd2d solver.
V cycle and Full Multigrid are demonstrated
"""
import numpy as np
import time
import math
from mgd3d import V_cycle,FMG,GSrelax
import matplotlib.pyplot as plt

#analytical solution
# def Uann(x,y,z,n):
#   return 1.0/np.sqrt((x-0.5)**2.0+(y-0.5)**2.0+(z-0.5)**2.0)

def Uann(x,y,z,n):
  return np.sin(2*n*np.pi*x)*np.sin(2*n*np.pi*y)*np.sin(2*n*np.pi*z)

#RHS corresponding to above
def source(x,y,z,n):
  return 0

# def source(x,y,z,n):
#   return -12 * (np.pi)**2 * n**2 * np.sin(2*n*np.pi*x) * np.sin(2*n*np.pi*y) * np.sin(2*n*np.pi*z)


#input
max_cycles = 20           #maximum number of V cycles
nlevels    = 4           #total number of grid levels. 1 means no multigrid, 2 means one coarse grid. etc
NX         = 3*2**(5-1) #Nx and Ny are given as function of grid levels
NY         = 3*2**(5-1)
NZ         = 3*2**(5-1)
tol        = 1e-6

#the grid has one layer of ghost cells to help apply the boundary conditions
uann=np.zeros([NX+2,NY+2,NZ+2])#analytical solution
u   =np.zeros([NX+2,NY+2,NZ+2])#approximation
f   =np.zeros([NX+2,NY+2,NZ+2])#RHS

#calcualte the RHS and exact solution
DX=1.0/NX
DY=1.0/NY
DZ=1.0/NZ

xc=np.linspace(0.5*DX,1-0.5*DX,NX)
yc=np.linspace(0.5*DY,1-0.5*DY,NY)
zc=np.linspace(0.5*DZ,1-0.5*DZ,NZ)

XX,YY,ZZ=np.meshgrid(xc,yc,zc)

uann[1:NX+1,1:NY+1,1:NZ+1] = Uann  (XX,YY,ZZ,1)
f   [1:NX+1,1:NY+1,1:NZ+1] = source(XX,YY,ZZ,1)

#
# plt.figure()
# plt.subplot(2, 3, 1)
# plt.gca().set_title('Potentials')
# plt.imshow(uann[:,:,int(NX/2)])
#
# plt.draw()
# plt.pause(1)


print('mgd3d.py solver:')
print('NX:',NX,', NY:',NY,'NZ:',NZ,', tol:',tol,'levels: ',nlevels)

#start solving
tb=time.time()

b = np.zeros_like(u)
u[int(NX/2),int(NY/2),int(NZ/2)] = 1
b[int(NX/2),int(NY/2),int(NZ/2)] = 1


#V cycle
for it in range(1,max_cycles+1):
  u,res=V_cycle(b,NX,NY,NZ,nlevels,u,b)

  rtol=np.max(np.max(np.abs(res)))
  if(rtol<tol):
    break
  error=uann[1:NX+1,1:NY+1,1:NZ+1]-u[1:NX+1,1:NY+1,1:NZ+1]
  en=np.max(np.max(np.abs(error)))
  print('  cycle: ',it,', L_inf(res.)= ',rtol,',L_inf(true error): ',en)

print('Elapsed time: ',time.time()-tb,' seconds')

u,res=FMG(b,NX,NY,NZ,nlevels,b,1)
print('Elapsed time: ',time.time()-tb,' seconds')
error=uann[1:NX+1,1:NY+1,1:NZ+1]-u[1:NX+1,1:NY+1,1:NZ+1]
en=np.max(np.max(np.abs(error)))
print('L_inf(true error): ',en)
