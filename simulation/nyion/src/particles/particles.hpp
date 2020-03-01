//
// #define PARTICLE_TYPE double
//
// const int MAX_TIMESTEPS = 3; //includes root
//
// struct particles{
//     //SoA
//     PARTICLE_TYPE * x;
//     PARTICLE_TYPE * y;
//     PARTICLE_TYPE * z;
//
//     PARTICLE_TYPE * v_x;
//     PARTICLE_TYPE * v_y;
//     PARTICLE_TYPE * v_z;
//

//these array pointers could be stored in a seperate struct, particle_data,
//with a second struct for mesh_parameters.
//mesh_data on the host could be ~300 timesteps long, and
//~3 timesteps long on the device, or whatever RK4 needs.
//only the latest timestep is copied device->host using a pointer offset.

//     uint16_t * particle_category;
//
//     uint32_t num_particles[timesteps]; //heap
//     //a lookup
//     //lookup(number, timestep);
//     //timestep*num_particles[timestep] + number?
//
//      store as many timesteps as are required, then copy only the current timestep
//      using a pointer offset
// }
