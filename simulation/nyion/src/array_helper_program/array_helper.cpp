#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include "array_helper.hpp"
#include "visualize.hpp"

// void menu(traverse_state &user_state){
// }


int main()
{

  initialize_opengl();
  opengl_3d_mode();

  int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
  physics_mesh mesh(mesh_sizes, 1);

  traverse_state user_state;

  bool level_splitting = true;

  while(true){

    opengl_clear_screen();
    opengl_draw_axis_cross();

    draw_mesh(mesh, mesh.potential, level_splitting);

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
