import json
import matplotlib.pyplot as plt

point_cloud = json.loads(open("./point_cloud.json","r").read())

for layer in point_cloud:
    x = []
    y = []
    print(layer)
    if(layer["points"]):
        for point in layer["points"]:
            print(point)
            x.append(point["x"])
            y.append(point["y"])
        plt.plot(x,y)
        plt.show()
