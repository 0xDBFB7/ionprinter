#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Spinner.H>

#include <linenoise.hpp>

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
    Fl_Spinner * x_counter;
    Fl_Spinner * y_counter;
    Fl_Spinner * z_counter;
    Fl_Spinner * step_counter;
    Fl_Spinner * current_depth_counter;
    Fl_Spinner * size_counter;
    Fl_Spinner * mesh_depth_counter;
    Fl_Check_Button * level_splitting;
    Fl_Button * read_button;
    Fl_Button * write_button;
    Fl_Input * file_input;
    Fl_Choice * array_menu;
    Fl_Float_Input * set_value;
    Fl_Window * window;

    menu_struct(){
        window = new Fl_Window(500,500);

        const int counter_height = 20;
        const int counter_width = 100;

        const int text_height = 20;
        int widget_x = 20;
        int widget_y = 40;

        x_counter = new Fl_Spinner(widget_x,widget_y,counter_width,counter_height,"x"); widget_y += counter_height + text_height;
        y_counter = new Fl_Spinner(widget_x,widget_y,counter_width,counter_height,"y"); widget_y += counter_height + text_height;
        z_counter = new Fl_Spinner(widget_x,widget_y,counter_width,counter_height,"z"); widget_y += counter_height + text_height;
        current_depth_counter = new Fl_Spinner(widget_x,widget_y,counter_width,counter_height,"current_depth"); widget_y += counter_height + text_height;
        size_counter = new Fl_Spinner(widget_x,widget_y,counter_width,counter_height,"size"); widget_y += counter_height + text_height;
        mesh_depth_counter = new Fl_Spinner(widget_x,widget_y,counter_width,counter_height,"mesh_depth"); widget_y += counter_height + text_height;
        step_counter = new Fl_Spinner(widget_x,widget_y,counter_width,counter_height,"step"); widget_y += counter_height + text_height;

        window->end();
        window->show();

        initialize_opengl();
        opengl_3d_mode();

    }
    //
    // void handle_inputs(traverse_state &user_state){
    //     current_depth_counter =
    //     x_counter =
    // }

    void inspect(traverse_state &system_state, physics_mesh &mesh); //manual "step" button
};

void init_inspect();


void show_image(); //for MathGL

int main()
{

    menu_struct menu;

    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh mesh(mesh_sizes, 1);

    traverse_state user_state;

    bool level_splitting = true;



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
