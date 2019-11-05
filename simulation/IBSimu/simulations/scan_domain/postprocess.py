data = open('assembled_output.csv').read().split("\n\n\n")
data = [x.split('\n\n') for x in data]
data = [[y.split('\n') for x in data] for y in data]
print(len(data))
