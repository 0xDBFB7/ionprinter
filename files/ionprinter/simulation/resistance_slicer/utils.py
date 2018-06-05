import numpy as np
import numpy.linalg as la


def points3d(verts, point_size=3, **kwargs):
    if 'mode' not in kwargs:
        kwargs['mode'] = 'point'
    p = mlab.points3d(verts[:, 0], verts[:, 1], verts[:, 2], **kwargs)
    p.actor.property.point_size = point_size


def trimesh3d(verts, faces, **kwargs):
    mlab.triangular_mesh(verts[:, 0], verts[:, 1], verts[:, 2], faces,
                         **kwargs)


def orthogonal_vector(v):
    """Return an arbitrary vector that is orthogonal to v"""
    if v[1] != 0 or v[2] != 0:
        c = (1, 0, 0)
    else:
        c = (0, 1, 0)
    return np.cross(v, c)

def find_mins_maxs(obj):
    minx = maxx = miny = maxy = minz = maxz = None
    for p in obj.points:
        # p contains (x, y, z)
        if minx is None:
            minx = p[stl.Dimension.X]
            maxx = p[stl.Dimension.X]
            miny = p[stl.Dimension.Y]
            maxy = p[stl.Dimension.Y]
            minz = p[stl.Dimension.Z]
            maxz = p[stl.Dimension.Z]
        else:
            maxx = max(p[stl.Dimension.X], maxx)
            minx = min(p[stl.Dimension.X], minx)
            maxy = max(p[stl.Dimension.Y], maxy)
            miny = min(p[stl.Dimension.Y], miny)
            maxz = max(p[stl.Dimension.Z], maxz)
            minz = min(p[stl.Dimension.Z], minz)
    return minx, maxx, miny, maxy, minz, maxz
