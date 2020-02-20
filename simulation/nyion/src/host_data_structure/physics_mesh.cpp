
void physics_mesh::refine_cell(int current_depth, int current_indice){
    assert("Tried to refine beyond acceptable depth." && current_depth+1 < mesh_depth);
    refined_indices[current_indice] = buffer_end_pointer;
    // block_indices[block_num] = buffer_end_pointer; //figure this out!

    buffer_end_pointer += cube(mesh_sizes[current_depth+1]);
}


__host__ void physics_mesh::compute_world_scale(){  //must be called if mesh_depth is changed
    for(int i = 0; i < MESH_BUFFER_DEPTH; i++){ world_scale[i] = 0; };
    // pre-compute scales
    float scale = ROOT_WORLD_SCALE;
    for(int i = 0; i < mesh_depth; i++){
        assert("Mesh size must be > 2" && mesh_sizes[i]-2 > 0);
        scale /= mesh_sizes[i]-2; //-2 compensates for ghost points.
        world_scale[i] = scale;
    } // TODO: Scales must be re-computed if the size changes!
}


//constructor
__host__ physics_mesh::physics_mesh(int (&set_mesh_sizes)[MESH_BUFFER_DEPTH], int new_mesh_depth){
    //set scales and sizes
    assert("Increase MESH_BUFFER_DEPTH" && MESH_BUFFER_DEPTH >= new_mesh_depth);
    mesh_depth = new_mesh_depth;

    for(int i = 0; i < MESH_BUFFER_DEPTH; i++){ mesh_sizes[i] = set_mesh_sizes[i]; };
    for(int i = 0; i < MESH_BUFFER_DEPTH; i++){ block_num[i] = 0; };

    compute_world_scale();

    //on construction, initialize root
    buffer_end_pointer = cube(mesh_sizes[0]);

    //and allocate memory
    temperature = new float[MESH_BUFFER_SIZE];
    potential = new float[MESH_BUFFER_SIZE];
    space_charge = new int32_t[MESH_BUFFER_SIZE];
    boundary_conditions = new uint16_t[MESH_BUFFER_SIZE];
    refined_indices = new uint32_t[MESH_BUFFER_SIZE];
    ghost_linkages = new uint32_t[MESH_BUFFER_SIZE];
    block_indices = new uint32_t[MESH_BUFFER_SIZE];//max blocks?

    //std::fill not available on GPU.
    for(int i = 0; i < MESH_BUFFER_SIZE; i++){
        temperature[i] = 0;
        potential[i] = 0;
        space_charge[i] = 0; //a canary (perhaps -inf?) might be useful
        boundary_conditions[i] = 0;
        refined_indices[i] = 0;
        ghost_linkages[i] = 0;
        block_indices[i] = 0;
    }
}

void physics_mesh::copy_to_gpu();
//first copy struct; constructor never runs on device?
// Then malloc and memcpy temperature...?
// create device-only destructor with cudaFree?

// void descend_into(){
//     state.ref_queue[state.current_depth] = refined_indices[state.current_indice];
//     user_state.current_depth++; //descend_into() function?
//     user_state.x_queue[user_state.current_depth] = 0; //state.x,y,z should go.
//     user_state.z_queue[user_state.current_depth] = 0; //everything has to be updated simultaneously anyhow,
//     user_state.y_queue[user_state.current_depth] = 0; //and state. should never be in the hot loop anyhow.
// }

physics_mesh::~physics_mesh(){
        //on destruction,
        if(!device_only){
            delete [] temperature;
            delete [] potential;
            delete [] space_charge;
            delete [] boundary_conditions;
            delete [] refined_indices;
            delete [] ghost_linkages;
            delete [] block_indices;
        }
}

__host__ void pretty_print(){
    std::cout << "\n\033[1;32mtraverse_state: \033[0m {\n";

    named_array(world_scale,MESH_BUFFER_DEPTH);
    named_array(mesh_sizes,MESH_BUFFER_DEPTH);
    named_value(mesh_depth);

    named_array(temperature,buffer_end_pointer);
    named_array(potential, buffer_end_pointer);
    // float * potential;
    // int32_t * space_charge; //charge probably can't reasonably be fractional - we're not working with quarks?
    // uint16_t * boundary_conditions;
    // uint32_t * refined_indices;
    // uint32_t * ghost_linkages; // can't include ghosts at 'overhangs' - those'll be handled by 'copy_down' I suppose?
    //                            // - just those on the same level, which'll be changed every iteration.
    //                            // - could also have 6 pointers to blocks up/down/left/right
    //                            // I suppose
    //
    // uint32_t * block_indices; //an unrolled list of pointers to the beginnings of blocks
    //                         //needed for fast traversal
    //                         //must be in ascending order of level - 0,->1,->1,->1,->1,->2,->2,->2,0,0...
    // uint32_t block_num[MESH_BUFFER_DEPTH]; //1,4,3,0 //including root
    //we need both block_indices and refined_indices:
    //one provides the spatial data, and one the fast vectorized traverse

    // uint32_t buffer_end_pointer;

    std::cout << "}\n";
}
