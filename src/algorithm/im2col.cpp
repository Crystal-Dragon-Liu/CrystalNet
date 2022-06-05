#include "include/algorithm/im2col.h"
#include <stdio.h>

void im2ColCPU(std::vector<float>* batchData, int channels, int height, int width, int kernelSize, int stride, int pad, std::vector<float>* dataCol){
    // int c, h, w;
    int heightCol = (height + 2*pad - kernelSize) / stride + 1;
    int widthCol = (width + 2*pad - kernelSize) / stride + 1;
    int cols = channels * kernelSize * kernelSize;
    for(int c = 0; c < cols; c++){
        // calculate the offset of width for each kernel.
        // when c = 5, we can calculate widthOffset to be 2 in that "5 % 3 = 2", meaning that we now access 
        // the data in the second column and index  2 - 1,
        int widthOffset = c % kernelSize;
        // calculate offset of height for each kernel.
        // c/kernelSize: it stands for the loop has traversed c/kernelSize rows.
        // To get heightOffset, (c/kernelSize) should mod kernelSize(3 rows)
        int heightOffset = (c / kernelSize) % kernelSize;
        int channelOffset = c / (kernelSize*kernelSize);
        for(int h = 0; h < heightCol; h++){
            for(int w = 0; w < widthCol; w++){
                int imRow = heightOffset + h * stride; // initial row index when we accessing image.
                int imCol = widthOffset + w*stride; // initial col index when we accessing image.
                int colIndex = (c*heightCol + h) * widthCol + w; // find the index of dataCol to fill.
                (*dataCol)[colIndex] = getPixelIm2col(batchData, 
                height, width, channels, 
                imRow, imCol, channelOffset, pad);
            }
        }
    }
}


float getPixelIm2col(std::vector<float>* img_data, int img_width, int img_height, int img_channels, int index_row, int index_col, int index_c, int padding){
    //! index_row is  the row index on padded image.
    index_row -= padding;
    index_col -= padding;
    if(index_row <= 0 || index_col <=0 // consider the index at upper left corner of image.
        || index_row >= img_height || index_col >= img_width) // consider the index at lower right corner of image.
        return 0;
    // index_c * img_height * img_width + img_width* index_row + index_col
    return (*img_data)[index_col + img_width*(index_row + img_height*index_c)];
}
