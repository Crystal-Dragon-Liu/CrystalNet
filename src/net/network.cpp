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
        return network;
    }

    void freeNetwork(Network net){
        LayerAllocator::deallocate(net.layers_);
        SizeAllocator::deallocate(net.seen_);
        DataCitemAllocator::deallocate(net.cost_);
    }

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
        std::cout << "<---- loading common parameters ---->" << std::endl;
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
    }


}

