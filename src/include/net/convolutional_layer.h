#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H
#include "include/net/layer.h"


typedef Layer ConvolutionalLayer;
namespace CONVOLUTIONAL_OP{
   extern ConvolutionalLayer makeConvolutionalLayer(int batch, int h, int w, int c, int n, int size, int stride, int padding, ACTIVATION activation, int batch_normalize, int binary, int xnor, int adam);
}

#endif
