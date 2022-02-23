#include "include/net/layer.h"
#include <cstring>
#include "include/Utils/utils.h"
#include "include/Utils/common.h"
namespace LayerOP{
    void freeLayer(Layer){ return; }

    void initializeWeightNormal(Layer l, int scaleSize){
        float scale = sqrt(2./(scaleSize));
        for(int i = 0; i < l.numWeights; ++i) 
        l.weights[i] = scale*UtilFunc::randNormal();
    }

    void binaryWeightInit(Layer l, int weightSize, int scaleSize){
        l.binaryWeights = ALLOC_FLOAT_PTR(weightSize);
        l.cWeights      = ALLOC_CHAR_PTR(weightSize);
        l.scales        = ALLOC_FLOAT_PTR(scaleSize);
    }

    void xnorInit(Layer l, int weightSize){
        l.binaryWeights = ALLOC_FLOAT_PTR(weightSize);
        l.binaryInput = ALLOC_FLOAT_PTR(l.numInputs*l.batchSize);
    }

    void batchNormalInit(Layer l, int n){
        l.scales = ALLOC_FLOAT_PTR(n);
        l.scaleUpdates = ALLOC_FLOAT_PTR(n);
        for(int i = 0; i < n; i++){
                l.scales[i] = 1;
        }
        l.mean = ALLOC_FLOAT_PTR(n);
        l.variance = ALLOC_FLOAT_PTR(n);
        l.meanDelta = ALLOC_FLOAT_PTR(n);
        l.varianceDelta =ALLOC_FLOAT_PTR(n);
        l.rollingMean = ALLOC_FLOAT_PTR(n);
        l.rollingVariance = ALLOC_FLOAT_PTR(n);
        l.x = ALLOC_FLOAT_PTR(l.batchSize * l.numOutputs);
        l.xNorm = ALLOC_FLOAT_PTR(l.batchSize * l.numOutputs);
    }

    void adamInit(Layer l, int weightSize, int n){
        l.adam = true;
        l.adamM = ALLOC_FLOAT_PTR(weightSize);
        l.adamV = ALLOC_FLOAT_PTR(weightSize);
        l.adamBiasM = ALLOC_FLOAT_PTR(n);
        l.adamScaleM = ALLOC_FLOAT_PTR(n);
        l.adamBiasV = ALLOC_FLOAT_PTR(n);
        l.adamScaleV = ALLOC_FLOAT_PTR(n);
    }



    Layer makeLayer(){
        Layer l;
        l.forward = nullptr;
        l.backward = nullptr;
        l.update = nullptr;
        return l;
    }

    LAYER_TYPE parseLayerType(char* type){
            if (strcmp(type, "[shortcut]")==0) return LAYER_TYPE::SHORTCUT;
    if (strcmp(type, "[crop]")==0) return LAYER_TYPE::CROP;
    if (strcmp(type, "[cost]")==0) return LAYER_TYPE::COST;
    if (strcmp(type, "[detection]")==0) return LAYER_TYPE::DETECTION;
    if (strcmp(type, "[region]")==0) return LAYER_TYPE::REGION;
    if (strcmp(type, "[local]")==0) return LAYER_TYPE::LOCAL;
    if (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) return LAYER_TYPE::CONVOLUTIONAL;
    if (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) return LAYER_TYPE::DECONVOLUTIONAL;
    if (strcmp(type, "[activation]")==0) return LAYER_TYPE::ACTIVE;
    if (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) return LAYER_TYPE::NETWORK;
    if (strcmp(type, "[crnn]")==0) return LAYER_TYPE::CRNN;
    if (strcmp(type, "[gru]")==0) return LAYER_TYPE::GRU;
    if (strcmp(type, "[rnn]")==0) return LAYER_TYPE::RNN;
    if (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) return LAYER_TYPE::CONNECTED;
    if (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) return LAYER_TYPE::MAXPOOL;
    if (strcmp(type, "[reorg]")==0) return LAYER_TYPE::REORG;
    if (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) return LAYER_TYPE::AVGPOOL;
    if (strcmp(type, "[dropout]")==0) return LAYER_TYPE::DROPOUT;
    if (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) return LAYER_TYPE::NORMALIZATION;
    if (strcmp(type, "[batchnorm]")==0) return LAYER_TYPE::BATCHNORM;
    if (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) return LAYER_TYPE::SOFTMAX;
    if (strcmp(type, "[route]")==0) return LAYER_TYPE::ROUTE;
    return LAYER_TYPE::BLANK;
    }

}