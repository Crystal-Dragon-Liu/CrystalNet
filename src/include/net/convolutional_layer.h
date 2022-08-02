#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H
#include "include/net/layer.h"


typedef Layer ConvolutionalLayer;
namespace CONVOLUTIONAL_OP{
   extern ConvolutionalLayer  makeConvolutionalLayer(int batch, int h, int w, int c, int n, int size, int stride, int padding, ACTIVATION activation, int batch_normalize, int binary, int xnor, int adam);
   extern int                 getConvOutputHeight(Layer l);
   extern int                 getConvOutputWidth(Layer l);
   extern void                forwardConvLayer(Layer layer, Network network);
   extern void                backwardConvLayer(Layer layer, Network network);
   extern void                updateConvLayer(Layer layer, Network network);
   extern size_t              getWorkSpaceSize(Layer l);
   extern void				  showConvLayerRelativeMatrix(Layer l);
   extern void				  showConvLayerInfo(Layer l); // TODO show the name of convolutional layer.


}

#endif
