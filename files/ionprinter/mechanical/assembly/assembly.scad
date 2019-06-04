$fs = 0.1;
module round_square(x,y,z,r){
    translate([-x/2,-y/2]){
        hull(){
            translate([r/2,r/2,0]) cylinder(h=z,d=r);
            translate([-(r/2)+x,r/2,0]) cylinder(h=z,d=r);
            translate([(r/2),-(r/2)+y,0]) cylinder(h=z,d=r);
            translate([-(r/2)+x,-(r/2)+y,0]) cylinder(h=z,d=r);
        }
    }
}
module square_hole_pattern(dist,radius,height){
    translate([dist,0,0]) cylinder(h=height,d=radius);
    translate([-dist,0,0]) cylinder(h=height,d=radius);
    translate([dist,0,0]) cylinder(h=height,d=radius);
    translate([dist,0,0]) cylinder(h=height,d=radius);

}


front_plate_height = 4.0;
rod_hole_pattern = 20;
rod_hole_edge_distance = 3;
rod_hole_diameter = 0.1120*25.4;
chamfer_radius = 5;

difference(){
    round_square(25,12,front_plate_height,chamfer_radius);

}

