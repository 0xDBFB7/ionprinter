import scipy as sp
import numpy as np
import scipy.sparse as sparse
import scipy.sparse.linalg as sla
import matplotlib.pyplot as plt

def hnorm(r):
    """define ||r||_h = h ||r||_2"""
    n = len(r)
    h = 1.0 / (n+1)
    hrnorm = h * np.linalg.norm(r)
    return hrnorm

def poissonop(n):
    """
    Poisson operator h^{-2} * [-1 2 1]
    """
    A = (n+1)**2 * sparse.diags([-1, 2, -1], [-1, 0, 1], shape=(n,n), format='csr')
    return A

def residual(u, f):
    """
    f - A u
    """
    n = len(f)
    r = np.zeros(len(u))
    r[1:-1] = f[1:-1] - ((n+1)**2) * (2 * u[1:-1] - u[2:] - u[:-2])
    r[0]    = f[0]    - ((n+1)**2) * (2 * u[0]    - u[1])
    r[-1]   = f[-1]   - ((n+1)**2) * (2 * u[-1]   - u[-2])
    return r

def relax(u, f, nu):
    """
    Weighted Jacobi
    """
    n = len(u)
    Dinv = 1.0 / (2.0 * ((n+1)**2))
    omega = 2.0 / 3.0
    unew = u.copy()

    for steps in range(nu):
        unew = unew + omega * Dinv * residual(unew, f)

    return unew

def interpolate(uc):
    """interpolate u of size 2**(k-1)-1 to 2**(k)-1"""
    uf = np.zeros((2*len(uc) + 1,))
    uf[:-1:2] = 0.5 * uc
    uf[1::2] = uc
    uf[2::2] += 0.5 * uc
    return uf

def restrict(uf):
    """restrict u of size 2**(k)-1 to 2**(k-1)-1"""
    uc = 0.25 * uf[:-1:2] + 0.5 * uf[1::2] + 0.25 * uf[2::2]
    return uc




def vcycle(kmax, kmin, u, f, nu):
    ulist = [None for k in range(kmax+1)]
    flist = [None for k in range(kmax+1)]

    print('grid: ', end=' ')
    # down cycle
    for k in range(kmax, kmin, -1):
        print(k, end=' ')
        u = relax(u, f, nu)
        ulist[k] = u
        flist[k] = f

        f = restrict(residual(u, f))
        u = np.zeros(f.shape)
        ulist[k-1] = u
        flist[k-1] = f

    # coarsest grid
    print(kmin, end=' ')
    Ac = poissonop(2**kmin - 1)
    flist[kmin] = f
    ulist[kmin] = sla.spsolve(Ac, f)

    # up cycle
    for k in range(kmin+1, kmax+1, 1):
        print(k, end=' ')
        u = ulist[k]
        f = flist[k]
        uc = ulist[k-1]
        u += interpolate(uc)
        u = relax(u, f, nu)
    print('.')
    return u


kmax = 7
kmin = 2

# set up fine problem
n = 2**kmax - 1

xx = np.linspace(0, 1, n+2)[1:-1]

f = 2 - 12 * xx**2

ustar = xx**4 - xx**2
A = poissonop(len(f))
udstar = sla.spsolve(A, f)
print(hnorm(ustar - udstar))
u = np.random.rand(len(f))

# set up smoothing sweeps
nu = 2

res = []
err = []

res.append(hnorm(residual(u, f)))
for i in range(12):
    u = vcycle(kmax, kmin, u, f, nu)
    #u = vcycle3(u, f, nu)
    res.append(hnorm(residual(u, f)))
    err.append(hnorm(u - ustar))
print(res)
