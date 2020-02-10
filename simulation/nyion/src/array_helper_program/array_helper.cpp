#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include "array_helper.hpp"
#include "visualize.hpp"

void menu(traverse_state &user_state, int * refined_indices, int (&mesh_sizes)[MESH_BUFFER_DEPTH]){


    //x to select a new cell within mesh,
    //d to go down a level,
    //s to set cell to a value,
    //f to fill block with a value,
    //b


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

  initialize_opengl();
  opengl_3d_mode();

  int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
  physics_mesh mesh(mesh_sizes, 3);

  traverse_state user_state;

  // bool level_splitting = false;

  while(true){

    opengl_clear_screen();
    opengl_draw_axis_cross();

    // draw_mesh(mesh, mesh.potential, level_splitting);

    update_screen();

    // menu(user_state,refined_indices,mesh_sizes);

    // std::cout << "Enter x to select a new cell, "
    // std::cout << ;
    // std::cout << "\n> ";
    // while(true){

    Fl::wait();

  }

	return 0;
}
