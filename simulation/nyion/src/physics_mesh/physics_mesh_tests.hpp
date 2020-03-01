#include "nyion.hpp"

#include "gtest/gtest.h"

#ifndef DATA_STRUCTURE_TEST_H
#define DATA_STRUCTURE_TEST_H

#include "physics_mesh_unrolled_tests.hpp"

// https://softwareengineering.stackexchange.com/questions/379575/how-do-you-write-unit-tests-for-code-with-difficult-to-predict-results
// great resource


TEST(physics_mesh, refine_test){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh mesh(mesh_sizes,3);

    ASSERT_EQ(mesh.buffer_end_pointer,cube(3)); //root initialized by default
    int block_pointer = cube(3);

    mesh.refine_cell(0, 14); //refine level 0, position 14
    ASSERT_EQ(mesh.buffer_end_pointer,cube(3) + cube(5)); // we refine

    mesh.refine_cell(1, block_pointer);
    ASSERT_EQ(mesh.buffer_end_pointer,cube(3) + cube(5) + cube(5)); // we refine
}


TEST(physics_mesh, descend_test_1){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 3, 3};
    physics_mesh mesh(mesh_sizes,3);
    traverse_state state;

    mesh.refine_cell(0, 14); //refine level 0, position 14
    // state.descend_into();

    mesh.refine_cell(1, mesh.buffer_end_pointer-1);

}



TEST(physics_mesh, scale_test){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh mesh(mesh_sizes,3);

    ASSERT_NEAR(ROOT_WORLD_SCALE*(1/(3.0-2)), mesh.world_scale[0], 1e-5); //-2 because ghosts
    ASSERT_NEAR(ROOT_WORLD_SCALE*(1/(3.0-2))*(1/(5.0-2)), mesh.world_scale[1], 1e-5);
    ASSERT_NEAR(ROOT_WORLD_SCALE*(1/(3.0-2))*(1/(5.0-2))*(1/(5.0-2)), mesh.world_scale[2], 1e-5);

    ASSERT_EQ(mesh.mesh_sizes[0],3);
    ASSERT_EQ(mesh.mesh_sizes[1],5);
    ASSERT_EQ(mesh.mesh_sizes[2],5);
}

TEST(physics_mesh, breadth_first_1){
    //traverse with ghosts, root only.
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3};
    physics_mesh mesh(mesh_sizes,1);

    traverse_state state;

    while(mesh.breadth_first(state,0,1,false)){
        // state.pretty_print();
    }

}

TEST(physics_mesh, breadth_first_2){
    //traverse with ghosts, root only.
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3,3};
    physics_mesh mesh(mesh_sizes,2);

    mesh.refine_cell(0,14);
    std::vector<traverse_state> case_2;

    traverse_state state;

    while(mesh.breadth_first(state,0,0,false)){
        // state.pretty_print();
    }
}

TEST(physics_mesh, breadth_first_3){
    //traverse with ghosts, root only.
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3,3,3};
    physics_mesh mesh(mesh_sizes,3);

    mesh.refine_cell(0,14);
    mesh.refine_cell(1, cube(3));

    traverse_state state;

    while(mesh.breadth_first(state,0,MESH_BUFFER_DEPTH,false)){
        // state.pretty_print();
    }
}


TEST(physics_mesh, is_equal){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3,3};
    physics_mesh mesh(mesh_sizes,2);

    physics_mesh mesh_2(mesh_sizes,2);

    ASSERT_EQ(mesh.equals(mesh),true);
    ASSERT_EQ(mesh.equals(mesh_2),true);
    mesh_2.refined_indices[0] = 56;

    ASSERT_EQ(mesh.equals(mesh_2),false);
}


TEST(physics_mesh, serialization){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3,3};
    physics_mesh mesh(mesh_sizes,2);
    mesh.potential[0] = 500;
    // std::cout << mesh.serialize().dump(4);
    std::string output = mesh.to_json_object().dump(4);

    physics_mesh mesh_2(mesh_sizes,1);
    json j = json::parse(output);
    mesh_2.from_json_object(j);
    ASSERT_EQ(mesh_2.equals(mesh), true);
}


TEST(physics_mesh, ghost_linkages_plusx){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {4,5};
    physics_mesh mesh(mesh_sizes,2);

    mesh.refine_cell(0, 21); //refine two non-ghost blocks
    mesh.refine_cell(0, 22); //adjacent in +x

    traverse_state state;

    state.set_x(1); state.set_y(1); state.set_z(1); //block 0
    state.update_position(mesh);
    mesh.set_cell_ghost_linkages(state);

    //we're setting the +x face, so we must iterate over +y,+z

    ASSERT_EQ(mesh.ghost_linkages[98],220); //first non-corner ghost block on +x face on block 0
                                            //pointing at first real block on -x face on block 1
    ASSERT_EQ(mesh.ghost_linkages[158],280); //last non-corner ghost block on +x face on block 0
                                            //pointing at last real block on -x face on block 1

}


// TEST(cell_world_lookup, cell_world_lookup_test_3){
//   float x,y,z;
//
//   int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 3, 3};
//   traverse_state state;
//
//   cell_world_lookup(state, x, y, z);
//
//   ASSERT_NEAR(-ROOT_WORLD_SCALE, x, 1e-5);
//   ASSERT_NEAR(-ROOT_WORLD_SCALE, y, 1e-5);
//   ASSERT_NEAR(-ROOT_WORLD_SCALE, z, 1e-5);
//
//   xyz_traverse(state, mesh_sizes, 0);
//
// }


// TEST(cell_world_lookup, cell_world_lookup_test_5){
//   float x,y,z;
//   traverse_state state;
//   int mesh_sizes[MESH_BUFFER_DEPTH];
//   std::fill(mesh_sizes, mesh_sizes + MESH_BUFFER_DEPTH, 5);
//   init_state(state, mesh_sizes);
//
//   state.current_depth = 2;
//   state.x_queue[0] = 1;
//   state.x_queue[1] = 1;
//   state.x_queue[2] = 1;
//   cell_world_lookup(state, x, y, z);
//
//   ASSERT_NEAR(, x, 1e-5);
//   ASSERT_NEAR(-ROOT_WORLD_SCALE, y, 1e-5);
//   ASSERT_NEAR(-ROOT_WORLD_SCALE, z, 1e-5);
//
// }


//
// TEST(data_structure_mesh, traverse_benchmark){
//   int test_mesh_size = 10;
//
//   int mesh_sizes[MESH_BUFFER_DEPTH];
//   std::fill(mesh_sizes, mesh_sizes + MESH_BUFFER_DEPTH, test_mesh_size);
//
//
//   int * test = new int[MESH_BUFFER_SIZE];
//   int * refined_indices = new int[MESH_BUFFER_SIZE];
//   std::fill(test, test + MESH_BUFFER_SIZE, 0);
//   std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);
//
//
//   int heap_end = mesh_sizes[0]*mesh_sizes[0]*mesh_sizes[0];
//   for(int i = 0; i < pow(test_mesh_size,3); i++){
//     refined_indices[i] = heap_end;//center-middle
//     heap_end += pow(test_mesh_size,3);
//   }
//
//   traverse_state state;
//
//   auto start = std::chrono::high_resolution_clock::now();
//
//   for(init_state(state, mesh_sizes); breadth_first(state, refined_indices, MESH_BUFFER_DEPTH, 0, mesh_sizes); xyz_traverse(state, mesh_sizes, 0)){
//     test[state.current_indice] = state.current_indice-state.block_beginning_indice;
//   }
//
//   auto stop = std::chrono::high_resolution_clock::now();
//   auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//   std::cout << "Time taken by function: " << duration.count() << " microseconds\n";
//
//
//   delete [] test;
//   delete [] refined_indices;
// }
//
//
#endif
