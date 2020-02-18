#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include "array_helper.hpp"
#include "visualize.hpp"

//
// struct complete_state{ //for use with callbacks only
//
// }
//
// void fill_array(Fl_Widget *w, traverse_state *user_state) {
//     std::fill()
// }

//We really should use callbacks.
//However, consider:
//I need
//What if I want to access a different widget's state?

int main()
{

    initialize_opengl();
    opengl_3d_mode();

    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh mesh(mesh_sizes, 1);

    traverse_state user_state;

    bool level_splitting = true;

    Fl_Window *window = new Fl_Window(340,180);

    Fl_Box *button = new Fl_Button(20,40,300,100,"Hello, World!");
    window->end();
    window->show();

    while(true){

        opengl_clear_screen();
        opengl_draw_axis_cross();

        draw_mesh(mesh, mesh.potential, level_splitting);

        update_screen();

        Fl::wait();

    }

	return 0;
}
