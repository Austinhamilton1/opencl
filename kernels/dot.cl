__kernel void dot(uint len, __global int *a, __global int *b, __global int *c) {
    __local int local_sum[256]; //assuming 256 work-items per group
    uint gid = get_global_id(0);
    uint lid = get_local_id(0);
    uint group_size = get_local_size(0);
    uint step = get_global_size(0);

    //each work-item computes a partial sum
    int sum = 0;
    for(uint i = gid; i < len; i += step) {
        sum += a[i] * b[i];
    }

    //store the partial sum in local memory
    local_sum[lid] = sum;
    barrier(CLK_LOCAL_MEM_FENCE);

    //reduce within the work-group
    for(uint s = group_size / 2; s > 0; s /= 2) {
        if(lid < s) {
            local_sum[lid] += local_sum[lid + s];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    //write the final result from work-group to global memory
    if(lid == 0) {
        atomic_add(c, local_sum[0]);
    }

}