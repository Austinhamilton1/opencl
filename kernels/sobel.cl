__kernel void gray(__global float *input_img, __global float *output_img) {
    //get the row and column of the current thread
    uint rowid = get_global_id(0);
    uint colid = get_global_id(1);
    uint maxrows = get_global_size(0);
    uint maxcols = get_global_size(1);

    //convert 2d r,g,b coordinate to 1d coordinate
    uint idx = rowid * maxcols + colid;
    uint input_idx = 3 * idx;

    //convert image to grayscale
    output_img[idx] = 0.3 * input_img[input_idx] + 0.59 * input_img[input_idx + 1] + 0.11 * input_img[input_idx + 2];
}

__kernel void sobel(__global float *input_img, __global float *output_img) {
    float Gx = 0.0;
    float Gy = 0.0;
    
    float x_conv[9] = { -1.0, 0.0, 1.0, -2.0, 0.0, 2.0, -1.0, 0.0, 1.0 };
    float y_conv[9] = { 1.0, 2.0, 1.0, 0.0, 0.0, 0.0, -1.0, -2.0, -1.0 };
    
    //get the row and column of the current thread
    uint rowid = get_global_id(0);
    uint colid = get_global_id(1);
    uint maxrows = get_global_size(0);
    uint maxcols = get_global_size(1);

    //dont convolve the last few pixels
    if(rowid < maxrows - 2 && colid < maxcols - 2) {
        //convert 2d image coordinate to 1d coordinate
        uint global_idx = rowid * maxcols + colid;

        //calculate the horizontal and vertical gradients for the image
        for(uint i = 0; i < 3; i++) {
            for(uint j = 0; j < 3; j++) {
                uint local_idx = i * 3 + j;
                uint global_offset = (rowid + i) * maxcols + (colid + j);
                Gx += x_conv[local_idx] * input_img[global_offset];
                Gy += y_conv[local_idx] * input_img[global_offset]; 
            }
        }

        //set the output_img pixel to the average gradient of the convolution
        output_img[global_idx] = sqrt(pown(Gx, 2) + pown(Gy, 2));
    }
}