__kernel void gauss(float sigma, __global float *input_img, __global float *output_img) {
    //get the row and column of the current thread
    uint rowid = get_global_id(0);
    uint colid = get_global_id(1);
    uint maxrows = get_global_size(0);
    uint maxcols = get_global_size(1);

    int kernel_radius = ceil(3.0f * sigma);

    //dont convolve the edge pixels
    float rgb[3] = {0.0f, 0.0f, 0.0f};
    float weight_sum = 0.0f;

    //convert 2d image coordinate to 1d coordinate
    uint global_idx = 3 * (rowid * maxcols + colid);

    //calculate the total rgb values for neighboring pixels
    for(int i = -kernel_radius; i < kernel_radius; i++) {
        for(int j = -kernel_radius; j < kernel_radius; j++) {
            int x = rowid + i;
            int y = colid + j;

            if(x > 0 && x < maxrows && y > 0 && y < maxcols) {
                uint global_offset = 3 * (x * maxcols + y);

                float distance = (i * i + j * j) / (2.0f * sigma * sigma);
                float G_xy = exp(-distance) / (2.0f * M_PI * sigma * sigma);

                rgb[0] += G_xy * input_img[global_offset];
                rgb[1] += G_xy * input_img[global_offset+1];
                rgb[2] += G_xy * input_img[global_offset+2];

                weight_sum += G_xy;
            }
        }
    }

    if(weight_sum > 0.0f) {
        output_img[global_idx] = rgb[0] / weight_sum;
        output_img[global_idx+1] = rgb[1] / weight_sum;
        output_img[global_idx+2] = rgb[2] / weight_sum; 
    }
}