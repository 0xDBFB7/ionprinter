


for(i = [0:NUM_CONES]){
    translate([0,0,-i*(cone_thickness+(cone_gap/sin(cone_angle)))]){
        difference(){
        cylinder(cone_height,d1=CONE_OD_1,d2=CONE_OD_2,center=true);
        cylinder(cone_height,d1=CONE_OD_1-cone_thickness,
                                            d2=CONE_OD_2-cone_thickness,center=true); 
                                            
        }
    }
}