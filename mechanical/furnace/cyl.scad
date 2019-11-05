$fn = 40;

difference(){
    union(){
       cylinder(h=10,d=15, center=true);
     }
     union(){
        translate([0,0,-5]) cylinder(h=5,d=12);
        translate([0,0,0]) cylinder(h=5,d=3.8);
     }
}