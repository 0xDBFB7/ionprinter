#include "array_helper.hpp"
#include "visualize.hpp"


int main(int argc, char **argv)
{


  traverse_state user_state;

  initialize_opengl();
  opengl_3d_mode();

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


    //
    // opengl_clear_screen();
    // opengl_draw_axis_cross();
    //
    // traverse_state breadth_state;
    // while(breadth_first(breadth_state,)){
    //   glPushMatrix();
    //     float current_scale = breadth_state[breadth_state.current_depth];
    //     glTranslatef(breadth_state.x/, user_state.y, user_state.z,
    //
    //       );
    //     glColor4f(0,255.0,0,255);
    //
    //     glutWireCube((mesh_geometry.root_scale/sub_len)/OPENGL_SCALE);
    //   glPopMatrix();
    // }
    //
    // update_screen();

  }

	return 0;
}
