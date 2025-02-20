__kernel void dot(uint len, __global int *a, __global int *b, __global int *c) {
    uint start = get_global_id(0);
    uint step = get_global_size(0);
    int sum = 0;
    for(uint i = start; i < len; i += step) {
        sum += a[i] * b[i];
    }
    c[start] = sum;
}