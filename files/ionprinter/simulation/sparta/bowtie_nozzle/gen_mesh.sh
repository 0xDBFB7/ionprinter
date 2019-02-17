cp ../../../mechanical/bowtie/nozzle.stl nozzle.stl
#../tools/convertSTL.rb nozzle.stl
python admesh_rotate_scale.py
python ../tools/stl2surf.py nozzle-fixed.stl nozzle.surf
