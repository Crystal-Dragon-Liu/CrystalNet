#ifndef IM2COL_H
#define IM2COL_H

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
extern void im2ColCPU(float* batchData, int channels, int height, int width, int kernelSize, int stride, int pad, float* dataCol);

#endif