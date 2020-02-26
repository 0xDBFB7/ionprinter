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

#include "linenoise.hpp"

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

    }
    //
    // void handle_inputs(traverse_state &user_state){
    //     current_depth_counter =
    //     x_counter =
    // }

};


void show_image(); //for MathGL


void init_inspect(){
    initialize_opengl();
    opengl_3d_mode();
}

void gui_loop(physics_mesh &mesh, traverse_state &user_state, bool level_splitting, bool single_loop){
    if(!single_loop){ std::cout << "\n GUI unlocked \n"; };
    while(true){
        opengl_clear_screen();
        opengl_draw_axis_cross();

        draw_mesh(mesh, mesh.potential, level_splitting);
        draw_cell(mesh, user_state, 1,0,0,0.5, true, level_splitting);

        update_screen();

        Fl::wait(0.001);
        if(escape_hit() || single_loop){ //this is the dumbest thing... https://github.com/daniele77/cli is async by default...
            std::cout << "\n\n";
            break;
        }
    }
}


void inspect(traverse_state &state, physics_mesh &mesh){ //manual "step" button
    gui_loop(mesh, state, false, true);
    std::cin.get(); //wait for input
}


void move_cursor(physics_mesh &mesh, traverse_state &user_state, std::vector<std::string> &args){
    int action = (args[2] == "+") ? 1 : -1;
    if(args[1] == "x"){
        if((action == 1 && user_state.x < mesh.mesh_sizes[user_state.current_depth]-1)
                                                        || (user_state.x > 0 && action == -1)){
            user_state.x_queue[user_state.current_depth] += action;
            user_state.x += action;
        }
    }
    if(args[1] == "y"){
        if((action == 1 && user_state.y < mesh.mesh_sizes[user_state.current_depth]-1)
                                                        || (user_state.y > 0 && action == -1)){
            user_state.y_queue[user_state.current_depth] += action;
            user_state.y += action;
        }
    }
    if(args[1] == "z"){
        if((action == 1 && user_state.z < mesh.mesh_sizes[user_state.current_depth]-1)
                                                        || (user_state.z > 0 && action == -1)){
            user_state.z_queue[user_state.current_depth] += action;
            user_state.z += action;
        }
    }

    user_state.update_position(mesh);
    linenoise::linenoiseClearScreen();
    user_state.pretty_print();
}

int main()
{

    // menu_struct menu;

    initialize_opengl();
    opengl_3d_mode();

    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 10, 10};
    physics_mesh mesh(mesh_sizes, 1);

    traverse_state user_state;

    bool level_splitting = false;

    // linenoise::SetCompletionCallback([](const char* editBuffer, std::vector<std::string>& completions) {
    //     if (editBuffer[0] == 'h') {
    //         completions.push_back("hello");
    //         completions.push_back("hello there");
    //     }
    // });

    gui_loop(mesh, user_state, level_splitting,true);
    gui_loop(mesh, user_state, level_splitting,true); //required for stable start for unknown reason


    while(true){



        std::string line;

        auto quit = linenoise::Readline("ds > ", line);

        if (quit) {
            break;
        }

        std::istringstream iss(line);
        std::vector<std::string> args{ std::istream_iterator<std::string>(iss), {}};

        linenoise::AddHistory(line.c_str());

        if(args.size() > 0){
            if(args[0] == "move" && args.size() == 3){
                move_cursor(mesh, user_state, args);
            }
            if(args[0] == "info" && args.size() == 2){
                if(args[1] == "mesh"){
                    mesh.pretty_print();
                }
                if(args[1] == "state"){
                    user_state.pretty_print();
                }
            }
            if(args[0] == "refine"){
                mesh.refine_cell(user_state.current_depth,user_state.current_indice);
                                        //how about a wrapper state.x() that just returns user_state.x_queue[user_state.current_depth]?

                mesh.pretty_print();

            }
            if(args[0] == "descend"){
                user_state.descend_into(mesh, false);
            }
            if(args[0] == "set" && args.size() == 3){
                if(args[1] == "potential"){
                    mesh.potential[user_state.current_indice] = std::stof(args[2]);
                    mesh.pretty_print();
                }
            }
        }
        else{
            gui_loop(mesh, user_state, level_splitting,false);
        }

        gui_loop(mesh, user_state, level_splitting,true);
    }

	return 0;
}
