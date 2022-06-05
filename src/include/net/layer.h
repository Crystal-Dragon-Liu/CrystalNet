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

    std::vector<float>*     weights;
    std::vector<float>*     weightUpdates; // the derivative of error with respect to weight
    std::vector<float>*     biases;
    std::vector<float>*     biasesUpdates;
    int         numWeights;
    int         numBiases;

    // output 
    int         outputHeight;
    int         outputWidth;
    int         outputChannel;
    int         numOutputs;
    int         numInputs;
    std::vector<float>*     outputData; // the output data with dimension(l.out_h * l.out_w * l.out_c * l.batch)
    std::vector<float>*     deltas;


    //binary weight
    std::vector<float>*     binaryWeights;
    std::vector<char>*      cWeights;
    std::vector<float>*     scales; // also used in BatchNormalization

    //xnor 
    std::vector<float>*     binaryInput;
    
    // batch normalization
    std::vector<float>*     scaleUpdates;
    std::vector<float>*     mean;
    std::vector<float>*     variance;
    std::vector<float>*     meanDelta;
    std::vector<float>*     varianceDelta;
    std::vector<float>*     rollingMean;
    std::vector<float>*     rollingVariance;
    std::vector<float>*     x;
    std::vector<float>*     xNorm;

    // adam
    bool         adam;
    std::vector<float>*     adamM;
    std::vector<float>*     adamV;
    std::vector<float>*     adamBiasM;
    std::vector<float>*     adamScaleM;
    std::vector<float>*     adamBiasV;
    std::vector<float>*     adamScaleV;
    float               B1;
    float               B2;
    float               eps;
    
    //workspace 
    size_t      workspaceSize;

    int         flipped;
    int         dot;

    //convolutional layer
    float       clip;
    bool        truth;
    bool        onlyForward;
    bool        stopBackward;
    bool        noDataLoad;
    bool        noScaleLoad;
    float       learningRateScale;
    float       smooth;




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
    // initialize weights 
    extern void initializeWeightNormal(Layer* l, int scaleSize);
    extern void initializeWeightUniform(Layer* l, int scaleSize, int weightSize);

    // initialize biases
    extern void zeroBiases(Layer* l, int biasesSize);

    extern void binaryWeightInit(Layer* l, int weightSize, int scaleSize);
    extern void xnorInit(Layer* l, int weightSize);
    extern void batchNormalInit(Layer *l, int n);
    extern void adamInit(Layer* l, int weightSize, int n);
}

#endif