#include "include/net/network.h"
#include "include/Utils/common.h"
#include "include/Utils/config_list.h"
// #include "include/Utils/utils.h"

#define CONFIG_FIND_I(...) ConfigIO::configFindToInt(__VA_ARGS__)
#define CONFIG_FIND_F(...) ConfigIO::configFindToFloat(__VA_ARGS__)
#define CONFIG_FIND_S(...) ConfigIO::configFindToStr(__VA_ARGS__)


/* some allocator for Layer*/
typedef NetSimpleAlloc<Layer, MyNetCAlloc> LayerAllocator;
#define ALLOC_LAYER(n) LayerAllocator::allocate(n, sizeof(Layer))
#define DEALLOC_LAYER(...) LayerAllocator::deallocate(__VA_ARGS__)


namespace NetworkOP{
    Network makeNetwork(int n){
        Network network;
        network.totalLayerNum_ = n;
        network.layers_ = ALLOC_LAYER(network.totalLayerNum_);
        network.seen_ = ALLOC_SIZE_PTR(1);
        network.cost_ = ALLOC_FLOAT_PTR(1);
        network.steps_ = nullptr;
        network.scales_ = nullptr;
        return network;
    }

    void freeNetwork(Network net){
        DEALLOC_LAYER(net.layers_);
        DEALLOC_SIZE_PTR(net.seen_);
        // SizeAllocator::deallocate(net.seen_);
        DEALLOC_FLOAT_PTR(net.cost_);
        // free the space for parameters
        freeNetworkParam(&net);
    }

    void  freeStepParam(Network* net){
        if(net->steps_ != nullptr) DEALLOC_INT_PTR(net->steps_);
        if(net->scales_ != nullptr) DEALLOC_FLOAT_PTR(net->scales_);
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
        //TODO some bugs to fix in method loadNetworkCommonConfig.
        // loadNetworkCommonConfig(options, &net);
        
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
        net->batch_ =           CONFIG_FIND_I(options,"batch", 1);
        net->learningRate_ =    CONFIG_FIND_F(options, "learning_rate", .001);
        net->decay_ =           CONFIG_FIND_F(options, "decay", .0001);
        net->momentum_ =        CONFIG_FIND_F(options, "momentum", .9);
        int subdivs =           CONFIG_FIND_I(options, "subdivisions", 1);
        net->timeSteps_ =       CONFIG_FIND_I(options, "time_steps", 1, true);
        net->noTruth_ =         CONFIG_FIND_I(options, "notruth", 0);
        net->batch_ /= subdivs;
        net->batch_ *= net->timeSteps_;
        net->subdivisions_ = net->subdivisions_;
        // parameters for adam algorithm
        net->adam_ =            CONFIG_FIND_I(options, "adam", 0);
        if(net->adam_){
            net->b1_ =          CONFIG_FIND_F(options, "B1", 0);
            net->b2_ =          CONFIG_FIND_F(options, "B2", 0);
            net->eps_ =         CONFIG_FIND_F(options, "eps", .00000001);
        }
        net->height_ =          CONFIG_FIND_I(options, "height", 0);
        net->width_ =           CONFIG_FIND_I(options, "width", 0);
        net->channel_ =         CONFIG_FIND_I(options, "channels", 0);
        net->numInputs_ =       CONFIG_FIND_I(options, "inputs", net->channel_ * net->height_ * net->width_, true);
        net->maxCrop_ =         CONFIG_FIND_I(options, "max_crop", net->width_ * 2, true);
        net->minCrop_ =         CONFIG_FIND_I(options, "min_crop", net->width_, true);
        net->center_ =          CONFIG_FIND_I(options, "center", 0, true);
        net->angle_ =           CONFIG_FIND_F(options, "angle", 0, true);
        net->aspect_ =          CONFIG_FIND_F(options, "aspect", 1, true);
        net->saturation_ =      CONFIG_FIND_F(options, "saturation", 1, true);
        net->exposure_ =        CONFIG_FIND_F(options, "exposure", 1, true);
        net->hue_ =             CONFIG_FIND_F(options, "hue", 0, true);
        // check input parameters
        if(!net->numInputs_ && !(net->height_ && net->width_ && net->channel_) ){
            UtilFunc::errorOccur("No input parameters supplied");}
        char* policy_s =        CONFIG_FIND_S(options, "policy", const_cast<char*>("constant"));
        net->learningRatePolicy_ = parseLearningRatePolicy(policy_s);
        //TODO initialize burn in ???
        net->burnIn_ = CONFIG_FIND_I(options, "burn_in", 0, true);
        net->power_  = CONFIG_FIND_F(options, "power", 4, true);
        // TODO parse learning rate policy and initialize involved parameters.
        initLrParam(net, options);
        net->maxBatches_ =        CONFIG_FIND_I(options, "max_batches", 0);
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
        net->step_ = CONFIG_FIND_I(options, "step", 1);
        net->scale_ = CONFIG_FIND_I(options, "scale", 1);
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
        int* steps = ALLOC_INT_PTR(n);
        float* scales = ALLOC_FLOAT_PTR(n);
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
        net->numSteps_ = n;
    }

    void                 expInitialize(Network* net, NodeList* options){
        net->gamma_ = CONFIG_FIND_F(options, "gamma", 1);
    }

    void                 sigInitialize(Network* net, NodeList* options){
        net->gamma_ = CONFIG_FIND_F(options, "gamma", 1);
        net->step_  = CONFIG_FIND_F(options, "step", 1);
    }
    void                 polyInitialize(Network* net, NodeList*options){}
    void                 randomInitialize(Network*net, NodeList*options){}
    void                 initializePolicy(LearningRatePolicy policy){
        switch (policy)
        {
        case LearningRatePolicy::STEP:
            /* code */
            break;
        
        default:
            break;
        }
    }
    void                 initLrParam(Network* net, NodeList* options){
        switch (net->learningRatePolicy_)
        {
        case LearningRatePolicy::STEP:{stepInitialize(net, options);break;}
        case LearningRatePolicy::STEPS:{stepsInitialize(net, options);break;}
        case LearningRatePolicy::EXP:{expInitialize(net, options);break;}
        case LearningRatePolicy::SIG:{sigInitialize(net, options);break;}
        case LearningRatePolicy::POLY:{polyInitialize(net, options);break;}
        default:
            break;
        }
    }

}

