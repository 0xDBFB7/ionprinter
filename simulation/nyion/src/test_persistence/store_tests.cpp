#include <nlohmann/json.hpp>

#include "host_data_structure.hpp"


using json = nlohmann::json;

void persist_to_file(){
    std::vector<physics_mesh> sub;

    //Push back new subject created with default constructor.
    sub.push_back(subject());


    //Add a new element if you want another:
    sub.push_back(subject());
}
// // read a JSON file
// std::ifstream i("file.json");
// json j;
// i >> j;
//
// // write prettified JSON to another file
// std::ofstream o("pretty.json");
// o << std::setw(4) << j << std::endl;
