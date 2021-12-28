#include "include/net/activations.h"

namespace ACT_OP{


    ACTIVATION getActivation(const char* funcName){
        return ACTIVATION::RELU;
    }

    char* getActivationStr(ACTIVATION act){
        return "";
    }

    float activate(float input, ACTIVATION act){
        return -1;
    }
}