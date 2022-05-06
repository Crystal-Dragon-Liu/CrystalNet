#include "include/net/layer.h"
#include <cstring>
#include "include/Utils/utils.h"
#include "include/Utils/common.h"
namespace LayerOP{
    void        freeLayer(Layer l){
        if(l.type == LAYER_TYPE::DROPOUT){
                //TODO delete rand.
                return;
        }
        // weights
        if(l.weights) DEALLOC_FLOAT_PTR(l.weights);
        if(l.weightUpdates) DEALLOC_FLOAT_PTR(l.weightUpdates);
        if(l.biases) DEALLOC_FLOAT_PTR(l.biases);
        if(l.biasesUpdates) DEALLOC_FLOAT_PTR(l.biasesUpdates);
        
        // output
        if(l.outputData) DEALLOC_FLOAT_PTR(l.outputData);
        if(l.deltas) DEALLOC_FLOAT_PTR(l.deltas);
        // binary weights
        if(l.binaryWeights) DEALLOC_FLOAT_PTR(l.binaryWeights);
        if(l.cWeights) DEALLOC_CHAR_PTR(l.cWeights);
        if(l.scales) DEALLOC_FLOAT_PTR(l.scales);

        // xnor
        if(l.binaryInput) DEALLOC_FLOAT_PTR(l.binaryInput);
        // batch normalization
        if(l.scaleUpdates) DEALLOC_FLOAT_PTR(l.scaleUpdates);
        if(l.mean) DEALLOC_FLOAT_PTR(l.mean);
        if(l.variance) DEALLOC_FLOAT_PTR(l.variance);
        if(l.meanDelta) DEALLOC_FLOAT_PTR(l.meanDelta);
        if(l.varianceDelta) DEALLOC_FLOAT_PTR(l.varianceDelta);
        if(l.rollingMean) DEALLOC_FLOAT_PTR(l.rollingMean);
        if(l.rollingVariance) DEALLOC_FLOAT_PTR(l.rollingVariance);
        if(l.x) DEALLOC_FLOAT_PTR(l.x);
        if(l.xNorm) DEALLOC_FLOAT_PTR(l.xNorm);
        // adam
        if(l.adamM) DEALLOC_FLOAT_PTR(l.adamM);
        if(l.adamV) DEALLOC_FLOAT_PTR(l.adamV);
        if(l.adamBiasM) DEALLOC_FLOAT_PTR(l.adamBiasM);
        if(l.adamScaleM) DEALLOC_FLOAT_PTR(l.adamScaleM);
        if(l.adamBiasV) DEALLOC_FLOAT_PTR(l.adamBiasV);
        if(l.adamScaleV) DEALLOC_FLOAT_PTR(l.adamScaleV);
    }

    void        initializeWeightNormal(Layer* l, int scaleSize){
        PRINT_LOG("initializing weights");
        float scale = sqrt(2./(scaleSize));
        for(int i = 0; i < l->numWeights; ++i) 
        l->weights[i] = scale*UtilFunc::randNormal();
    }

    void        initializeWeightUniform(Layer* l, int scaleSize, int weightSize){
        PRINT_LOG("initializing weights");
        float scale = sqrt(2./scaleSize);
        for(int i = 0; i < weightSize;i++){
            l->weights[i] = scale*UtilFunc::randUniform(-1, 1);
        }
    }

    void        zeroBiases(Layer* l, int biasesSize){
        PRINT_LOG("initializing bias with 0.");
        for(int i = 0; i < biasesSize; ++i){
        l->biases[i] = 0;
        }
    }

    void        binaryWeightInit(Layer* l, int weightSize, int scaleSize){
        PRINT_LOG("setting parameters for binary weights");
        l->binaryWeights = ALLOC_FLOAT_PTR(weightSize);
        l->cWeights      = ALLOC_CHAR_PTR(weightSize);
        l->scales        = ALLOC_FLOAT_PTR(scaleSize);
    }

    void        xnorInit(Layer* l, int weightSize){
        PRINT_LOG("setting parameters for binary input and weights");
        l->binaryWeights = ALLOC_FLOAT_PTR(weightSize);
        l->binaryInput = ALLOC_FLOAT_PTR(l->numInputs*l->batchSize);
    }

    void        batchNormalInit(Layer *l, int n){
        PRINT_LOG("setting parameters for BatchNormalization");
        l->scales = ALLOC_FLOAT_PTR(n);
        l->scaleUpdates = ALLOC_FLOAT_PTR(n);
        for(int i = 0; i < n; i++){
                l->scales[i] = 1;
        }
        l->mean = ALLOC_FLOAT_PTR(n);
        l->variance = ALLOC_FLOAT_PTR(n);
        l->meanDelta = ALLOC_FLOAT_PTR(n);
        l->varianceDelta =ALLOC_FLOAT_PTR(n);
        l->rollingMean = ALLOC_FLOAT_PTR(n);
        l->rollingVariance = ALLOC_FLOAT_PTR(n);
        l->x = ALLOC_FLOAT_PTR(l->batchSize * l->numOutputs);
        l->xNorm = ALLOC_FLOAT_PTR(l->batchSize * l->numOutputs);
    }

    void        adamInit(Layer* l, int weightSize, int n){
        PRINT_LOG("setting parameters for Adam.");
        l->adam = true;
        l->adamM = ALLOC_FLOAT_PTR(weightSize);
        l->adamV = ALLOC_FLOAT_PTR(weightSize);
        l->adamBiasM = ALLOC_FLOAT_PTR(n);
        l->adamScaleM = ALLOC_FLOAT_PTR(n);
        l->adamBiasV = ALLOC_FLOAT_PTR(n);
        l->adamScaleV = ALLOC_FLOAT_PTR(n);
    }

    Layer       makeLayer(){
        Layer l;
        l.forward = nullptr;
        l.backward = nullptr;
        l.update = nullptr;
        //weights
        l.weights = nullptr;
        l.weightUpdates = nullptr;
        l.biases = nullptr;
        l.biasesUpdates = nullptr;
        //output
        l.outputData = nullptr;
        l.deltas = nullptr;
        //binary weights
        l.binaryWeights = nullptr;
        l.cWeights = nullptr;
        l.scales = nullptr;
        // xnor
        l.binaryInput = nullptr;
        // batch normalization
        l.scaleUpdates = nullptr;
        l.mean = nullptr;
        l.variance = nullptr;
        l.meanDelta = nullptr;
        l.varianceDelta = nullptr;
        l.rollingMean = nullptr;
        l.rollingVariance = nullptr;
        l.x= nullptr;
        l.xNorm = nullptr;
        // adam
        l.adamM = nullptr;
        l.adamV = nullptr;
        l.adamBiasM = nullptr;
        l.adamScaleM = nullptr;
        l.adamBiasV = nullptr;
        l.adamScaleV = nullptr;
        return l;
    }

    LAYER_TYPE  parseLayerType(char* type){
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