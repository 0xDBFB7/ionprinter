#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Simple_Counter.H>

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
//I need both state and mesh.
//What if I want to access a different widget's state?
//How do I pass which widget was changed - x,y,z? Nah dawg.
//oh hey, it'd be great if any arbitrary thing could be opened
//huh

struct menu_struct{
    Fl_Simple_Counter * x_counter;
    Fl_Simple_Counter * y_counter;
    Fl_Simple_Counter * z_counter;
    Fl_Simple_Counter * current_depth_counter;
    Fl_Simple_Counter * size_counter;
    Fl_Simple_Counter * mesh_depth_counter;
    Fl_Button * ;
    Fl_File_Chooser * file_input;
    Fl_Choice * array_menu;
    Fl_Float_Input * set_value;


    menu_struct(){
        const int counter_height = 100;
        const int text_height = 20;
        int window_x = 0;
        x_counter = new Fl_Simple_Counter(20,40,300,counter_height,"x"); window_x += counter_height + text_height;
        y_counter = new Fl_Simple_Counter(20,40,300,counter_height,"y"); window_x += counter_height + text_height;
        z_counter = new Fl_Simple_Counter(20,40,300,counter_height,"z"); window_x += counter_height + text_height;
        depth_counter = new Fl_Simple_Counter(window_x,40,300,counter_height,"current_depth"); window_x += counter_height + text_height;
        size_counter = new Fl_Simple_Counter(window_x,40,300,counter_height,"size"); window_x += counter_height + text_height;
        mesh_depth_counter = new Fl_Simple_Counter(window_x,40,300,counter_height,"mesh_depth"); window_x += counter_height + text_height;
        //FL_Input
    }
}

void init_inspect();

void inspect(traverse_state &system_state, physics_mesh &mesh, inspect_state ); //manual "step" button

void show_image(); //for MathGL

int main()
{

    initialize_opengl();
    opengl_3d_mode();

    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh mesh(mesh_sizes, 1);

    traverse_state user_state;

    bool level_splitting = true;

    Fl_Window * window = new Fl_Window(340,180);

    window->end();
    window->show();

    while(true){

        opengl_clear_screen();
        opengl_draw_axis_cross();

        draw_mesh(mesh, mesh.potential, level_splitting);
        draw_cell(mesh, user_state, 1,0,0,0.5, true, level_splitting);

        update_screen();

        Fl::wait(0.001);

    }

	return 0;
}
