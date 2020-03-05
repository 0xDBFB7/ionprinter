#include "physics_mesh.hpp"
#include "physics.hpp"

TEST(physics, jacobi_point_source_potential_level_0){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {34};
    physics_mesh origin_host(mesh_sizes,1);


    origin_host.potential[idx()] = 1;

    physics_mesh * host_struct = &origin_host;
    physics_mesh * device_struct;
    physics_mesh::device_constructor(&device_struct);
    physics_mesh::copy_to_device(&device_struct, &host_struct);

    physics_mesh::device_jacobi_relax(host_struct, device_struct, &((*device_struct).potential), 100, 0);

    physics_mesh::copy_to_host(&device_struct, &host_struct);

    //compare potential across entire level to that of a point source
    int level = 0;
    traverse_state state;
    while(origin_host.breadth_first(state,level,level,true)){
        float x,y,z;
        state.cell_world_lookup(mesh, x, y, z);
        world_distance = norm3(x-0.5,y-0.5,z);

        //points too close to the object will have discretization errors
        if(world_distance > 0.1*ROOT_WORLD_SCALE){
            potential = 1/(distance);
            ASSERT_NEAR(mesh.potential[state.current_indice],potential);
        }
    }
}

TEST(physics, jacobi_point_source_electric_field_level_0){

}


#endif
