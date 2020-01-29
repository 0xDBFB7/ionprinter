#include "array_helper.hpp"
#include "visualize.hpp"



void menu(traverse_state &user_state, int * refined_indices, int (&mesh_sizes)[MAX_DEPTH]){

  std::cout << "\n> ";
  char menu;
  if(std::cin >> menu){
    if(menu == 'x'){
      if (std::cin >> user_state.x >> user_state.y >> user_state.z) {
        user_state.current_indice = user_state.block_beginning_indice+idx(user_state.x,user_state.y,user_state.z,mesh_sizes[user_state.current_depth]);
        // array[current_indice] =
      }
    }
  }
  else {
      std::cout << "Couldn't parse input. \n";
  }
}


int main()
{


  traverse_state user_state;

  initialize_opengl();
  opengl_3d_mode();

  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, 3);
  mesh_sizes[1] = 5;
  init_state(user_state,mesh_sizes);

  float * potentials = new float[MESH_BUFFER_SIZE];
  int * refined_indices = new int[MESH_BUFFER_SIZE];
  std::fill(potentials, potentials + MESH_BUFFER_SIZE, 0);
  std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);

  bool level_splitting = false;

  int heap_end = mesh_sizes[0]*mesh_sizes[0]*mesh_sizes[0]+1;

  refined_indices[13] = heap_end;

  while(true){

    opengl_clear_screen();
    opengl_draw_axis_cross();

    draw_mesh(potentials, refined_indices, mesh_sizes, level_splitting);

    update_screen();

    // menu(user_state,refined_indices,mesh_sizes);

    //x to select a new cell within mesh,
    //d to go down a level,
    //s to set cell to a value,
    //f to fill block with a value,
    //b

    // std::cout << "Enter x to select a new cell, "
    // std::cout << ;
    // std::cout << "\n> ";
    // while(true){



  }

	return 0;
}
