#include "include/net/network.h"
#include "include/Utils/common.h"
#include "include/Utils/config_list.h"
// #include "include/Utils/utils.h"

/* some allocator for Layer*/
typedef NetSimpleAlloc<Layer, MyNetCAlloc> LayerAllocator;
namespace NetworkOP{
    Network makeNetwork(int n){
        Network network;
        network.totalLayerNum_ = n;
        network.layers_ = LayerAllocator::allocate(network.totalLayerNum_, sizeof(Layer));
        network.seen_ = SizeAllocator::allocate(1, sizeof(int));
        network.cost_ = DataCitemAllocator::allocate(1, sizeof(float));
        network.steps_ = nullptr;
        network.scales_ = nullptr;
        return network;
    }

    void freeNetwork(Network net){
        LayerAllocator::deallocate(net.layers_);
        SizeAllocator::deallocate(net.seen_);
        DataCitemAllocator::deallocate(net.cost_);
        // free the space for parameters
        freeNetworkParam(&net);
    }

    void  freeStepParam(Network* net){
        if(net->steps_ != nullptr) IntAllocator::deallocate(net->steps_);
        if(net->scales_ != nullptr) DataCitemAllocator::deallocate(net->scales_);
    }

    void  freeNetworkParam(Network* net){freeStepParam(net);}

    Network parseNetworkConfig(const char* fileName){
        // read network configuration.
        NodeList* sections = ConfigIO::readModelConfig(fileName);
        Node* node = sections->front_;
        if(!node) UtilFunc::errorOccur("no sections loaded from config file!");
        Network net = makeNetwork(sections->size_ - 1);

        ConfigSection* s = reinterpret_cast<ConfigSection*>(node->value_);
        NodeList* options = s->config;
        if(!checkNetworkConfig(s)) UtilFunc::errorOccur("First section must be [net] or [network]");
        // std::cout << "parsing network config" << std::endl;
        loadNetworkCommonConfig(options, &net);
        
        // free all nodeList;
        NodeOP::freeNodeList(sections, UtilFunc::freeConfigSection);
        return net;
    }

    bool checkNetworkConfig(ConfigSection* section){
        return (strcmp(section->type, "[net]")==0
            || strcmp(section->type, "[network]")==0);
    }

    void loadNetworkCommonConfig(NodeList* options, Network* net){
        using namespace std;
        cout << "<---- loading common parameters ---->" << endl;
        net->batch_ = ConfigIO::configFindToInt(options,"batch", 1);
        net->learningRate_ = ConfigIO::configFindToInt(options, "learning_rate", .001);
        net->decay_ = ConfigIO::configFindToFloat(options, "decay", .0001);
        net->momentum_ = ConfigIO::configFindToFloat(options, "momentum", .9);
        int subdivs = ConfigIO::configFindToInt(options, "subdivisions", 1);
        net->timeSteps_ = ConfigIO::configFindToInt(options, "time_steps", 1, true);
        net->noTruth_ = ConfigIO::configFindToInt(options, "notruth", 0);
        net->batch_ /= subdivs;
        net->batch_ *= net->timeSteps_;
        net->subdivisions_ = net->subdivisions_;
        // parameters for adam algorithm
        net->adam_ = ConfigIO::configFindToInt(options, "adam", 0);
        if(net->adam_){
            net->b1_ = ConfigIO::configFindToFloat(options, "B1", 0);
            net->b2_ = ConfigIO::configFindToFloat(options, "B2", 0);
            net->eps_ = ConfigIO::configFindToFloat(options, "eps", .00000001);
        }
        net->height_ = ConfigIO::configFindToInt(options, "height", 0);
        net->width_ = ConfigIO::configFindToInt(options, "width", 0);
        net->channel_ = ConfigIO::configFindToInt(options, "channels", 0);
        net->numInputs_ = ConfigIO::configFindToInt(options, "inputs", net->channel_ * net->height_ * net->width_, true);
        net->maxCrop_ = ConfigIO::configFindToInt(options, "max_crop", net->width_ * 2, true);
        net->minCrop_ = ConfigIO::configFindToInt(options, "min_crop", net->width_, true);
        net->center_ = ConfigIO::configFindToInt(options, "center", 0, true);
        net->angle_ = ConfigIO::configFindToFloat(options, "angle", 0, true);
        net->aspect_ = ConfigIO::configFindToFloat(options, "aspect", 1, true);
        net->saturation_ = ConfigIO::configFindToFloat(options, "saturation", 1, true);
        net->exposure_ = ConfigIO::configFindToFloat(options, "exposure", 1, true);
        net->hue_ = ConfigIO::configFindToFloat(options, "hue", 0, true);
        // check input parameters
        if(!net->numInputs_ && !(net->height_ && net->width_ && net->channel_) ){
            UtilFunc::errorOccur("No input parameters supplied");}
        char* policy_s = ConfigIO::configFindToStr(options, "policy", const_cast<char*>("constant"));
        net->learningRatePolicy_ = parseLearningRatePolicy(policy_s);
        // TODO parse learning rate policy and initialize involved parameters.
        net->numSteps_ = ConfigIO::configFindToInt(options, "max_batches", 0);
    }

    LearningRatePolicy parseLearningRatePolicy(char* policy){
        if (strcmp(policy, "random")==0) return LearningRatePolicy::RANDOM;
        if (strcmp(policy, "poly")==0) return LearningRatePolicy::POLY;
        if (strcmp(policy, "constant")==0) return LearningRatePolicy::CONSTANT;
        if (strcmp(policy, "step")==0) return LearningRatePolicy::STEP;
        if (strcmp(policy, "exp")==0) return LearningRatePolicy::EXP;
        if (strcmp(policy, "sigmoid")==0) return LearningRatePolicy::SIG;
        if (strcmp(policy, "steps")==0) return LearningRatePolicy::STEPS;
        PRINT("Couldn't find policy <", policy, ">, going with constant\n");
        return LearningRatePolicy::CONSTANT;
    }

    void               stepInitialize(Network* net, NodeList* options){
        net->step_ = ConfigIO::configFindToInt(options, "step", 1);
        net->scale_ = ConfigIO::configFindToFloat(options, "scale", 1);
    }
    void                 stepsInitialize(Network* net, NodeList* options){
        char *l = ConfigIO::configFind(options, "steps");   
        char *p = ConfigIO::configFind(options, "scales");   
        if(!l || !p) UtilFunc::errorOccur("STEPS policy must have steps and scales in cfg file");
        int len = strlen(l);
        int n = 1;
        int i;
        for(i = 0; i < len; ++i){
            if (l[i] == ',') ++n;
        }
        int* steps = IntAllocator::allocate(n, sizeof(int));
        float* scales = DataCitemAllocator::allocate(n, sizeof(float));
        for(i = 0; i < n; ++i){
            int step    = UtilFunc::charToInt(l);
            float scale = UtilFunc::charToFloat(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->steps_ = steps;
        net->scales_ = scales;
    }

    void                 expInitialize(Network* net, NodeList* options){
        net->gamma_ = ConfigIO::configFindToFloat(options, "gamma", 1);
    }

    void                 sigInitialize(Network* net, NodeList* options){
        net->gamma_ = ConfigIO::configFindToFloat(options, "gamma", 1);
        net->step_  = ConfigIO::configFindToFloat(options, "step", 1);
    }
    void                 polyInitialize(Network* net, NodeList*options){}
    void                 randomInitialize(Network*net, NodeList*options){}

}

