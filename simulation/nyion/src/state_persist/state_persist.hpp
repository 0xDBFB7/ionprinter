#ifndef STATE_PERSIST_H
#define STATE_PERSIST_H
//
#include <nyion.hpp>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

void persist_to_file(std::vector<physics_mesh> &meshes, std::vector<traverse_state> &states, std::string filename);
void restore_from_file(std::vector<physics_mesh> &meshes, std::vector<traverse_state> &states, std::string filename);


#endif
