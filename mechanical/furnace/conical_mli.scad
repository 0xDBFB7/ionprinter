$fn = 30;

CONE_OD_1 = 75;
CONE_OD_2 = 50;

cone_thickness = 0.5;

NUM_CONES  = 3;

cone_height = 115;
cone_gap = 1;

cone_angle = asin(((CONE_OD_1 - CONE_OD_2)/2)/cone_height);
echo(cone_angle);
base_correction = sin(cone_angle)*cone_thickness;

echo(cone_height-NUM_CONES*(cone_thickness+(cone_gap/sin(cone_angle))));


//intersection(){
   // cylinder(h=cone_height-NUM_CONES*(cone_thickness+(cone_gap/sin(cone_angle))),d=CONE_OD_1);
    for(i = [1:NUM_CONES]){
        translate([0,0,-i*(cone_thickness+(cone_gap/sin(cone_angle)))]){
            translate([0,0,cone_height]){cylinder(cone_thickness,d1=CONE_OD_2);}
            difference(){
                
            cylinder(cone_height,d1=CONE_OD_1,d2=CONE_OD_2);
            cylinder(cone_height,d1=CONE_OD_1-cone_thickness,
                                                d2=CONE_OD_2-cone_thickness); 
                                                
            }
        }
    }
//}

translate([0,0,-5]){
    translate([0,0,0]){cube([31.5,12.56,30], center=true);}
    translate([0,0,15+30]){cube([26,6,60], center=true);}
}

translate([0,0,-20]){cylinder(cone_thickness,d1=65);}
translate([0,0,-22]){cylinder(cone_thickness,d1=65);}
translate([0,0,-24]){cylinder(cone_thickness,d1=70);}