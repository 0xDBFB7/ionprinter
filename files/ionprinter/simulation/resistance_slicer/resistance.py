
import os
import numpy as np
import numpy.linalg as la
import itertools
import utils
import ply
import scipy.spatial.distance as spdist
import stl
import meshcut
from shapely.geometry import Polygon, Point, LineString
import sys
import mayavi.mlab as mlab
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
##
def merge_close_vertices(verts, faces, close_epsilon=1e-5):
    """
    Will merge vertices that are closer than close_epsilon.
    Warning, this has a O(n^2) memory usage because we compute the full
    vert-to-vert distance matrix. If you have a large mesh, might want
    to use some kind of spatial search structure like an octree or some fancy
    hashing scheme
    Returns: new_verts, new_faces
    """
    # Pairwise distance between verts
    D = spdist.cdist(verts, verts)

    # Compute a mapping from old to new : for each input vert, store the index
    # of the new vert it will be merged into
    close_epsilon = 1e-5
    old2new = np.zeros(D.shape[0], dtype=np.int)
    # A mask indicating if a vertex has already been merged into another
    merged_verts = np.zeros(D.shape[0], dtype=np.bool)
    new_verts = []
    for i in range(D.shape[0]):
        if merged_verts[i]:
            continue
        else:
            # The vertices that will be merged into this one
            merged = np.flatnonzero(D[i, :] < close_epsilon)
            old2new[merged] = len(new_verts)
            new_verts.append(verts[i])
            merged_verts[merged] = True

    new_verts = np.array(new_verts)

    # Recompute face indices to index in new_verts
    new_faces = np.zeros((len(faces), 3), dtype=np.int)
    for i, f in enumerate(faces):
        new_faces[i] = (old2new[f[0]], old2new[f[1]], old2new[f[2]])

    # again, plot with utils.trimesh3d(new_verts, new_faces)
    return new_verts, new_faces


def load_stl(stl_fname):
    m = stl.mesh.Mesh.from_file(stl_fname)

    # Flatten our vert array to Nx3 and generate corresponding faces array
    verts = m.vectors.reshape(-1, 3)
    faces = np.arange(len(verts)).reshape(-1, 3)

    verts, faces = merge_close_vertices(verts, faces)
    return verts, faces



verts, faces = load_stl(sys.argv[1])

mesh = meshcut.TriangleMesh(verts, faces)

for x_slice in np.arange(0,10,0.1):
	plane_orig = (x_slice, 0, 0)
	plane_norm = (x_slice, 1, 0)
	plane = meshcut.Plane(plane_orig, plane_norm)
	plane_points = meshcut.cross_section_mesh(mesh, plane)
	print(LineString(plane_points))

	print(plane_points)

