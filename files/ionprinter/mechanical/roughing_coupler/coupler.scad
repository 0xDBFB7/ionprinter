$fn=60; 

outer_diameter = 33;
inner_diameter = 18.5;
height = 41;
spoke_depth = 3.5;
spoke_width = 3.8;
difference(){
    difference(){
    cylinder(height,d=outer_diameter);
    cylinder(height,d=inner_diameter);
    }

    for(angle=[0:45:360]){
        rotate([0,0,angle]){
            translate([(inner_diameter/2)+(spoke_depth/2)-2,0,height/2]){
                cube([spoke_depth+2,spoke_width,height],center=true);
            }
        }
    }
}