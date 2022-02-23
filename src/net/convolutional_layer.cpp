#include "include/net/convolutional_layer.h"
#include "include/Utils/common.h"
#include "include/net/network.h"
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
        LayerOP::initializeWeightNormal(l, c*size*size);
        // calculate the size of output by convolutional layer.
        int outputWidth = getConvOutputWidth(l);
        int outputHeight = getConvOutputHeight(l);
        l.outputHeight = outputHeight;
        l.outputWidth  = outputWidth;
        l.outputChannel = n;
        l.numOutputs = outputWidth * outputHeight * n;
        // calculate num of input.
        l.numInputs = l.width * l.height * l.channel;
        // initialize relative data.
        l.outputData = ALLOC_FLOAT_PTR(l.batchSize * l.numOutputs);
        l.deltas    = ALLOC_FLOAT_PTR(l.batchSize * l.numOutputs);
        // initialize functions for forwarding, backwarding, updating parameters.
        l.forward  = forwardConvLayer;
        l.backward = backwardConvLayer;
        l.update  = updateConvLayer;
        // TODO continue to initialize params.
        

    }
    int                 getConvOutputHeight(Layer l){
        return (l.height + l.padSize * 2  - l.filterSize) / l.stride + 1;
    }
    int                 getConvOutputWidth(Layer l){
        return  (l.width + l.padSize * 2 - l.filterSize ) / l.stride + 1;
    }
    void                forwardConvLayer(Layer layer, Network network){

    }
    void                backwardConvLayer(Layer layer, Network network){

    }
    void                updateConvLayer(Layer layer, Network network){

    }
    size_t              getWorkSpaceSize(Layer l){
        return static_cast<size_t>(l.outputHeight * l.outputWidth * l.channel * l.filterSize * l.filterSize * sizeof(float));
    }


}