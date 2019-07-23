#include <vector>

float value_plus_z(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, int root_z_len, bool &valid){
  if(s_z == sub_len-1){ // if at top of submesh bounds
    if(r_z < root_z_len-1){ //if below top of root mesh
      valid = true;
      return input_mesh[idx(r_x,r_y,r_z+1,root_x_len,root_y_len)][idx(s_x,s_y,0,sub_len,sub_len)];
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

float value_minus_z(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid){
  if(s_z == 0){ // if at top of submesh bounds
    if(r_z > 0){ //if below top of root mesh
      valid = true;
      return input_mesh[idx(r_x,r_y,r_z-1,root_x_len,root_y_len)][idx(s_x,s_y,sub_len-1,sub_len,sub_len)];
    }
    else{
      valid = false;
      return 0;
    }
  }
  else{
    valid = true;
    return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x,s_y,s_z-1,sub_len,sub_len)];
  }
}

float value_plus_y(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid){
  if(s_y == sub_len-1){ // if at top of submesh bounds
    if(r_y < root_y_len-1){ //if below top of root mesh
      valid = true;
      return input_mesh[idx(r_x,r_y+1,r_z,root_x_len,root_y_len)][idx(s_x,0,s_z,sub_len,sub_len)];
    }
    else{
      valid = false;
      return 0;
    }
  }
  else{
    valid = true;
    return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x,s_y+1,s_z,sub_len,sub_len)];
  }
}

float value_minus_y(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid){
  if(s_y == 0){ // if at top of submesh bounds
    if(r_y > 0){ //if below top of root mesh
      valid = true;
      return input_mesh[idx(r_x,r_y-1,r_z,root_x_len,root_y_len)][idx(s_x,sub_len-1,s_z,sub_len,sub_len)];
    }
    else{
      valid = false;
      return 0;
    }
  }
  else{
    valid = true;
    return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x,s_y-1,s_z,sub_len,sub_len)];
  }
}

float value_plus_x(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid){
  if(s_x == sub_len-1){ // if at top of submesh bounds
    if(r_x < root_x_len-1){ //if below top of root mesh
      valid = true;
      return input_mesh[idx(r_x+1,r_y,r_z,root_x_len,root_y_len)][idx(0,s_y,s_z,sub_len,sub_len)];
    }
    else{
      valid = false;
      return 0;
    }
  }
  else{
    valid = true;
    return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x+1,s_y,s_z,sub_len,sub_len)];
  }
}

float value_minus_x(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid){
  if(s_x == 0){ // if at top of submesh bounds
    if(r_x > 0){ //if below top of root mesh
      valid = true;
      return input_mesh[idx(r_x-1,r_y,r_z,root_x_len,root_y_len)][idx(sub_len-1,s_y,s_z,sub_len,sub_len)];
    }
    else{
      valid = false;
      return 0;
    }
  }
  else{
    valid = true;
    return input_mesh[idx(r_x,r_y,r_z,root_x_len,root_y_len)][idx(s_x-1,s_y,s_z,sub_len,sub_len)];
  }
}
