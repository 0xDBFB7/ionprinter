#include <vector>
#include "multiphysics.hpp"

int idx(int x, int y, int z, int x_len, int y_len){
  return ((x_len*y_len*z) + (x_len*y) + x);
}

int root_idx(int x, int y, int z, root_mesh_geometry mesh_geometry){
  return ((mesh_geometry.root_x_len*mesh_geometry.root_y_len*z) + (mesh_geometry.root_x_len*y) + x);
}


float value_plus_z(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, int root_z_len, bool &valid){
  if(s_z == sub_len-1){
    int root_i = idx(r_x,r_y,r_z+1,root_x_len,root_y_len);
    if(r_z < root_z_len-1 && input_mesh[root_i].size()){
      float size_scalar = ((float)submesh_side_length(input_mesh[root_i]))/sub_len;
      valid = true;
      printf("%i,%i,%i,%i,%f,%i\n",idx(r_x,r_y,r_z,root_x_len,root_y_len),root_i,(int)(s_x*size_scalar),(int)(s_y*size_scalar),size_scalar,submesh_side_length(input_mesh[root_i]));
      return input_mesh[root_i][idx((int)(s_x*size_scalar),(int)(s_y*size_scalar),0,(sub_len*size_scalar),(sub_len*size_scalar))];
    }
    else{
      valid = false;
      return 0;
    }
  }
  else{
    valid = true;
    return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x,s_y,s_z+1,sub_len,sub_len)];
  }
}


//
// float value_plus_y(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
//                                                             int s_x, int s_y, int s_z,
//                                                             int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid){
//   if(s_y == sub_len-1){ // if at top of submesh bounds
//     if(r_y < root_y_len-1){ //if below top of root mesh
//       valid = true;
//       return input_mesh[idx(r_x,r_y+1,r_z,root_x_len,root_y_len)][idx(s_x,0,s_z,sub_len,sub_len)];
//     }
//     else{
//       valid = false;
//       return 0;
//     }
//   }
//   else{
//     valid = true;
//     return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x,s_y+1,s_z,sub_len,sub_len)];
//   }
// }
//

// float value_plus_x(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
//                                                             int s_x, int s_y, int s_z,
//                                                             int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid){
//   if(s_x == sub_len-1){ // if at top of submesh bounds
//     if(r_x < root_x_len-1){ //if below top of root mesh
//       valid = true;
//       return input_mesh[idx(r_x+1,r_y,r_z,root_x_len,root_y_len)][idx(0,s_y,s_z,sub_len,sub_len)];
//     }
//     else{
//       valid = false;
//       return 0;
//     }
//   }
//   else{
//     valid = true;
//     return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x+1,s_y,s_z,sub_len,sub_len)];
//   }
// }
//
// float value_minus_x(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
//                                                             int s_x, int s_y, int s_z,
//                                                             int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid){
//   if(s_x == 0){ // if at top of submesh bounds
//     if(r_x > 0){ //if below top of root mesh
//       valid = true;
//       return input_mesh[idx(r_x-1,r_y,r_z,root_x_len,root_y_len)][idx(sub_len-1,s_y,s_z,sub_len,sub_len)];
//     }
//     else{
//       valid = false;
//       return 0;
//     }
//   }
//   else{
//     valid = true;
//     return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x-1,s_y,s_z,sub_len,sub_len)];
//   }
// }
//
// float value_minus_y(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
//                                                             int s_x, int s_y, int s_z,
//                                                             int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid){
//   if(s_y == 0){ // if at top of submesh bounds
//     if(r_y > 0){ //if below top of root mesh
//       valid = true;
//       return input_mesh[idx(r_x,r_y-1,r_z,root_x_len,root_y_len)][idx(s_x,sub_len-1,s_z,sub_len,sub_len)];
//     }
//     else{
//       valid = false;
//       return 0;
//     }
//   }
//   else{
//     valid = true;
//     return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x,s_y-1,s_z,sub_len,sub_len)];
//   }
// }
//
// float value_minus_z(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
//                                                             int s_x, int s_y, int s_z,
//                                                             int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid){
//   if(s_z == 0){ // if at top of submesh bounds
//     if(r_z > 0){ //if below top of root mesh
//       valid = true;
//       return input_mesh[idx(r_x,r_y,r_z-1,root_x_len,root_y_len)][idx(s_x,s_y,sub_len-1,sub_len,sub_len)];
//     }
//     else{
//       valid = false;
//       return 0;
//     }
//   }
//   else{
//     valid = true;
//     return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x,s_y,s_z-1,sub_len,sub_len)];
//   }
// }
