
//internal_width
$fn = 40;
internal_length = 40;
height = 20;

mold_thickness = 1.5;

layer_num = 6;
layer_width = 1.5;
layer_spacing = 1.5;

nozzle_orfice = 3.2;


internal_radius = 10;


//difference(){
    //cylinder(h=10,d=65, center=true);

    union(){
        for (n = [0:layer_num] ){
            difference(){
                cylinder(h=height,r=internal_radius+(n*(layer_width+layer_spacing))+layer_width);
                cylinder(h=height,r=internal_radius+(n*(layer_width+layer_spacing)));
            }
        }
    }
  
    
//}
echo(2*(internal_radius+layer_num*(layer_width+layer_spacing)));

    