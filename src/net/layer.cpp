#include "include/net/layer.h"
#include <cstring>
namespace LayerOP{
    void freeLayer(Layer){ return; }

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