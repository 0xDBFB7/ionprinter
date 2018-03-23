from shapely.geometry import box, MultiLineString, Point
from shapely.affinity import rotate
from shapely import speedups
from math import sqrt
import json
from shapely import geometry
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import xml.etree.ElementTree
import numpy as np
# enable Shapely speedups, if possible
if speedups.available:
    speedups.enable()

# def hatchbox(rect, angle, spacing):
#
#     (llx, lly, urx, ury) = rect.bounds
#     centre_x = (urx + llx) / 2
#     centre_y = (ury + lly) / 2
#     diagonal_length = sqrt((urx - llx) ** 2 + (ury - lly) ** 2)
#     number_of_lines = 2 + int(diagonal_length / spacing)
#     hatch_length = spacing * (number_of_lines - 1)
#
#     # build a square (of side hatch_length) horizontal lines
#     # centred on centroid of the bounding box, 'spacing' units apart
#     coords = []
#     for i in range(number_of_lines):
#         if i % 2:
#             coords.extend([((centre_x - hatch_length / 2, centre_y
#                           - hatch_length / 2 + i * spacing), (centre_x
#                           + hatch_length / 2, centre_y - hatch_length
#                           / 2 + i * spacing))])
#         else:
#             coords.extend([((centre_x + hatch_length / 2, centre_y
#                           - hatch_length / 2 + i * spacing), (centre_x
#                           - hatch_length / 2, centre_y - hatch_length
#                           / 2 + i * spacing))])
#     # turn array into Shapely object
#     lines = MultiLineString(coords)
#     # Rotate by angle around box centre
#     lines = rotate(lines, angle, origin='centroid', use_radians=False)
#     # return clipped array
#     return rect.intersection(lines)

# point_cloud = json.loads(open("./point_cloud.json","r").read())

e = xml.etree.ElementTree.parse('./svg/5mm_Calibration_Steps.svg').getroot()
points = []
z = 0.0
for child in e.findall("{http://www.w3.org/2000/svg}g"):
    z = float(child.get("{http://slic3r.org/namespaces/slic3r}z"))
    for i in child:
        if(i.get('points')):
            layer = []
            point = i.get('points').split(' ')
            for coord in point:
                floats = coord.split(',')
                layer.append([float(m) for m in floats])
            points.append({"z":z,"points":layer})
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# final_points = []
#
# for layer in points:
#     poly = geometry.Polygon(layer["points"])
#     for point_x in np.arange(0,30,1):
#         for point_y in np.arange(0,30,1):
#             if(poly.contains(Point(point_x,point_y))):
#                 ax.scatter(point_x, point_y, layer["z"])
#                 print(point_x)
# plt.show()

for layer in points:
    poly = geometry.Polygon(layer["points"])

#
#
# for layer in point_cloud:
#     index = []
#     if(layer["points"]):
#         for point in layer["points"]:
#             index.append([point["x"],point["y"]])
#         poly = geometry.Polygon(index)
#         x,y = poly.exterior.xy
#         fig = plt.figure()
#         ax = fig.add_subplot(111, projection='3d')
#         ax.scatter(x, y, 0)
#         plt.show()
#         hatch = poly.intersection(hatching)
#         fig = plt.figure()
#         ax = fig.add_subplot(111, projection='3d')
#         ax.scatter(x, y, 0)
#         plt.show()
