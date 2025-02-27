__kernel void gray(__global float *input_img, __global float *output_img) {
    //get the row and column of the current thread
    uint rowid = get_global_id(0);
    uint colid = get_global_id(1);
    uint maxrows = get_global_size(0);
    uint maxcols = get_global_size(1);

    //convert 2d r,g,b coordinate to 1d coordinate
    uint idx = 3 * (rowid * maxcols + colid);

    //convert image to grayscale
    float gray_val = 0.3 * input_img[idx] + 0.59 * input_img[idx+1] + 0.11 * input_img[idx+2];
    output_img[idx] = gray_val;
    output_img[idx+1] = gray_val;
    output_img[idx+2] = gray_val;
}