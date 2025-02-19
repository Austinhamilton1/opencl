__kernel void vadd(__global int *a, __global int *b, __global int *c) {
    size_t id = get_global_id(0);
    c[id] = a[id] + b[id];
}