// /*
//
// Having a tree relationship between with levels and blocks makes some kind of sense.
// Everything multigrid is naturally tree-based,
// having a structure to traverse is great for cell-world and world-cell lookups,
// and to generate the linkages between ghosts.
// It's also easier to code the construction of meshes.
//
// However, traversing the tree takes jumps and is generally poorly optimized,
// especially on a GPU, where a jump stalls the warp.
// Once the trees are established on the heap, however, a simple list of indices
// can be constructed to traverse linearly.
//
// It might be possible to do away with the tree entirely, but
// trying to visualize 'neighbors' with arbitrarily placed blocks
// does my head in.
//
// It's important to note that this entire implementation completely disregards
// the cache altogether. The indice system means the next block could be halfway to Manitoba
// out of cache; there's no cache concurrency, congruency, or congealency; the
// &mesh argument means we're probably bringing 20x as much data along as we need for each function;
// etc.
//
// If one were truly smart, one could probably eek the same performance out of a
// homogenous computing system. I am not.
//
// //block_list
// //block_sizes
// //connections
//
// //
// //Ryzen 7: 0.25 TF/40 gbps. https://en.wikichip.org/wiki/amd/ryzen_7/1700
// //GTX 1060: 3 TFlops/160 GBps.
// //RTX 2070 Super: 8.2 TF/448.0 GBps - 21 to 51 TFlops half precision.
// //V100 hits 14TF/900 GBps + 100TF half.
// //https://en.wikipedia.org/wiki/List_of_Nvidia_graphics_processing_units
// //
// //Special thanks to http://ianfinlayson.net/class/cpsc425/notes/cuda-random!
// //
// */
//
// #include "device_data_structure.hpp"
// #include "host_data_structure.hpp"
//
// //
// //
// // #define CONSTRUCTOR_MACRO(TYPE, NAME, SIZE)     \
// //     TYPE * device_temp;         \
// //     gpu_error_check(cudaMalloc(&device_storage, (SIZE)*sizeof(* device_temp)));\
// //     gpu_error_check(cudaMemset(device_storage,0,(SIZE)*sizeof(* device_temp))); \//must memset the whole array!
// //     ;\ //copy pointer to array into struct
// //     gpu_error_check(cudaMemcpy(&((**device_struct).NAME), &device_temp, sizeof((**device_struct).NAME), cudaMemcpyHostToDevice));
// //
//
// void physics_mesh::device_constructor(physics_mesh ** device_struct){
//     //construct the struct itself
//     gpu_error_check(cudaMalloc(device_struct, sizeof(physics_mesh)));
//
// }
//
// void physics_mesh::copy_to_device_struct(test_struct ** device_struct, test_struct ** host_struct){
//     float * device_storage; //get the pointer from the device
//     gpu_error_check(cudaMemcpy(&device_storage, &((**device_struct).storage), sizeof(((**device_struct).storage)), cudaMemcpyDeviceToHost));
//     //and now copy the data.
//     gpu_error_check(cudaMemcpy(device_storage, (**host_struct).storage,  10*sizeof(* device_storage), cudaMemcpyHostToDevice));
//
//     //copy struct itself, wiping all the pointers,
//     gpu_error_check(cudaMemcpy(*device_struct, *host_struct, sizeof(test_struct), cudaMemcpyHostToDevice));
//
//     //then re-copy the pointers.
//     gpu_error_check(cudaMemcpy(&((**device_struct).storage), &device_storage, sizeof((**device_struct).storage), cudaMemcpyHostToDevice));
//     //There's a PCIe latency issue here, since we're going * -> host, data -> device,
//     //but whatever!
// }
//
// void physics_mesh::copy_to_host_struct(test_struct ** device_struct, test_struct ** host_struct){
//     float * device_temp;
//     //copy the pointer to the data
//     gpu_error_check(cudaMemcpy(&device_temp, &((**device_struct).storage), sizeof(((**device_struct).storage)), cudaMemcpyDeviceToHost));
//     //then the data itself
//     gpu_error_check(cudaMemcpy((**host_struct).storage, device_temp, 10*sizeof(* device_temp), cudaMemcpyDeviceToHost));
//
//     //save for after the wipe
//     float * host_temp = (**host_struct).storage;
//
//     //copy struct itself, wiping all the host pointers,
//     gpu_error_check(cudaMemcpy(*host_struct, *device_struct, sizeof(test_struct), cudaMemcpyDeviceToHost));
//
//     //must be done simultaneously!
//     (**host_struct).storage = host_storage;
// }
//
// void physics_mesh::destruct_device_struct(test_struct ** device_struct){
//     float * device_output_storage;
//     //copy the pointer to the data
//     gpu_error_check(cudaMemcpy(&device_output_storage, &((**device_struct).storage), sizeof(((**device_struct).storage)), cudaMemcpyDeviceToHost));
//     //then destroy it!
//     gpu_error_check(cudaFree(device_output_storage));
//
//
//     gpu_error_check(cudaFree(&(**device_struct)));
// }
// //
//
//
//
//
// void test_cuda(float * x){
// }
