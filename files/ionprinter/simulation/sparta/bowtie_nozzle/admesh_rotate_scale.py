import admesh

# load an STL file
stl = admesh.Stl('nozzle.stl')

stl.scale(0.001)
stl.translate(0,0,0)
# stl.repair()
stl.write_ascii("nozzle-fixed.stl")

# load an STL file
stl = admesh.Stl('emitting_region.stl')
stl.scale(0.001)
stl.translate(0.001-0.0001,0.001-0.0001,0.002)
# stl.repair()
stl.write_ascii("emitting_region-fixed.stl")
#
# import stl
#
# nozzle = stl.mesh.Mesh.from_file('nozzle.stl')
# nozzle.save('nozzle-fixed.stl',mode=stl.stl.ASCII)
#
# nozzle = stl.mesh.Mesh.from_file('emitting_region.stl')
# nozzle.transform()
# nozzle.save('emitting_region.stl',mode=stl.stl.ASCII)
