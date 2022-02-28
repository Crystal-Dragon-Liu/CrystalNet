#include "include/algorithm/im2col.h"
#include <stdio.h>

void im2ColCPU(float* batchData, int channels, int height, int width, int kernelSize, int stride, int pad, float* dataCol){
    int c, h, w;
    int heightCol = (height + 2*pad - kernelSize) / stride + 1;
    int widthCol = (width + 2*pad - kernelSize) / stride + 1;
    int channelsCol = channels * kernelSize * kernelSize;
}
