//module rotate_about_pt(z, y, pt) {
//    translate(pt)
//        rotate([0, y, z])
//            translate(-pt)
//                children();   
//}

//internal_width
$fn = 40;
internal_length = 40;
height = 20;

mold_thickness = 1.5;

layer_num = 4;
layer_width = 1.75;
layer_spacing = 1.75;

nozzle_orfice = 6.8;


internal_radius = 20;

mounting_hole_radius = 2.9/2;

num_spokes = 4;
 //4-40, 2.9 mm

difference(){
    union(){
        difference(){
            translate([0,0,0]) cylinder(h=height+mold_thickness,d=2*(internal_radius+layer_num*(layer_width+layer_spacing))+6);
            cylinder(h=height+mold_thickness,r=internal_radius-mold_thickness);
        }
     }
    
    translate([0,0,mold_thickness]){
        difference(){ //subtract mounting holes
                
            union(){
                for (n = [0:layer_num]){
                    difference(){ //the MLI ring itself
                        cylinder(h=height,r=internal_radius+(n*(layer_width+layer_spacing))+layer_width);
                        cylinder(h=height,r=internal_radius+(n*(layer_width+layer_spacing)));
                    }
                    
                    if(n > 0){ //the spokes
                        for (sp = [0:num_spokes-1]){
                            rotate([0,0,sp*(360/num_spokes)+(n % 2)*((360/num_spokes)/2)])
                                translate([internal_radius+(n*(layer_width+layer_spacing))-layer_width/2,0,height*3/4]) 
                                    cube([layer_width+0.3,layer_width,height/2],center=true);
                        }
                    }
                    
                    if(n == layer_num){ //the mounting tabs
                        for (sp = [0:num_spokes-1]){
                            rotate([0,0,sp*(360/num_spokes)+(n % 2)*((360/num_spokes)/2)])
                                translate([internal_radius+(n*(layer_width+layer_spacing))-layer_width/2,0,height/2]) 
                                    cylinder(h=height,r=mounting_hole_radius*2,center=true);
                        }
                    }
                }
            }
        
           union(){
                for (sp = [0:num_spokes-1]){
                            rotate([0,0,sp*(360/num_spokes)+(layer_num % 2)*((360/num_spokes)/2)])
                                translate([internal_radius+(layer_num*(layer_width+layer_spacing))-layer_width/2,0,height/2]) 
                                    cylinder(h=height*2,r=mounting_hole_radius,center=true);
                }
            }     
        }
   
    }  
}
echo(2*(internal_radius+layer_num*(layer_width+layer_spacing)));

    