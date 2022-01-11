#include "include/net/network.h"
#include "include/Utils/common.h"
#include "include/Utils/config_list.h"
// #include "include/Utils/utils.h"

/* some allocator for Layer*/
typedef NetSimpleAlloc<Layer, MyNetCAlloc> LayerAllocator;
namespace NetworkOP{
    Network makeNetwork(int n){
        Network network;
        network.totalLayerNum = n;
        network.layers = LayerAllocator::allocate(network.totalLayerNum, sizeof(Layer));
        network.seen = SizeAllocator::allocate(1, sizeof(int));
        network.cost = DataCitemAllocator::allocate(1, sizeof(float));
        return network;
    }

    void freeNetwork(Network net){
        LayerAllocator::deallocate(net.layers);
        SizeAllocator::deallocate(net.seen);
        DataCitemAllocator::deallocate(net.cost);
    }

    Network parseNetworkConfig(const char* fileName){
        // read network configuration.
        NodeList* sections = ConfigIO::readModelConfig(fileName);
        Node* node = sections->front_;
        if(!node) UtilFunc::errorOccur("no sections loaded from config file!");
        Network net = makeNetwork(sections->size_ - 1);
        // free all nodeList;
        NodeOP::freeNodeList(sections, UtilFunc::freeConfigSection);
        return net;
    }


}

