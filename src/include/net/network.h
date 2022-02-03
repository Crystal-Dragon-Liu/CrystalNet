#ifndef NETWORK_H
#define NETWORK_H
#include <stddef.h>
#include "include/net/layer.h"
#include "include/Utils/tree.h"
#include <map>
#include <string>
/*@brief  learning rate policy. */
enum class LearningRatePolicy{
    CONSTANT, STEP, EXP, POLY, STEPS, SIG, RANDOM
};


typedef struct Network{
    int                 totalLayerNum_;
    int                 batch_; /// the batch size of image set.
    size_t*             seen_; /// num of image which have been processed. //! ptr
    float               learningRate_;
    float               momentum_;
    float               decay_;
    // float               epoch;
    // int                 subdivisions;
    // float               momentum;
    // float               decay;
    Layer *             layers_; //! ptr
    int                 timeSteps_;
    int                 noTruth_;
    int                 subdivisions_;
    int                 adam_;
    // parameters for adam algorithm.
    float               b1_;
    float               b2_;
    float               eps_;
    int                 height_;
    int                 width_;
    int                 channel_;
    int                 numInputs_;
    int                 maxCrop_;
    int                 minCrop_;
    int                 center_;
    float               angle_;
    float               aspect_;
    float               saturation_;
    float               exposure_;
    float               hue_;
    LearningRatePolicy  learningRatePolicy_;
    // parameters for policy STEP, SIG
    int                 step_;
    float               scale_;
    // parameters for policy STEPS
    int*                steps_;  //! ptr
    float*              scales_; //! ptr
    int                 numSteps_;
    // parameters for policy EXP, SIG
    float               gamma_;

    int                 maxBatches_;
    // Once burnIn_ is set, when number of updates is less than burnIn_
    // instead of using the configured learning rate update policy, the following formula is used
    // lr = base_lr * power(batch_num/burn_in, pwr)
    int                 burnIn_; 
    float               power_;
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
    float *cost_; 
} Network;

typedef struct SizeParams{
    size_t batch;
    size_t inputs;
    size_t width;
    size_t height;
    size_t channals;
    size_t time_steps;
    Network net;
}SizeParams;

struct ConfigSection;
struct NodeList;
// typedef struct Network Network;

namespace NetworkOP{
    /*  
        @brief make sure the config item is relative to Network.
        @param section, a piece of config, would be expected.
    */
    extern bool                 checkNetworkConfig(ConfigSection* section);

    /*  
        @brief create a Network obj with some parameters initialized.
        @param n is the total size of layers.
    */
    extern Network              makeNetwork(int n);

    /*
        @brief  free space from Network.
        @param  network
    */
    extern void                 freeNetwork(Network net);

    /*
        @brief free parameters of learning rate policy
    */
    extern void                 freeNetworkParam(Network* net);
    extern void                 freeStepParam(Network* net);

    /*
        @brief parse all of the config from a file with format 'cfg'
        @param fileName is the path of .cfg file.
    */
    extern Network              parseNetworkConfig(const char* fileName);

    /*
        @brief load the general config
    */
    extern void                 loadNetworkCommonConfig(NodeList* options, Network* net);

    /*
        @brief loading learning rate policy from char array expected.
    */
    extern LearningRatePolicy   parseLearningRatePolicy(char* policy);

    /* @brief functions for initializing the parameters for learning rate policy*/
    extern void                 initializePolicy(LearningRatePolicy policy);
    extern void                 stepInitialize(Network* net, NodeList* options);
    extern void                 stepsInitialize(Network* net, NodeList* options);
    extern void                 expInitialize(Network* net, NodeList* options);
    extern void                 sigInitialize(Network* net, NodeList* options);
    extern void                 polyInitialize(Network* net, NodeList*options);
    extern void                 randomInitialize(Network*net, NodeList*options);

    /* @brief functions for initializing involving parameters via LearningRatePolicy expected. */
    extern void                 initLrParam(Network* net, NodeList* options);
}

#endif