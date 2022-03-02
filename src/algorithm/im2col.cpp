#include "include/algorithm/im2col.h"
#include <stdio.h>

void im2ColCPU(float* batchData, int channels, int height, int width, int kernelSize, int stride, int pad, float* dataCol){
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
                int colIndex = (c*heightCol + h) * widthCol + w; //TODO what this means???????
            }
        }
    }

}
