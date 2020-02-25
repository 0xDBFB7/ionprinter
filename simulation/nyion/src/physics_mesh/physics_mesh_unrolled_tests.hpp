#ifndef physics_mesh_unrolled_tests_TEST_H
#define physics_mesh_unrolled_tests_TEST_H


TEST(physics_mesh, block_depth_lookup_1){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh mesh(mesh_sizes,3);

    //matching digraph
    ASSERT_EQ(mesh.blocks_on_level(0), 1);
    ASSERT_EQ(mesh.blocks_on_level(1), 0);

    mesh.block_list_insert(1, 8);
    // mesh.pretty_print();
    ASSERT_EQ(mesh.block_depth_lookup[0], 0);
    ASSERT_EQ(mesh.block_depth_lookup[1], 1);
    ASSERT_EQ(mesh.block_depth_lookup[2], 2);
    ASSERT_EQ(mesh.block_depth_lookup[2], 2);

    mesh.block_list_insert(1, 16);
    mesh.block_list_insert(1, 24);
    mesh.block_list_insert(2, 32);

    // mesh.pretty_print();

    ASSERT_EQ(mesh.blocks_on_level(0), 1);
    ASSERT_EQ(mesh.blocks_on_level(1), 3);
    ASSERT_EQ(mesh.blocks_on_level(2), 1);

    ASSERT_EQ(mesh.block_depth_lookup[0], 0);
    ASSERT_EQ(mesh.block_depth_lookup[1], 1);
    ASSERT_EQ(mesh.block_depth_lookup[2], 4);
    ASSERT_EQ(mesh.block_depth_lookup[3], 5);

    ASSERT_EQ(mesh.block_indices[0], 0);
    ASSERT_EQ(mesh.block_indices[1], 24); //order is irrelevant within levels
    ASSERT_EQ(mesh.block_indices[2], 16);
    ASSERT_EQ(mesh.block_indices[3], 8);
    ASSERT_EQ(mesh.block_indices[4], 32);
}

#endif
