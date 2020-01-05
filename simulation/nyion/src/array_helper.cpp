#include "array_helper.hpp"
#include "visualize.hpp"


int main(int argc, char **argv)
{


  traverse_state user_state;

  initialize_opengl();
  opengl_3d_mode();

  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, 3);
  init_state(user_state,mesh_sizes);

  int * potentials = new int[MESH_BUFFER_SIZE];
  int * refined_indices = new int[MESH_BUFFER_SIZE];
  std::fill(potentials, potentials + MESH_BUFFER_SIZE, 0);
  std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);

  bool level_splitting = true;// render meshes with an offset corresponding to their level

  while(true){

    // while(true){
    //   if (std::cin >> user_state.x >> user_state.y >> user_state.z) {
    //     update_idx(user_state,);
    //     // array[current_indice] =
    //   }
    //   else {
    //       std::cout << "Couldn't parse input. \n";
    //   }
    // }


    traverse_state t_state;

    opengl_clear_screen();
    opengl_draw_axis_cross();

    for(init_state(t_state, mesh_sizes); breadth_first(t_state, refined_indices, MAX_DEPTH, 0, mesh_sizes); xyz_traverse(t_state, mesh_sizes, 0)){
      glPushMatrix();
        float current_scale = t_state.world_scale[t_state.current_depth];

        float level_split_offset = 0;
        if(t_state.current_depth){
          level_split_offset = (level_splitting * t_state.world_scale[t_state.current_depth-1]*1.5);
        }

        float gl_x = ((t_state.x-1)*current_scale)*OPENGL_SCALE + level_split_offset;
        float gl_y = ((t_state.y-1)*current_scale)*OPENGL_SCALE + level_split_offset;
        float gl_z = ((t_state.z-1)*current_scale)*OPENGL_SCALE + level_split_offset;

        glTranslatef(gl_x, gl_y, gl_z);
        if(is_ghost(t_state,mesh_sizes)){
          glColor4f(0,255.0,0,255);
        }
        else{
          glColor4f(255.0,0,0,255);

        }

        glutWireCube(current_scale*OPENGL_SCALE);
      glPopMatrix();
    }

    update_screen();

  }

	return 0;
}
