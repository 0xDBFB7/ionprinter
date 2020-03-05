

// void planck_law()
// to get RGB colors from a black-body,
// integrate Planck's law over the wavelengths of r,g,b
// (ideally one should compensate for color space and eye response, but screw that)

float norm3(float x, float y, float z){
    return sqrt((x*x)+(y*y)+(z*z));
}
