#include "include/net/convolutional_layer.h"
#include "include/Utils/common.h"
#include "include/net/network.h"
#include "include/Utils/utils.h"
namespace CONVOLUTIONAL_OP{
    ConvolutionalLayer makeConvolutionalLayer(int batch, int h, int w, int c, int n, int size, int stride, int padding, ACTIVATION activation, int batch_normalize, int binary, int xnor, int adam){
        int i;
        ConvolutionalLayer l = LayerOP::makeLayer();
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
        l.weightUpdates = ALLOC_FLOAT_PTR(c*n*size*size);
        l.biases = ALLOC_FLOAT_PTR(n);
        l.biasesUpdates = ALLOC_FLOAT_PTR(n);
        l.numWeights = c*n*size*size;
        l.numBiases  = n;
        LayerOP::initializeWeightNormal(&l, c*size*size);
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

        if(binary) LayerOP::binaryWeightInit(&l, c*n*size*size, n);
        if(xnor) LayerOP::xnorInit(&l, c*n*size*size);
        if(batch_normalize) LayerOP::batchNormalInit(&l, n);
        if(adam) LayerOP::adamInit(&l, c*n*size*size, n);

        //TODO GPU

        // initialize size of workspace
        l.workspaceSize = getWorkSpaceSize(l);
        l.activation = activation;
        PRINT("conv ", n, size, "x", size, "/", stride, w, "x", h, "x", c, "-> ", l.outputWidth, "x", l.outputHeight, "x", l.outputChannel);
        return l;
    }
    
    int                 getConvOutputHeight(Layer l){
        return (l.height + l.padSize * 2  - l.filterSize) / l.stride + 1;
    }
    
    int                 getConvOutputWidth(Layer l){
        return  (l.width + l.padSize * 2 - l.filterSize ) / l.stride + 1;
    }
    
    void                forwardConvLayer(Layer l, Network network){
        int out_h = l.outputHeight;
        int out_w = l.outputWidth;
        // initialize outputs by zero.
        UtilFunc::initDataByCPU(l.numOutputs*l.batchSize, 0, l.outputData);
        // TODO binary op for inputs and weights??
        int m = l.filterNum;
        int k = l.filterSize*l.filterSize*l.channel; // size of each kernel in this Conv layer
        int n = out_h* out_w;

        float* weights = l.weights;
        float* workspace = network.workspace_;
        float* outputData = l.outputData;
        for(int i = 0; i < l.batchSize; ++i){
            //Convolutional Algorithm for each batch.
        }
    }
    
    void                backwardConvLayer(Layer layer, Network network){}
    
    void                updateConvLayer(Layer layer, Network network){}
    
    size_t              getWorkSpaceSize(Layer l){
        return static_cast<size_t>(l.outputHeight * l.outputWidth * l.channel * l.filterSize * l.filterSize * sizeof(float));
    }


}