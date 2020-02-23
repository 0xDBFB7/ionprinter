#include <physics_mesh.hpp>
#include <traverse_state.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/*

The data structures are of sufficient complexity that it's impossible to write test cases for them manually.
The array_helper_program allows one to diagram operations which are then serialized and saved with this program.
These are later loaded and compared for unit testing.

*/

void persist_to_file(std::vector<physics_mesh> &meshes, std::vector<traverse_state> &states, std::string filename){
    // std::vector<physics_mesh> sub;
    // sub.push_back();

    // // read a JSON file
    // std::ifstream i("file.json");
    // json j;
    // i >> j;
    //
    // // write prettified JSON to another file
    // std::ofstream o(filename);
    // o << std::setw(4) << j << std::endl;

}


// void read_from_file(){
