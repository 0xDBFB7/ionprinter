"""
2017 (c) A. R. Malipeddi
3D geometric multigrid code for poissons equation in a cube.
 - Finite difference method
 - 7pt operator
 - trilinear interpolation
 - Two-color Gauss Seidel smoothing

"""
import matplotlib.pyplot as plt

import numpy as np

def GSrelax(B,nx,ny,nz,u,f,iters=1,flag=1):
  '''
  Red-Black Gauss Seidel smoothing
  flag : 1 = pre-sweep
         2 = post-sweep
  '''

  dx=1.0/nx
  dy=1.0/ny
  dz=1.0/nz

  Ax=1.0/dx**2
  Ay=1.0/dy**2
  Az=1.0/dz**2
  Ap=1.0/(2.0*(1.0/dx**2+1.0/dy**2+1.0/dz**2))

  #BCs. Needs to be generalized!
  u[ 0,:,:] = 0
  u[-1,:,:] = 0
  u[: ,0,:] = 0
  u[:,-1,:] = 0
  u[:,:, 0] = 0
  u[:,:,-1] = 0

  T = u.copy()
  for it in range(iters):
    for i in range(1,nx+1):
        for j in range(1,ny+1):
            for k in range(1,nz+1):
                if(not B[i,j,k]):
                    T[i,j,k]= Ap*( Ax*(u[i+1,j,k]+u[i-1,j,k])
                        + Ay*(u[i,j+1,k]+u[i,j-1,k])
                        + Az*(u[i,j,k+1]+u[i,j,k-1])
                        - f[i,j,k])
    np.copyto(u,T)
    #BCs. Needs to be generalized!
    u[ 0,:,:] = 0
    u[-1,:,:] = 0
    u[: ,0,:] = 0
    u[:,-1,:] = 0
    u[:,:, 0] = 0
    u[:,:,-1] = 0

  #if residual not needed
  if(flag==2):
    return u,None

  res=np.zeros([nx+2,ny+2,nz+2])

  for i in range(1,nx+1):
    for j in range(1,ny+1):
      for k in range(1,nz+1):
          if(not B[i,j,k]):
            res[i,j,k]=f[i,j,k] - (Ax*(u[i+1,j,k]+u[i-1,j,k])
                                +  Ay*(u[i,j+1,k]+u[i,j-1,k])
                                +  Az*(u[i,j,k+1]+u[i,j,k-1])
                                -  2.0*(Ax+Ay+Az)*u[i,j,k])



  return u,res


def restrict(B,nx,ny,nz,v):
  '''
  restrict 'v' to the coarser grid
  '''
  v_c=np.zeros([nx+2,ny+2,nz+2])

  for i in range(1,nx+1):
    for j in range(1,ny+1):
      for k in range(1,nz+1):
          # if(not B[i*2,j*2,k*2]):
          v_c[i,j,k]=0
          count = 0
          if(not B[i*2-1,j*2,k*2]):
              v_c[i,j,k] += v[2*i-1,2*j,2*k]
              count+=1
          if(not B[i*2,j*2-1,k*2]):
              v_c[i,j,k] += v[2*i,2*j-1,2*k]
              count+=1
          if(not B[i*2,j*2-1,k*2]):
              v_c[i,j,k] += v[2*i,2*j-1,2*k]
              count+=1
          if(not B[i*2,j*2,k*2]):
              v_c[i,j,k] += v[2*i,2*j,2*k]
              count+=1

          v_c[i,j,k] /= count
              # 0.125*(v[2*i-1,2*j-1,2*k-1]+v[2*i,2*j-1,2*k-1]+v[2*i-1,2*j,2*k-1]+v[2*i,2*j,2*k-1]
              #            +v[2*i-1,2*j-1,2*k  ]+v[2*i,2*j-1,2*k  ]+v[2*i-1,2*j,2*k  ]+v[2*i,2*j,2*k  ])
  return v_c

def restrict_B(nx,ny,nz,B):
    v_c=np.zeros([nx+2,ny+2,nz+2])
    for i in range(1,nx+1):
      for j in range(1,ny+1):
        for k in range(1,nz+1):
            v_c[i,j,k] = B[i*2,j*2,k*2]

    return v_c

# def prolong(B,nx,ny,nz,v):
#   '''
#   interpolate correction to the fine grid
#   '''
#   v_f=np.zeros([2*nx+2,2*ny+2,2*nz+2])
#
#   a=27.0/64
#   b= 9.0/64
#   c= 3.0/64
#   d= 1.0/64
#
#   for i in range(1,nx+1):
#     for j in range(1,ny+1):
#       for k in range(1,nz+1):
#             v_f[2*i-1,2*j-1,2*k-1] = a*v[i,j,k] + b*(v[i-1,j,k] + v[i,j-1,k] + v[i,j,k-1]) + c*(v[i-1,j-1,k] + v[i-1,j,k-1] + v[i,j-1,k-1]) + d*v[i-1,j-1,k-1]
#             v_f[2*i  ,2*j-1,2*k-1] = a*v[i,j,k] + b*(v[i+1,j,k] + v[i,j-1,k] + v[i,j,k-1]) + c*(v[i+1,j-1,k] + v[i+1,j,k-1] + v[i,j-1,k-1]) + d*v[i+1,j-1,k-1]
#             v_f[2*i-1,2*j  ,2*k-1] = a*v[i,j,k] + b*(v[i-1,j,k] + v[i,j+1,k] + v[i,j,k-1]) + c*(v[i-1,j+1,k] + v[i-1,j,k-1] + v[i,j+1,k-1]) + d*v[i-1,j+1,k-1]
#             v_f[2*i  ,2*j  ,2*k-1] = a*v[i,j,k] + b*(v[i+1,j,k] + v[i,j+1,k] + v[i,j,k-1]) + c*(v[i+1,j+1,k] + v[i+1,j,k-1] + v[i,j+1,k-1]) + d*v[i+1,j+1,k-1]
#             v_f[2*i-1,2*j-1,2*k  ] = a*v[i,j,k] + b*(v[i-1,j,k] + v[i,j-1,k] + v[i,j,k+1]) + c*(v[i-1,j-1,k] + v[i-1,j,k+1] + v[i,j-1,k+1]) + d*v[i-1,j-1,k+1]
#             v_f[2*i  ,2*j-1,2*k  ] = a*v[i,j,k] + b*(v[i+1,j,k] + v[i,j-1,k] + v[i,j,k+1]) + c*(v[i+1,j-1,k] + v[i+1,j,k+1] + v[i,j-1,k+1]) + d*v[i+1,j-1,k+1]
#             v_f[2*i-1,2*j  ,2*k  ] = a*v[i,j,k] + b*(v[i-1,j,k] + v[i,j+1,k] + v[i,j,k+1]) + c*(v[i-1,j+1,k] + v[i-1,j,k+1] + v[i,j+1,k+1]) + d*v[i-1,j+1,k+1]
#             v_f[2*i  ,2*j  ,2*k  ] = a*v[i,j,k] + b*(v[i+1,j,k] + v[i,j+1,k] + v[i,j,k+1]) + c*(v[i+1,j+1,k] + v[i+1,j,k+1] + v[i,j+1,k+1]) + d*v[i+1,j+1,k+1]
#
#   return v_f

def prolong(B,nx,ny,nz,v):
    v_f=np.zeros([2*nx+2,2*ny+2,2*nz+2])
    for x in range(1, nx+1):
        for y in range(1, ny+1):
            for z in range(1, nz+1):
                V000 = v[x,y,z]
                V001 = v[x,y,z+1]
                V010 = v[x,y+1,z]
                V100 = v[x+1,y,z]
                V101 = v[x+1,y,z+1]
                V110 = v[x+1,y+1,z]
                V111 = v[x+1,y+1,z+1]
                for i in range(0,2):
                    for j in range(0,2):
                        for k in range(0,2):
                            f_x = float(i)/2
                            f_y = float(j)/2
                            f_z = float(k)/2

                            v_f[2*x+i,2*y+j,2*z+k] = 0
                            v_f[2*x+i,2*y+j,2*z+k] += V000*(1.0-f_x)*(1.0-f_y)*(1.0-f_z)
                            v_f[2*x+i,2*y+j,2*z+k] += V001*(1.0-f_x)*(1.0-f_y)*(f_z)
                            v_f[2*x+i,2*y+j,2*z+k] += V010*(1.0-f_x)*(f_y)*(1.0-f_z)
                            v_f[2*x+i,2*y+j,2*z+k] += V100*(f_x)*(1.0-f_y)*(1.0-f_z)
                            v_f[2*x+i,2*y+j,2*z+k] += V101*(f_x)*(1.0-f_y)*(f_z)
                            v_f[2*x+i,2*y+j,2*z+k] += V110*(f_x)*(f_y)*(1.0-f_z)
                            v_f[2*x+i,2*y+j,2*z+k] += V111*(f_x)*(f_y)*(f_z)
    return v_f

def V_cycle(B,nx,ny,nz,num_levels,u,f,level=1):
  '''
  V cycle
  '''
  if(level==num_levels):#bottom solve
    u,res=GSrelax(B,nx,ny,nz,u,f,iters=100)
    return u,res

  #Step 1: Relax Au=f on this grid
  u,res=GSrelax(B,nx,ny,nz,u,f,2)

  # plt.figure()
  # plt.subplot(2, 3, 2)
  # plt.gca().set_title('Potentials')
  # plt.plot(u[:,int(nx/2),int(nx/2)])
  #
  # plt.subplot(2, 3, 3)
  # plt.gca().set_title('Residuals')
  # plt.plot(res[:,int(nx/2),int(nx/2)])
  # plt.subplot(2, 3, 3)
  # plt.plot(B[:,int(nx/2),int(nx/2)])

  #Step 2: Restrict residual to coarse grid
  res_c=restrict(B,nx//2,ny//2,nz//2,res)
  b_c=restrict_B(nx//2,ny//2,nz//2,B)
  #
  # plt.subplot(2, 3, 4)
  # plt.gca().set_title('Restricted Residuals')
  # plt.plot(res_c[:,int(nx/4),int(nx/4)])
  # plt.subplot(2, 3, 4)
  # plt.plot(b_c[:,int(nx/4),int(nx/4)])

  print(np.amax(u))
  #Step 3:Solve A e_c=res_c on the coarse grid. (Recursively)
  e_c=np.zeros_like(res_c)
  e_c,res_c=V_cycle(b_c,nx//2,ny//2,nz//2,num_levels,e_c,res_c,level+1)
  #
  # plt.subplot(2, 3, 5)
  # plt.gca().set_title('Restricted Correction')
  # plt.plot(e_c[:,int(nx/4),int(nx/4)])

  #Step 4: Interpolate(prolong) e_c to fine grid and add to u
  R = prolong(B,nx//2,ny//2,nz//2,e_c)*(1.0-B)
  u+= R
  #
  # plt.subplot(2, 3, 6)
  # plt.gca().set_title('Prolongated Correction')
  # plt.plot(R[:,int(nx/2),int(nx/2)])
  # plt.subplot(2, 3, 6)
  # plt.gca().set_title('Prolongated Correction')
  # plt.plot(B[:,int(nx/2),int(nx/2)])
  #
  # plt.draw()
  # plt.pause(1)

  #Step 5: Relax Au=f on this grid
  if(level==1):
    u,res=GSrelax(B,nx,ny,nz,u,f,2,flag=1)
    input(">")
  else:
    u,res=GSrelax(B,nx,ny,nz,u,f,2,flag=2)
  return u,res


def FMG(B,nx,ny,nz,num_levels,f,nv=1,level=1):
  if(level==num_levels):#bottom solve
    u=np.zeros([nx+2,ny+2,nz+2])
    u,res=GSrelax(B,nx,ny,nz,u,f,iters=100)
    return u,res

  #Step 1: Restrict the rhs to a coarse grid
  f_c=restrict(B,nx//2,ny//2,nz//2,f)
  b_c=restrict_B(nx//2,ny//2,nz//2,B)

  #Step 2: Solve the coarse grid problem using FMG
  u_c,_=FMG(b_c,nx//2,ny//2,nz//2,num_levels,f_c,nv,level+1)

  #Step 3: Interpolate u_c to the fine grid
  u=prolong(B,nx//2,ny//2,nz//2,u_c)

  #step 4: Execute 'nv' V-cycles
  for _ in range(nv):
    u,res=V_cycle(B,nx,ny,nz,num_levels-level,u,f)
  return u,res
