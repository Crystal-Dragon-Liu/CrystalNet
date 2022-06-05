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
        if(l.weights) delete l.weights;
        if(l.weightUpdates) delete l.weightUpdates;
        if(l.biases) delete l.biases;
        if(l.biasesUpdates) delete l.biasesUpdates;
        
        // output
        if(l.outputData) delete l.outputData;
        if(l.deltas) delete l.deltas;
        // binary weights
        if(l.binaryWeights) delete l.binaryWeights;
        if(l.cWeights) delete l.cWeights;
        if(l.scales) delete l.scales;

        // xnor
        if(l.binaryInput) delete l.binaryInput;
        // batch normalization
        if(l.scaleUpdates) delete l.scaleUpdates;
        if(l.mean) delete l.mean;
        if(l.variance) delete l.variance;
        if(l.meanDelta) delete l.meanDelta;
        if(l.varianceDelta) delete l.varianceDelta;
        if(l.rollingMean) delete l.rollingMean;
        if(l.rollingVariance) delete l.rollingVariance;
        if(l.x) delete l.x;
        if(l.xNorm) delete l.xNorm;
        // adam
        if(l.adamM) delete l.adamM;
        if(l.adamV) delete l.adamV;
        if(l.adamBiasM) delete l.adamBiasM;
        if(l.adamScaleM) delete l.adamScaleM;
        if(l.adamBiasV) delete l.adamBiasV;
        if(l.adamScaleV) delete l.adamScaleV;
    }

    void        initializeWeightNormal(Layer* l, int scaleSize){
        PRINT_LOG("initializing weights");
        float scale = sqrt(2./(scaleSize));
        for(int i = 0; i < l->numWeights; ++i) 
        (*l->weights)[i] = scale*UtilFunc::randNormal();
    }

    void        initializeWeightUniform(Layer* l, int scaleSize, int weightSize){
        PRINT_LOG("initializing weights");
        float scale = sqrt(2./scaleSize);
        for(int i = 0; i < weightSize;i++){
            (*l->weights)[i] = scale*UtilFunc::randUniform(-1, 1);
        }
    }

    void        zeroBiases(Layer* l, int biasesSize){
        PRINT_LOG("initializing bias with 0.");
        for(int i = 0; i < biasesSize; ++i){
        (*l->biases)[i] = 0;
        }
    }

    void        binaryWeightInit(Layer* l, int weightSize, int scaleSize){
        PRINT_LOG("setting parameters for binary weights");
        l->binaryWeights = new std::vector<float>(weightSize);
        l->cWeights      = new std::vector<char>(weightSize);
        l->scales        = new std::vector<float>(scaleSize);
    }

    void        xnorInit(Layer* l, int weightSize){
        PRINT_LOG("setting parameters for binary input and weights");
        l->binaryWeights = new std::vector<float>(weightSize);
        l->binaryInput = new std::vector<float>(l->numInputs*l->batchSize);
    }

    void        batchNormalInit(Layer *l, int n){
        PRINT_LOG("setting parameters for BatchNormalization");
        l->scales = new std::vector<float>(n);
        l->scaleUpdates = new std::vector<float>(n);
        for(int i = 0; i < n; i++){
                (*l->scales)[i] = 1;
        }
        l->mean = new std::vector<float>(n);
        l->variance = new std::vector<float>(n);
        l->meanDelta = new std::vector<float>(n);
        l->varianceDelta =new std::vector<float>(n);
        l->rollingMean = new std::vector<float>(n);
        l->rollingVariance = new std::vector<float>(n);
        l->x = new std::vector<float>(l->batchSize * l->numOutputs);
        l->xNorm = new std::vector<float>(l->batchSize * l->numOutputs);
    }

    void        adamInit(Layer* l, int weightSize, int n){
        PRINT_LOG("setting parameters for Adam.");
        l->adam = true;
        l->adamM = new std::vector<float>(weightSize);
        l->adamV = new std::vector<float>(weightSize);
        l->adamBiasM = new std::vector<float>(n);
        l->adamScaleM = new std::vector<float>(n);
        l->adamBiasV = new std::vector<float>(n);
        l->adamScaleV = new std::vector<float>(n);
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