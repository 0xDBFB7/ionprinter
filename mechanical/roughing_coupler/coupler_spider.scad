$fn=60; 

outer_diameter = 33;
inner_diameter = 18.5;
height = 41;
spoke_depth = 4.3;
spoke_width = 4.6;
difference(){
    difference(){
        difference(){
        cylinder(height*(2/3),d=outer_diameter);
        cylinder(height*(2/3),d=inner_diameter);
        }

        for(angle=[0:45:360]){
            rotate([0,0,angle]){
                translate([(inner_diameter/2)+(spoke_depth/2)-2,0,height/2]){
                    cube([spoke_depth+2,spoke_width,height],center=true);
                }
            }
        }
    }
    
    translate([-1,-1,height*(1/3)]){
        cube([outer_diameter*2,outer_diameter*2,height*(1/3)]);
    }
    rotate([0,0,180]){
        translate([-1,-1,height*(1/3)]){
        cube([outer_diameter*2,outer_diameter*2,height*(1/3)]);
        }
    }
}