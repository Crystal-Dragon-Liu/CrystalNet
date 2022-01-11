#ifndef NETWORK_H
#define NETWORK_H
#include <stddef.h>
#include "include/net/layer.h"
#include "include/Utils/tree.h"

/*@brief  learning rate policy. */
enum class LearningRatePolicy{
    CONSTANT, STEP, EXP, POLY, STEPS, STG, RANDOM
};


typedef struct Network{
    int                 totalLayerNum;
    // int                 batch; /// the batch size of image set.
    size_t*             seen; /// num of image which have been processed.
    // float               epoch;
    // int                 subdivisions;
    // float               momentum;
    // float               decay;
    Layer *             layers;
    // float*              output;
    // LearningRatePolicy  policy;
    
    // float               learningRate;
    // float               gamma;
    // float               scale;
    // float               power;
    // int                 timeSteps;
    // int                 step;
    // int                 maxBatches;
    // float*              scales;
    // int*                steps;
    // int                 numSteps;
    // int                 burnIn;
    // int                 adam;
    // float               B1;
    // float               B2;
    // float               eps;

    // int                 inputs;         
    // int                 outputs;        

    // int                 truths;
    // int                 notruth;
    // int                 h, w, c;
    // int                 maxCrop;
    // int                 minCrop;
    // int                 center;
    // float               angle;
    // float               aspect;
    // float               exposure;
    // float               saturation;
    // float               hue;
    // int gpu_index;
    // Tree *hierarchy;

    // float *input;
    // float *truth;
    // float *delta;
    // float *workspace;
    // int train;
    // int index;
    float *cost; 
} Network;

typedef struct sizeParams{
    size_t batch;
    size_t inputs;
    size_t width;
    size_t height;
    size_t channals;
    size_t time_steps;
    Network net;
}sizeParams;

namespace NetworkOP{

    extern Network makeNetwork(int n);
    extern void freeNetwork(Network net);
    extern Network parseNetworkConfig(const char* fileName);
}

#endif