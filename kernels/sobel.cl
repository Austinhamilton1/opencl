__kernel void sobel(__global float *input_img, __global float *output_img) {
    float Gx[3] = {0.0f, 0.0f, 0.0f};
    float Gy[3] = {0.0f, 0.0f, 0.0f};
    
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
        uint global_idx = 3 * (rowid * maxcols + colid);

        //calculate the horizontal and vertical gradients for the image
        for(uint i = 0; i < 3; i++) {
            for(uint j = 0; j < 3; j++) {
                uint local_idx = i * 3 + j;
                uint global_offset = 3 * ((rowid + i) * maxcols + (colid + j));
                
                Gx[0] += x_conv[local_idx] * input_img[global_offset];
                Gx[1] += x_conv[local_idx] * input_img[global_offset+1];
                Gx[2] += x_conv[local_idx] * input_img[global_offset+2];
                
                Gy[0] += y_conv[local_idx] * input_img[global_offset];
                Gy[1] += y_conv[local_idx] * input_img[global_offset+1];
                Gy[2] += y_conv[local_idx] * input_img[global_offset+2];
            }
        }

        //set the output_img pixel to the average gradient of the convolution
        output_img[global_idx] = sqrt((pown(Gx[0], 2) + pown(Gy[0], 2)));
        output_img[global_idx+1] = sqrt((pown(Gx[1], 2) + pown(Gy[1], 2)));
        output_img[global_idx+2] = sqrt((pown(Gx[2], 2) + pown(Gy[2], 2)));
    }
}