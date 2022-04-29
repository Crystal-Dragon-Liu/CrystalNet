#include "include/net/activations.h"
#include <cstring>
#include "include/Utils/utils.h"
namespace ACT_OP{


    ACTIVATION getActivation(const char* s){
        if (strcmp(s, "logistic")==0) return ACTIVATION::LOGISTIC;
    if (strcmp(s, "loggy")==0) return ACTIVATION::LOGGY;
    if (strcmp(s, "relu")==0) return ACTIVATION::RELU;
    if (strcmp(s, "elu")==0) return ACTIVATION::ELU;
    if (strcmp(s, "relie")==0) return ACTIVATION::RELIE;
    if (strcmp(s, "plse")==0) return ACTIVATION::PLSE;
    if (strcmp(s, "hardtan")==0) return ACTIVATION::HARDTAN;
    if (strcmp(s, "lhtan")==0) return ACTIVATION::LHTAN;
    if (strcmp(s, "linear")==0) return ACTIVATION::LINEAR;
    if (strcmp(s, "ramp")==0) return ACTIVATION::RAMP;
    if (strcmp(s, "leaky")==0) return ACTIVATION::LEAKY;
    if (strcmp(s, "tanh")==0) return ACTIVATION::TANH;
    if (strcmp(s, "stair")==0) return ACTIVATION::STAIR;
    // fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    PRINT("Couldn't find activation function <", s, ">, ", "going with ReLU");
    return ACTIVATION::RELU;
    }

    char* getActivationStr(ACTIVATION act){
        return "";
    }

    float activate(float input, ACTIVATION act){
        return -1;
    }
}
