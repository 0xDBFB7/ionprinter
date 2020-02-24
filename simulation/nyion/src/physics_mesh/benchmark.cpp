
// TEST(benchmark, CUDA_physics_mesh_copy_benchmark){
//     int mesh_sizes[MESH_BUFFER_DEPTH] = {100};
//     physics_mesh origin_host(mesh_sizes, 1);
//     physics_mesh * host_struct = &origin_host;
//
//     physics_mesh * device_struct;
//
//     physics_mesh::device_constructor(&device_struct);
//
//     auto start = std::chrono::high_resolution_clock::now();
//
//     physics_mesh::copy_to_device(&device_struct, &host_struct);
//
//     physics_mesh::copy_to_host(&device_struct, &host_struct);
//
//     cudaDeviceSynchronize();
//
//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::microseconds>( end-start ).count();
//     std::cout << duration << " us, " << "\n";
//
//     physics_mesh::device_destructor(&device_struct);
// }
