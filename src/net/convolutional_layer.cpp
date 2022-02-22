#include "include/net/convolutional_layer.h"
#include "include/Utils/common.h"
namespace CONVOLUTIONAL_OP{
    ConvolutionalLayer makeConvolutionalLayer(int batch, int h, int w, int c, int n, int size, int stride, int padding, ACTIVATION activation, int batch_normalize, int binary, int xnor, int adam){
        int i;
        ConvolutionalLayer l;
        l.type = LAYER_TYPE::CONVOLUTIONAL; // type of layer.
        l.height = h;
        l.width = w;
        l.channel = c;
        l.filterNum = n;
        l.weightBinaryzation = binary;
        l.batchSize = batch;
        l.stride = stride;
        l.filterSize = size;
        l.padSize = padding;
        l.batchNormalize = batch_normalize;
        // initialize weights. num of weights = channels * filterNum * filterSize* filterSize.
        l.weights = ALLOC_FLOAT_PTR(c*n*size*size);
        l.weights_update = ALLOC_FLOAT_PTR(c*n*size*size);
        l.biases = ALLOC_FLOAT_PTR(n);
        l.biases_update = ALLOC_FLOAT_PTR(n);
        l.numWeights = c*n*size*size;
        l.numBiases  = n;
        LayerOP::initializeWeightNormal(l, c*size*size, l.numWeights);
    }
}