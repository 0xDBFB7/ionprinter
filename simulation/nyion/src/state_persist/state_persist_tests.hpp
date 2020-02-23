#include "nyion.hpp"

#include "gtest/gtest.h"

#ifndef STORE_TESTS_TEST_H
#define STORE_TESTS_TEST_H


TEST(statepersist, statepersisttest1){ //because googletest

    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh mesh_1(mesh_sizes, 1);

    std::vector<physics_mesh> meshes;
    sub.push_back(mesh_1);

    // traverse_state state;
    // std::vector<traverse_state> states;
    // sub.push_back(state);

    

}


#endif
