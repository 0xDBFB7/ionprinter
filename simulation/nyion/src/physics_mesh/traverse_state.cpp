#include "traverse_state.hpp"

traverse_state::traverse_state(){
    current_depth = 0;
    block_beginning_indice = 0;
    current_indice = 0;
    started_traverse = true;

    for(int i = 0; i < MESH_BUFFER_DEPTH; i++){
        x_queue[i] = 0;
        y_queue[i] = 0;
        z_queue[i] = 0;
        ref_queue[i] = 0;
    }
}



bool traverse_state::equals(traverse_state &state_2, int depth){
    bool e_s = true;

    e_s = e_s && (current_depth == state_2.current_depth);
    e_s = e_s && (block_beginning_indice == state_2.block_beginning_indice);
    e_s = e_s && (current_indice == state_2.current_indice);
    e_s = e_s && (get_x() == state_2.get_x());
    e_s = e_s && (get_y() == state_2.get_y());
    e_s = e_s && (get_z() == state_2.get_z());

    for(int i = 0; i < MESH_BUFFER_DEPTH; i++){
        e_s = e_s && (x_queue[i] == state_2.x_queue[i]);
        e_s = e_s && (y_queue[i] == state_2.y_queue[i]);
        e_s = e_s && (z_queue[i] == state_2.z_queue[i]);
        e_s = e_s && (ref_queue[i] == state_2.ref_queue[i]);
    }

    return e_s;
}

void traverse_state::pretty_print(){
    std::cout << "\n\033[1;32mtraverse_state: \033[0m {\n";

    named_value(current_depth);
    named_value(current_indice);
    named_value(block_beginning_indice);
    named_array(x_queue,MESH_BUFFER_DEPTH);
    named_array(y_queue,MESH_BUFFER_DEPTH);
    named_array(z_queue,MESH_BUFFER_DEPTH);
    named_array(ref_queue,MESH_BUFFER_DEPTH);

    std::cout << "}\n";
}


bool traverse_state::is_ghost(physics_mesh &mesh){
  if(get_x() == 0 || get_y() == 0 || get_z() == 0
      || get_x() == mesh.mesh_sizes[current_depth]-1
      || get_y() == mesh.mesh_sizes[current_depth]-1
      || get_z() == mesh.mesh_sizes[current_depth]-1){
        return true;
  }
  else{
    return false;
  }
}


void traverse_state::cell_world_lookup(physics_mesh &mesh, float &x, float &y, float &z){
  x = 0;
  y = 0;
  z = 0;
  for(int i = 0; i < ((current_depth+1)) && (i < MESH_BUFFER_DEPTH); i++){
    x += mesh.world_scale[i]*(x_queue[i]-1); //ghost offset
    y += mesh.world_scale[i]*(y_queue[i]-1);
    z += mesh.world_scale[i]*(z_queue[i]-1);
  }
}

//
// int world_cell_index_lookup(float x, float y, float z, int max_depth){
//   int x_,y_,z_;
//   for(int depth = 0, depth < max_depth; depth++){
//       x_ = (int)floor(x/(mesh_scale[depth]));
//       y_ = (int)floor(y/(mesh_scale[depth]));
//       z_ = (int)floor(z/(mesh_scale[depth])); //remember ghosts!
//
//       x -= x_*mesh_scale[depth]
//   }
// }

void traverse_state::descend_into(physics_mesh &mesh, bool ignore_ghosts){
    ref_queue[current_depth] = block_beginning_indice;
    current_depth += 1;
    ref_queue[current_depth] = mesh.refined_indices[current_indice];
    block_beginning_indice = mesh.refined_indices[current_indice];
    set_x(ignore_ghosts);
    set_y(ignore_ghosts);
    set_z(ignore_ghosts);
    update_position(mesh);

}

void traverse_state::ascend_from(physics_mesh &mesh, bool ignore_ghosts){
    current_depth-=1;
    update_position(mesh);
}

void traverse_state::update_position(physics_mesh &mesh){
    block_beginning_indice = ref_queue[current_depth];

    current_indice = block_beginning_indice+
                    idx(get_x(),get_y(),get_z(),mesh.mesh_sizes[current_depth]);

}

bool physics_mesh::breadth_first(traverse_state &state, int start_depth, int end_depth, int ignore_ghosts){

    /*
    A traverse through all the cells of all the blocks at a specified level.
    'Breadth first' is perhaps a bit misleading; more precisely it's depth first then breadth across
    depth
    Iterates through the x,y,z values of a block,
    ref_queue stores block indices of parent blocks.

    g o o o o o... g
    ^
    block_beginning_indice

    */

    while(true){
        if(state.started_traverse){
            state.set_x(ignore_ghosts);
            state.set_y(ignore_ghosts);
            state.set_z(ignore_ghosts);
            state.started_traverse=false;
        }
        else{
            state.x_queue[state.current_depth]++;
        }

        //ensure that we don't start in the corner if ghosts are to be ignored.

        if(state.get_x() == (mesh_sizes[state.current_depth]-ignore_ghosts)) {state.set_x(ignore_ghosts); state.y_queue[state.current_depth]++;}
        if(state.get_y() == (mesh_sizes[state.current_depth]-ignore_ghosts)) {state.set_y(ignore_ghosts); state.z_queue[state.current_depth]++;}

        // if(state.z < mesh_sizes[state.current_depth]){
        //     state.x_queue[state.current_depth] = state.x;
        //     state.y_queue[state.current_depth] = state.y;
        //     state.z_queue[state.current_depth] = state.z;
        // }

        bool just_visited = 0;

        while(true){

            state.update_position(*this);

          if(state.current_depth < end_depth && refined_indices[state.current_indice]
                                && !just_visited && state.get_z() < mesh_sizes[state.current_depth]){
              //Descend
              state.descend_into(*this, ignore_ghosts);
              continue;
          }

          if(state.get_z() == (mesh_sizes[state.current_depth]-ignore_ghosts)){

              if(state.current_depth == 0){
                  return false;
              }
              state.set_x(0); //not strictly necessary, just cleans up a bit
              state.set_y(0);
              state.set_z(0);
              state.current_depth-=1;

              just_visited = true;
              continue;
          }

          break;
        }

        if(state.current_depth >= start_depth && state.current_depth <= end_depth){
            return true;
        }
        else{
            continue;
        }
    }
}


//avoids complexity in updating x,y,z values constantly
//though it does add one level of pointer indirection for every update,
//rather than just occasionally.
int traverse_state::get_x(){
    return x_queue[current_depth];
}
int traverse_state::get_y(){
    return y_queue[current_depth];
}
int traverse_state::get_z(){
    return z_queue[current_depth];
}
void traverse_state::set_x(int i){
    x_queue[current_depth] = i;
}
void traverse_state::set_y(int i){
    y_queue[current_depth] = i;
}
void traverse_state::set_z(int i){
    z_queue[current_depth] = i;
}
