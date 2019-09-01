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

def GSrelax(b,nx,ny,nz,u,f,iters=1,flag=1):

  dx=1.0/nx
  dy=1.0/ny
  dz=1.0/nz
  print(dx,dy,dz)
  Ax=1.0/dx**2
  Ay=1.0/dy**2
  Az=1.0/dz**2
  Ap=1.0/(2.0*(1.0/dx**2+1.0/dy**2+1.0/dz**2))

  #BCs. Needs to be generalized!
  #
  u[ 0,:,:] = -u[ 1,:,:]
  u[-1,:,:] = -u[-2,:,:]
  u[: ,0,:] = -u[:, 1,:]
  u[:,-1,:] = -u[:,-2,:]
  u[:,:, 0] = -u[:,:, 1]
  u[:,:,-1] = -u[:,:,-2]

  T = u.copy()
  for it in range(iters):
    for i in range(1,nx):
        for j in range(1,ny):
            for k in range(1,nz):
                # if(b[i,j,k] == 0):
                    T[i,j,k]= Ap*( Ax*(u[i+1,j,k]+u[i-1,j,k])
                        + Ay*(u[i,j+1,k]+u[i,j-1,k])
                        + Az*(u[i,j,k+1]+u[i,j,k-1])
                        - f[i,j,k])

    np.copyto(u,T) #jacobi, not g-s

    # u[int(nx/2),int(ny/2),int(nz/2)] = 1
    #BCs. Needs to be generalized!
    u[ 0,:,:] = -u[ 1,:,:]
    u[-1,:,:] = -u[-2,:,:]
    u[: ,0,:] = -u[:, 1,:]
    u[:,-1,:] = -u[:,-2,:]
    u[:,:, 0] = -u[:,:, 1]
    u[:,:,-1] = -u[:,:,-2]

  #if residual not needed
  if(flag==2):
    return u,None

  res=np.zeros([nx+2,ny+2,nz+2])

  for i in range(1,nx+1):
    for j in range(1,ny+1):
      for k in range(1,nz+1):
          # if(b[i,j,k] == 0):
        res[i,j,k]=f[i,j,k] - (Ax*(u[i+1,j,k]+u[i-1,j,k])
                            +  Ay*(u[i,j+1,k]+u[i,j-1,k])
                            +  Az*(u[i,j,k+1]+u[i,j,k-1])
                            -  2.0*(Ax+Ay+Az)*u[i,j,k])
    #
    # plt.figure()
    # plt.subplot(2, 3, 1)
    # plt.gca().set_title('Potentials')
    # plt.imshow(u[:,:,int(nx/2)])
    #
    # plt.subplot(2, 3, 2)
    # plt.gca().set_title('Residuals')
    # plt.imshow(res[:,:,int(nx/2)])
    # plt.subplot(2, 3, 2)
    # plt.gca().set_title('Boundaries')
    # plt.imshow(b[:,:,int(nx/2)])
    # plt.draw()
    # plt.pause(1)


  return u,res


def restrict(nx,ny,nz,v):
  '''
  restrict 'v' to the coarser grid
  '''
  v_c=np.zeros([nx+2,ny+2,nz+2])

  for i in range(1,nx+1):
    for j in range(1,ny+1):
      for k in range(1,nz+1):
        v_c[i,j,k]=0.125*(v[2*i-1,2*j-1,2*k-1]+v[2*i,2*j-1,2*k-1]+v[2*i-1,2*j,2*k-1]+v[2*i,2*j,2*k-1]
                         +v[2*i-1,2*j-1,2*k  ]+v[2*i,2*j-1,2*k  ]+v[2*i-1,2*j,2*k  ]+v[2*i,2*j,2*k  ])
  return v_c


def prolong(nx,ny,nz,v):
  '''
  interpolate correction to the fine grid
  '''
  v_f=np.zeros([2*nx+2,2*ny+2,2*nz+2])

  a=27.0/64
  b= 9.0/64
  c= 3.0/64
  d= 1.0/64

  for i in range(1,nx+1):
    for j in range(1,ny+1):
      for k in range(1,nz+1):
        v_f[2*i-1,2*j-1,2*k-1] = a*v[i,j,k] + b*(v[i-1,j,k] + v[i,j-1,k] + v[i,j,k-1]) + c*(v[i-1,j-1,k] + v[i-1,j,k-1] + v[i,j-1,k-1]) + d*v[i-1,j-1,k-1]
        v_f[2*i  ,2*j-1,2*k-1] = a*v[i,j,k] + b*(v[i+1,j,k] + v[i,j-1,k] + v[i,j,k-1]) + c*(v[i+1,j-1,k] + v[i+1,j,k-1] + v[i,j-1,k-1]) + d*v[i+1,j-1,k-1]
        v_f[2*i-1,2*j  ,2*k-1] = a*v[i,j,k] + b*(v[i-1,j,k] + v[i,j+1,k] + v[i,j,k-1]) + c*(v[i-1,j+1,k] + v[i-1,j,k-1] + v[i,j+1,k-1]) + d*v[i-1,j+1,k-1]
        v_f[2*i  ,2*j  ,2*k-1] = a*v[i,j,k] + b*(v[i+1,j,k] + v[i,j+1,k] + v[i,j,k-1]) + c*(v[i+1,j+1,k] + v[i+1,j,k-1] + v[i,j+1,k-1]) + d*v[i+1,j+1,k-1]
        v_f[2*i-1,2*j-1,2*k  ] = a*v[i,j,k] + b*(v[i-1,j,k] + v[i,j-1,k] + v[i,j,k+1]) + c*(v[i-1,j-1,k] + v[i-1,j,k+1] + v[i,j-1,k+1]) + d*v[i-1,j-1,k+1]
        v_f[2*i  ,2*j-1,2*k  ] = a*v[i,j,k] + b*(v[i+1,j,k] + v[i,j-1,k] + v[i,j,k+1]) + c*(v[i+1,j-1,k] + v[i+1,j,k+1] + v[i,j-1,k+1]) + d*v[i+1,j-1,k+1]
        v_f[2*i-1,2*j  ,2*k  ] = a*v[i,j,k] + b*(v[i-1,j,k] + v[i,j+1,k] + v[i,j,k+1]) + c*(v[i-1,j+1,k] + v[i-1,j,k+1] + v[i,j+1,k+1]) + d*v[i-1,j+1,k+1]
        v_f[2*i  ,2*j  ,2*k  ] = a*v[i,j,k] + b*(v[i+1,j,k] + v[i,j+1,k] + v[i,j,k+1]) + c*(v[i+1,j+1,k] + v[i+1,j,k+1] + v[i,j+1,k+1]) + d*v[i+1,j+1,k+1]

  return v_f


def V_cycle(b,nx,ny,nz,num_levels,u,f,level=1):
  '''
  V cycle
  '''
  if(level==num_levels):#bottom solve
    u,res=GSrelax(b,nx,ny,nz,u,f,iters=100)
    return u,res

  #Step 1: Relax Au=f on this grid
  u,res=GSrelax(b,nx,ny,nz,u,f,2)

  #Step 2: Restrict residual to coarse grid
  res_c=restrict(nx//2,ny//2,nz//2,res)
  b_c=restrict(nx//2,ny//2,nz//2,b)

  #Step 3:Solve A e_c=res_c on the coarse grid. (Recursively)
  e_c=np.zeros_like(res_c)
  e_c,res_c=V_cycle(b_c,nx//2,ny//2,nz//2,num_levels,e_c,res_c,level+1)

  #Step 4: Interpolate(prolong) e_c to fine grid and add to u
  u+=prolong(nx//2,ny//2,nz//2,e_c)

  #Step 5: Relax Au=f on this grid
  if(level==1):
    u,res=GSrelax(b,nx,ny,nz,u,f,2,flag=1)
  else:
    u,res=GSrelax(b,nx,ny,nz,u,f,2,flag=2)

  return u,res


def FMG(b,nx,ny,nz,num_levels,f,nv=1,level=1):

  if(level==num_levels):#bottom solve
    u=np.zeros([nx+2,ny+2,nz+2])
    u,res=GSrelax(b,nx,ny,nz,u,f,iters=100)
    return u,res

  #Step 1: Restrict the rhs to a coarse grid
  f_c=restrict(nx//2,ny//2,nz//2,f)
  b_c=restrict(nx//2,ny//2,nz//2,b)

  #Step 2: Solve the coarse grid problem using FMG
  u_c,_=FMG(b_c,nx//2,ny//2,nz//2,num_levels,f_c,nv,level+1)

  #Step 3: Interpolate u_c to the fine grid
  u=prolong(nx//2,ny//2,nz//2,u_c)

  #step 4: Execute 'nv' V-cycles
  for _ in range(nv):
    u,res=V_cycle(b,nx,ny,nz,num_levels-level,u,f)
  return u,res
