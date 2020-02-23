

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


template <class TYPE>
void defenestrate_array_from_struct(TYPE ** array){
    TYPE * temp;
    /* //copy the pointer to the data */
    gpu_error_check(cudaMemcpy(&temp, array, sizeof(array), cudaMemcpyDeviceToHost));
    /* //then destroy it! */
    gpu_error_check(cudaFree(temp));
}
