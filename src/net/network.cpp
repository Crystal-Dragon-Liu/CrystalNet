#include "include/net/network.h"
#include "include/Utils/common.h"
#include "include/Utils/config_list.h"
#include "include/net/activations.h"
// #include "include/Utils/utils.h"
#include "include/net/convolutional_layer.h"
#include "include/net/fullyconnected_layer.h"

#define CONFIG_FIND_I(...) ConfigIO::configFindToInt(__VA_ARGS__)
#define CONFIG_FIND_F(...) ConfigIO::configFindToFloat(__VA_ARGS__)
#define CONFIG_FIND_S(...) ConfigIO::configFindToStr(__VA_ARGS__)


/* some allocator for Layer*/
typedef NetSimpleAlloc<Layer, MyNetCAlloc> LayerAllocator;
#define ALLOC_LAYER(n) LayerAllocator::allocate(n, sizeof(Layer))
#define DEALLOC_LAYER(...) LayerAllocator::deallocate(__VA_ARGS__)



namespace NetworkOP{
    Network                 makeNetwork(int n){
        Network network;
        network.totalLayerNum_ = n;
        network.layers_ = ALLOC_LAYER(network.totalLayerNum_);
        // network.seen_ = ALLOC_SIZE_PTR(1);
        network.seen_ = new std::vector<size_t>(1, 0);
        // network.cost_ = ALLOC_FLOAT_PTR(1);
        network.cost_ = new std::vector<float>(1, 0);
        network.steps_ = nullptr;
        network.scales_ = nullptr;
        network.outputData_ = nullptr;
        network.inputData_ = nullptr;
        network.truthData_ = nullptr;
        network.workspace_ = nullptr;
        network.deltas_ = nullptr;
        return network;
    }

    void                    freeNetwork(Network* net){
        //TODO delete layers.
        for(int i = 0; i < net->totalLayerNum_;i++){
            LayerOP::freeLayer(net->layers_[i]);
        }
        if(net->layers_) DEALLOC_LAYER(net->layers_);
        if(net->seen_)      delete net->seen_;
        if(net->cost_)      delete net->cost_; 
        if(net->inputData_) delete net->inputData_;
        if(net->truthData_) delete net->truthData_;
        if(net->workspace_) delete net->workspace_;
        if(net->deltas_)    delete net->deltas_;
        // free the space for parameters
        freeNetworkParam(net);
    }

    void                    freeStepParam(Network* net){
        if(net->steps_ != nullptr) delete net->steps_;
        if(net->scales_ != nullptr) delete net->scales_;
    }

    void                    freeNetworkParam(Network* net){freeStepParam(net);}


    void                 resizeNetwork(Network* net, int w, int h){
        
    }


    Network                 parseNetworkConfig(const char* fileName){
        // read network configuration.
        NodeList* sections = ConfigIO::readModelConfig(fileName);
        Node* node = sections->front_; // only [net] config would be loaded.
        if(!node) UtilFunc::errorOccur("no sections loaded from config file!");
        Network net = makeNetwork(sections->size_ - 1);

        ConfigSection* s = reinterpret_cast<ConfigSection*>(node->value_);
        NodeList* options = s->config;
        if(!checkNetworkConfig(s)) UtilFunc::errorOccur("First section must be [net] or [network]");
        // std::cout << "parsing network config" << std::endl;
        //TODO some bugs to fix in method loadNetworkCommonConfig.
        loadNetworkCommonConfig(options, &net);
        SizeParams params;
        params.height = net.height_;
        params.width = net.width_;
        params.channals = net.channel_;
        params.inputs = net.numInputs_;
        params.batch = net.batch_;
        params.time_steps = net.timeSteps_;
        params.net = net;
        // parse layer parameters
        size_t workplaceSize = 0;
        node = node->next_;
        int count = 0;
        while(node){
            //TODO config the layer.
            params.index = count;
            PRINT_LOG("loading layer ", count, "..");
            s = reinterpret_cast<ConfigSection*>(node->value_);
            options = s->config;
            // define layers
            LAYER_TYPE  layerType = LayerOP::parseLayerType(s->type);
            parseNetLayerFunc f = getParseNetFunc(layerType);
            if(f == nullptr){
                PRINT_LOG("unknown layer ", s->type);
                node = node->next_;
                continue;
            }
            Layer l = f(options, params);
            // no idea what for.
            l.clip = net.clip_;
            l.truth = CONFIG_FIND_I(options, "truth", 0, true);
            l.onlyForward = CONFIG_FIND_I(options, "onlyforward", 0, true);
            l.stopBackward = CONFIG_FIND_I(options, "stopbackward", 0, true);
            l.noDataLoad = CONFIG_FIND_I(options, "dontload", 0, true);
            l.noScaleLoad = CONFIG_FIND_I(options, "dontloadscales", 0, true);
            l.learningRateScale = CONFIG_FIND_F(options, "learning_rate", 1, true);
            l.smooth = CONFIG_FIND_F(options, "smooth", 0, true);
            net.layers_[count] = l;
            NodeOP::printUnusedOptions(options);
            if(l.workspaceSize >workplaceSize) workplaceSize = l.workspaceSize;
            node = node->next_;
            count++; 
            if(node){
                params.height = l.outputHeight;
                params.width = l.outputWidth;
                params.channals = l.outputChannel;
                params.inputs = l.numOutputs;
            }
            // PRINT("-- finished to load layer ", count, "..");
        }
        PRINT_LOG("finished to load all layers.");
        //TODO process output layer.
        Layer outputLayer = getOutputLayer(net);
        net.numOutputs_ = outputLayer.numOutputs;
        net.truth_ = outputLayer.numOutputs;
        if(net.layers_[net.totalLayerNum_-1].truth) net.truth_ = net.layers_[net.totalLayerNum_-1].truth;
        net.outputData_ = outputLayer.outputData;
        net.inputData_  = new std::vector<float>(net.numInputs_*net.batch_);
        net.truthData_  = new std::vector<float>(net.truth_*net.batch_);
        // free all nodeList;
        NodeOP::freeNodeList(sections, UtilFunc::freeConfigSection);
        //TODO GPU
        if(workplaceSize){
            net.workspace_ = new std::vector<float>(workplaceSize);
            // reinterpret_cast<float*>(calloc(1, workplaceSize));
        }
        return net;
    }

    bool                    checkNetworkConfig(ConfigSection* section){
        return (strcmp(section->type, "[net]")==0
            || strcmp(section->type, "[network]")==0);
    }

    void                    loadNetworkCommonConfig(NodeList* options, Network* net){
        using namespace std;
        PRINT("<---loading common parameters!--->");
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
        PRINT("<---finished to load common parameters!--->");
    }

    LEARNING_RATE_POLICY      parseLearningRatePolicy(char* policy){
        if (strcmp(policy, "random")==0) return LEARNING_RATE_POLICY::RANDOM;
        if (strcmp(policy, "poly")==0) return LEARNING_RATE_POLICY::POLY;
        if (strcmp(policy, "constant")==0) return LEARNING_RATE_POLICY::CONSTANT;
        if (strcmp(policy, "step")==0) return LEARNING_RATE_POLICY::STEP;
        if (strcmp(policy, "exp")==0) return LEARNING_RATE_POLICY::EXP;
        if (strcmp(policy, "sigmoid")==0) return LEARNING_RATE_POLICY::SIG;
        if (strcmp(policy, "steps")==0) return LEARNING_RATE_POLICY::STEPS;
        PRINT("Couldn't find policy <", policy, ">, going with constant\n");
        return LEARNING_RATE_POLICY::CONSTANT;
    }

    void                    stepInitialize(Network* net, NodeList* options){
        net->step_ = CONFIG_FIND_I(options, "step", 1);
        net->scale_ = CONFIG_FIND_I(options, "scale", 1);
    }

    void                    stepsInitialize(Network* net, NodeList* options){
        char *l = ConfigIO::configFind(options, "steps");   
        char *p = ConfigIO::configFind(options, "scales");   
        if(!l || !p) UtilFunc::errorOccur("STEPS policy must have steps and scales in cfg file");
        int len = strlen(l);
        int n = 1;
        int i;
        for(i = 0; i < len; ++i){
            if (l[i] == ',') ++n;
        }
        // int* steps = ALLOC_INT_PTR(n);
        net->steps_ = new std::vector<int>(n);
        net->scales_ = new std::vector<float>(n);
        // float* scales = ALLOC_FLOAT_PTR(n);
        for(i = 0; i < n; ++i){
            int step    = UtilFunc::charToInt(l);
            float scale = UtilFunc::charToFloat(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            (*net->steps_)[i] = step;
            (*net->scales_)[i] = scale;
        }
        // net->steps_ = steps;
        // net->scales_ = scales;
        net->numSteps_ = n;
    }

    void                    expInitialize(Network* net, NodeList* options){
        net->gamma_ = CONFIG_FIND_F(options, "gamma", 1);
    }

    void                    sigInitialize(Network* net, NodeList* options){
        net->gamma_ = CONFIG_FIND_F(options, "gamma", 1);
        net->step_  = CONFIG_FIND_F(options, "step", 1);
    }
    
    void                    polyInitialize(Network* net, NodeList*options){}
    
    void                    randomInitialize(Network*net, NodeList*options){}
    
    void                    initializePolicy(LEARNING_RATE_POLICY policy){
        switch (policy)
        {
        case LEARNING_RATE_POLICY::STEP:
            /* code */
            break;
        
        default:
            break;
        }
    }
    
    void                    initLrParam(Network* net, NodeList* options){
        switch (net->learningRatePolicy_)
        {
        case LEARNING_RATE_POLICY::STEP:{stepInitialize(net, options);break;}
        case LEARNING_RATE_POLICY::STEPS:{stepsInitialize(net, options);break;}
        case LEARNING_RATE_POLICY::EXP:{expInitialize(net, options);break;}
        case LEARNING_RATE_POLICY::SIG:{sigInitialize(net, options);break;}
        case LEARNING_RATE_POLICY::POLY:{polyInitialize(net, options);break;}
        default:
            break;
        }
    }

    parseNetLayerFunc       getParseNetFunc(LAYER_TYPE layerType){
        switch (layerType)
        {
        case LAYER_TYPE::CONVOLUTIONAL:{
            return parseConvolutionalLayer;
        }
        case LAYER_TYPE::CONNECTED:{
            return parseFullyConnectedLayer;
        }
        default:
            break;
        }
        return nullptr;
    }
    
    Layer                   parseFullyConnectedLayer(NodeList* options, SizeParams& params){
        int output = CONFIG_FIND_I(options, "output", 1);
        char*  activationStr     =   CONFIG_FIND_S(options, "activation", "logistic");
        ACTIVATION activation = ACT_OP::getActivation(activationStr);
        int batchNormalzation = CONFIG_FIND_I(options, "batch_normalize", 0, true);
        Layer l = FullyConnectedLayer_OP::makeFullyConnectedLayer(params.batch, params.inputs, output, activation, batchNormalzation);
        return l;
    }

    Layer                   parseConvolutionalLayer(NodeList* options, SizeParams& params){
        int n       =   CONFIG_FIND_I(options, "filters", 1); // number of filter.
        int size    =   CONFIG_FIND_I(options, "size", 1); // size of filter.
        int stride  =   CONFIG_FIND_I(options, "stride", 1);//  stride.
        int pad     =   CONFIG_FIND_I(options, "pad", 0, true); //    padding the data or not
        int padding =   CONFIG_FIND_I(options, "padding", 0, true); // length of padding, useless code.
        if(pad){
            padding = size / 2;
        }
        char*  activationStr     =   CONFIG_FIND_S(options, "activation", "logistic");
        ACTIVATION activation = ACT_OP::getActivation(activationStr);
        int batch, height, width, channel;
        height = params.height;
        width = params.width;
        channel = params.channals;
        batch = params.batch; // batch is always set to a fixed value.
        if(!(height && width && channel)) // if one of these value is set to zero, params is invalid.
            throw NetworkException("invalid input from former layer.");
        int batchNormalzation = CONFIG_FIND_I(options, "batch_normalize", 0, true);
        int binary = CONFIG_FIND_I(options, "binary", 0, true); // the binary operation for weights works or not.
        int xnor = CONFIG_FIND_I(options, "xnor", 0, true); //  the binary operation for weights and inputs works or not.
        //TODO build convolutional layer.
        ConvolutionalLayer l = CONVOLUTIONAL_OP::makeConvolutionalLayer(batch,height,width,channel,n,size,stride,padding,activation, batchNormalzation, binary, xnor, params.net.adam_);
        l.flipped = CONFIG_FIND_I(options, "flipped", 0, true);
        l.dot       = CONFIG_FIND_I(options, "dot", 0, true);
        if(params.net.adam_){
        l.B1 = params.net.b1_;
        l.B2 = params.net.b2_;
        l.eps = params.net.eps_;
        }
        return l;
    }

    Layer                   getOutputLayer(Network net){
        int i;
        for(i = net.totalLayerNum_ -1; i>=0; i--){
            if(net.layers_[i].type != LAYER_TYPE::COST){
                break;
            }
        }
        return net.layers_[i];
    }

    float*                 predictNetwork(Network net, float* inputData){
        // net.inputData_ = inputData;
        // // set the mode of network
        // net.networkMode_ = NETWORK_MODE::INFER;
        // forwardNetwork(net);
		// return net.outputData_;
        return nullptr;
    }

    std::vector<float>*    predictNetwork(Network net, std::vector<float>* inputData){
        net.inputData_ = inputData;
        net.networkMode_ = NETWORK_MODE::INFER;
        forwardNetwork(net);
        return net.outputData_;
    }

    void                 forwardNetwork(Network net){
        int layerNum = net.totalLayerNum_;
        for(int i = 0; i < layerNum; i++){
            Layer* l = &(net.layers_[i]);
            // assignment for delta.
            if(l->deltas)
            {
                //TODO process l->deltas.
            }
            l->forward(*l, net);
            net.inputData_ = l->outputData;
            //TODO figure out what the l->truth is.
            if(l->truth)
                net.truthData_ = l->outputData;
        }
        //TODO calculate cost.
    }


}

