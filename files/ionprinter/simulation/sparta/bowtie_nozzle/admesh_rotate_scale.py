import admesh

stl = admesh.Stl('nozzle.stl')

stl.repair()
stl.scale(0.001)
radius = 0.0003
stl.translate(0.001-radius,0.001-radius,0.0)
stl.write_ascii("nozzle-fixed.stl")

# load an STL file
stl = admesh.Stl('emitting_region.stl')
stl.repair()
stl.scale(0.001)
radius = 0.00007
stl.translate(0.001-radius,0.001-radius,0.00052)
stl.write_ascii("emitting_region-fixed.stl")
#
# import stl
#
# nozzle = stl.mesh.Mesh.from_file('nozzle.stl')
# nozzle.save('nozzle-fixed.stl',mode=stl.stl.ASCII)
#
# nozzle = stl.mesh.Mesh.from_file('emitting_region.stl')
# nozzle.save('emitting_region.stl',mode=stl.stl.ASCII)

# from pymesh import stl
# import pymesh
# m = stl.Stl("nozzle.stl")
# m.scale(1, 2, 1)
# m.save_stl("nozzle-fixed.stl", mode=pymesh.MODE_STL_ASCII)
