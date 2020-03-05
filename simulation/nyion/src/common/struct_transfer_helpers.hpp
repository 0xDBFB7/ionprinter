#ifndef struct_transfer_helpers_H
#define struct_transfer_helpers_H

#include <stdint.h>
#include <CUDA_helpers.hpp>


template <class TYPE>
void constructificate_array_on_struct(TYPE ** array, uint32_t N){
    TYPE * device_temp;         \
    /* malloc an array on the device, */ \
    gpu_error_check(cudaMalloc(&device_temp, (N)*sizeof(* device_temp))); \
    /* memset the whole array, not just a buffer_end_pointer's worth */ \
    gpu_error_check(cudaMemset(device_temp,0,(N)*sizeof(* device_temp))); \
    /* then copy the pointer to the new array into the struct. */ \
    gpu_error_check(cudaMemcpy(array, &device_temp, sizeof(array), cudaMemcpyHostToDevice));
}




#define COPY_ARRAY_TO_DEVICE(TYPE, NAME, N)     \
    TYPE * NAME; \
    /* get the pointer from the device */ \
    gpu_error_check(cudaMemcpy(&NAME, &((**device_struct).NAME), sizeof(((**device_struct).NAME)), cudaMemcpyDeviceToHost)); \
    /* and now copy the data. */ \
    gpu_error_check(cudaMemcpy(NAME, (**host_struct).NAME,  (N)*sizeof(* NAME), cudaMemcpyHostToDevice));


#define COPY_POINTERS_BACK_TO_DEVICE(TYPE, NAME, N)     \
    gpu_error_check(cudaMemcpy(&((**device_struct).NAME), &NAME, sizeof((**device_struct).NAME), cudaMemcpyHostToDevice));




//this is friggin horrific. FIXME.
#define COPY_ARRAY_TO_HOST(TYPE, NAME, N) \
    TYPE * NAME##_;    \
    /* copy the pointer to the data */  \
    gpu_error_check(cudaMemcpy(&NAME##_, &((**device_struct).NAME), sizeof(((**device_struct).NAME)), cudaMemcpyDeviceToHost)); \
    /*  then the data itself */ \
    gpu_error_check(cudaMemcpy((**host_struct).NAME, NAME##_, N*sizeof(* NAME##_), cudaMemcpyDeviceToHost));

#define SAVE_HOST_ARRAY_POINTER(TYPE, NAME, N) \
    TYPE * NAME = (**host_struct).NAME;

#define RESTORE_HOST_ARRAY_POINTER(TYPE, NAME, N) \
    (**host_struct).NAME = NAME;




template <class TYPE>
void defenestrate_array_from_struct(TYPE ** array){
    TYPE * temp;
    /* //copy the pointer to the data */
    gpu_error_check(cudaMemcpy(&temp, array, sizeof(array), cudaMemcpyDeviceToHost));
    /* //then destroy it! */
    gpu_error_check(cudaFree(temp));
}




#endif
