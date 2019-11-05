from shapely.geometry import box, MultiLineString, Point
from shapely.affinity import rotate
from shapely import speedups
from math import sqrt
import math
import json
from shapely import geometry
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import xml.etree.ElementTree
import numpy as np
from time import sleep

if speedups.available:
    speedups.enable()

def hatchbox(rect, angle, spacing):
    (llx, lly, urx, ury) = rect.bounds
    centre_x = (urx + llx) / 2
    centre_y = (ury + lly) / 2
    diagonal_length = sqrt((urx - llx) ** 2 + (ury - lly) ** 2)
    number_of_lines = 2 + int(diagonal_length / spacing)
    hatch_length = spacing * (number_of_lines - 1)
    coords = []
    for i in range(number_of_lines):
        if i % 2:
            coords.extend([((centre_x - hatch_length / 2, centre_y
                          - hatch_length / 2 + i * spacing), (centre_x
                          + hatch_length / 2, centre_y - hatch_length
                          / 2 + i * spacing))])
        else:
            coords.extend([((centre_x + hatch_length / 2, centre_y
                          - hatch_length / 2 + i * spacing), (centre_x
                          - hatch_length / 2, centre_y - hatch_length
                          / 2 + i * spacing))])
    lines = MultiLineString(coords)
    lines = rotate(lines, angle, origin='centroid', use_radians=False)
    return rect.intersection(lines)

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
            points.append({"z":z*1000000,"points":layer})

#Turn it into lines

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

lines = []

resolution = 0.5 #mm
depth = 60 #mm
center_x = 10
center_y = 10

page = box(0, 0, 25, 25)
hatching = hatchbox(page, 45, resolution)
for layer in points:
    lined_layer = []
    poly = geometry.Polygon(layer["points"])
    #x,y = poly.exterior.xy
    #plt.plot(x,y)
    #plt.show()
    hatch = poly.intersection(hatching)
    try:
        for i in hatch:
            x,y = i.xy
            for idx,val in enumerate(x):
                x[idx] = math.tan((center_x-val)/(depth-layer["z"]))
            for idx,val in enumerate(y):
                y[idx] = math.tan((center_y-val)/(depth-layer["z"]))
            lined_layer.append([x,y])
            #ax.plot(x,y,layer["z"])
    except:
        pass
    print(layer["z"])
    lines.append({"z":layer["z"],"lines":lined_layer})
#fig.show()
#sleep(100)
