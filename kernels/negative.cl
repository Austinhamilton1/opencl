__kernel void negative(__global float *input_img, __global float *output_img) {
    uint rowid = get_global_id(0);
    uint colid = get_global_id(1);
    uint maxrows = get_global_size(0);
    uint maxcols = get_global_size(1);

    uint idx = 3 * (rowid * maxcols + colid);

    output_img[idx] = 1.0f - input_img[idx];
    output_img[idx+1] = 1.0f - input_img[idx+1];
    output_img[idx+2] = 1.0f - input_img[idx+2];
}