#ifndef LAYER_H
#define LAYER_H
#include <stddef.h>
#include "include/net/activations.h"

struct Network;
typedef struct Network Network;
struct Layer;
typedef struct Layer Layer;

enum class LAYER_TYPE{
    CONVOLUTIONAL,
    DECONVOLUTIONAL,
    CONNECTED,
    MAXPOOL,
    SOFTMAX,
    DETECTION,
    DROPOUT,
    CROP,
    ROUTE,
    COST,
    NORMALIZATION,
    AVGPOOL,
    LOCAL,
    SHORTCUT,
    ACTIVE,
    RNN,
    GRU,
    CRNN,
    BATCHNORM,
    NETWORK,
    XNOR,
    REGION,
    REORG,
    BLANK               // 表示未识别的网络层名称
};

enum class COST_TYPE{
    SSE, MASKED, L1, SMOOTH
};

struct Layer{
    LAYER_TYPE  type;
    ACTIVATION  activation;
    COST_TYPE   costType;
    void        (*forward) (Layer layer, Network network);
    void        (*backward) (Layer layer, Network network);
    void        (*update) (Layer layer, Network network);
    bool        batchNormalize;
    int         height, width, channel;    //the height, width and channels in this layer.
    int         filterNum; // num of filters.
    bool        weightBinaryzation; // if this value is set to 1, weights would loaded with Binaryzation.
    int         batchSize; // the size of each batch.
    int         stride;     // stride of filter.
    int         filterSize; // the size of each filter.
    int         padSize; // padding size.

    float*      weights;
    float*      weights_update; // the derivative of error with respect to weight
    float*      biases;
    float*      biases_update;
    int         numWeights;
    int         numBiases;
    // bool        shortCut;
    // int         batch;
    // bool        forced;
    // bool        flipped;
    // int         inputs;
    // int         outputs;
    // int         nWeights;
    // int         nBiases;
    // int         extra;
    // int         truths;
    // int         inputHeight, inputWidth, inputChannel;
    // int         outputHeight, outputWidth, outputChannels;
    // int         n;
    // int         filterSize;
    // int         stride;
    // int         size;
    // int         reverse;
    // int         flatten;
    // int         pad;
    // int         numClasses;
    // int         onlyforward;
    // int         stopbackward;
    // int         dontload;
    // int         dontloadscales;
    // float       temperature;
    // float       probability;
    // float       scale;
    // char*       cweights;
    // int*        input_layers;
    // int*        input_sizes;
    // size_t      workspace_size;
    // int*        indexes;
    // float*      z_cpu;
    // float*      r_cpu;
    // float*      h_cpu;
};

namespace LayerOP{
    extern Layer makeLayer();
    extern void freeLayer(Layer);
    extern LAYER_TYPE parseLayerType(char* type);
    extern void initializeWeightNormal(Layer l, int scaleSize, int size);
}

#endif