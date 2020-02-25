#include <physics_mesh.hpp>
#include <traverse_state.hpp>
#include "state_persist.hpp"
/*

The data structures are of sufficient complexity that it's impossible to write test cases for them manually.
The array_helper_program allows one to diagram operations which are then serialized and saved with this program.
These are later loaded and compared for unit testing.

*/

void persist_to_file(std::vector<physics_mesh> &meshes, std::vector<traverse_state> &states, std::string filename){
    json meshes_json;
    for(auto & i: meshes){
        meshes_json.push_back(i.to_json_object());
    }
    json file_json;
    file_json["meshes"] = meshes_json;

    std::ofstream o(filename);
    o << std::setw(4) << file_json << std::endl;

}

void restore_from_file(std::vector<physics_mesh> &meshes, std::vector<traverse_state> &states, std::string filename){
    json file_json;

    std::ifstream i(filename);
    i >> file_json;

    json meshes_json = file_json["meshes"];

    for(auto & i: meshes_json){
        int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
        physics_mesh mesh(mesh_sizes, 1);
        mesh.from_json_object(i);
        meshes.push_back(mesh);
    }
}

// void read_from_file(){
