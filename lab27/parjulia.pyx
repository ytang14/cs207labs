#distutils: extra_compile_args = -fopenmp
#distutils: extra_link_args = -fopenmp
cimport cython
import numpy as np
cimport numpy as np
from cython.parallel cimport prange

cdef inline double norm2(double complex z) nogil:
    return z.real*z.real + z.imag*z.imag

cdef int escape(int maxiter, double complex z, double complex c) nogil:
    cdef int n=0
    while n < maxiter and norm2(z) < 4:
        z=z*z+c
        n+=1
    return n

@cython.boundscheck(False)
@cython.wraparound(False)
cpdef run_cython_amalgamated2(int gridsize, box, double complex c, int maxiter):
    #your code here
    cdef int i,j
    cdef double startx = box.x1, starty = box.y1, endx = box.x2, endy = box.y2
    cdef unsigned int n
    cdef double xinit = 0., yinit = 0., x = 0., y = 0.
    cdef double complex z
    cdef double complex[:,:] zs = np.zeros((gridsize, gridsize), dtype = np.complex128)
    cdef int[:,:] output = np.empty_like(zs,dtype=np.int32)
    cdef double xtick = (endx - startx)/np.float((gridsize - 1))
    cdef double ytick = (endy - starty)/np.float((gridsize - 1))
    
    for i in prange(gridsize, nogil=True,schedule='static', chunksize=1):
        x = xinit + i * xtick
        for j in range(gridsize):
            y = yinit + j * ytick
            z = x + j * y
            zs[i,j] = z
            output[i,j] = escape(maxiter, z, c)
    return output

