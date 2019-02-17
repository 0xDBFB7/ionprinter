import admesh

# load an STL file
stl = admesh.Stl('nozzle.stl')

stl.scale()
stl.rotate_x()
# see how many facets are there
len(stl)
