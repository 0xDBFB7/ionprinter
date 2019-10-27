//internal_width
$fn = 40;
internal_length = 40;
height = 20;

mold_thickness = 1.5;

layer_num = 6;
layer_width = 2;
spacing = 1.5;

nozzle_orfice = 3.2;

cylinder(h=10,d=65, center=true);



difference(){
    difference(){
       cube([mold_thickness*2+internal_length,((layer_num*(layer_width+spacing))+layer_width+2*mold_thickness),(height+mold_thickness)]);
       
     for (y = [0:layer_num] )
         {translate([mold_thickness,mold_thickness+y*(layer_width+spacing),0])cube([internal_length,layer_width,height]);}
    }
    translate([(mold_thickness*2+internal_length)/2,((layer_num*(layer_width+spacing))+3*mold_thickness)/2,height-10]) //extend a bit into the mold to allow for the nozzle length
            cylinder(h=mold_thickness+10,d=nozzle_orfice);
}

    