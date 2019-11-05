cp ../../../mechanical/bowtie/nozzle.stl nozzle.stl
cp ../../../mechanical/bowtie/emitting_region.stl emitting_region.stl

#../tools/convertSTL.rb nozzle.stl
python admesh_rotate_scale.py
python ../tools/stl2surf.py nozzle-fixed.stl nozzle.surf
python ../tools/stl2surf.py emitting_region-fixed.stl emitting_region.surf
