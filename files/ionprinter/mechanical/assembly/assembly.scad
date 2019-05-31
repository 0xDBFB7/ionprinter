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


round_square(25,12,4,5);


