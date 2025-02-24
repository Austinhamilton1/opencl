__kernel void gauss(float sigma, __global float *input_img, __global float *output_img) {
    //get the row and column of the current thread
    uint rowid = get_global_id(0);
    uint colid = get_global_id(1);
    uint maxrows = get_global_size(0);
    uint maxcols = get_global_size(1);

    //dont convolve the edge pixels
    if(rowid < maxrows - 4 && colid < maxcols - 4) {
        float rgb[3];
        float weight_sum = 273.0f;
        float conv[25] = { 
            1.0f, 4.0f, 7.0f, 4.0f, 1.0f, 
            4.0f, 16.0f, 26.0f, 16.0f, 4.0f, 
            7.0f, 26.0f, 41.0f, 26.0f, 7.0f, 
            4.0f, 16.0f, 26.0f, 16.0f, 4.0f, 
            1.0f, 4.0f, 7.0f, 4.0f, 1.0f
        };

        //convert 2d image coordinate to 1d coordinate
        uint global_idx = 3 * (rowid * maxcols + colid);

        //calculate the total rgb values for neighboring pixels
        for(uint i = 0; i < 5; i++) {
            for(uint j = 0; j < 5; j++) {
                uint local_idx = i * 5 + j;
                uint global_offset = (rowid + i) * maxcols + (colid + j);
                rgb[0] += conv[local_idx] * input_img[global_offset];
                rgb[1] += conv[local_idx] * input_img[global_offset+1];
                rgb[2] += conv[local_idx] * input_img[global_offset+2];
            }
        }

        output_img[global_idx] = rgb[0] / weight_sum;
        output_img[global_idx+1] = rgb[1] / weight_sum;
        output_img[global_idx+2] = rgb[2] / weight_sum;          
    }
}