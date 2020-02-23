
template <class TYPE> void constructificate_array_on_struct(TYPE ** array, uint32_t N);
template <class TYPE> void defenestrate_array_from_struct(TYPE ** array);

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
