#include "include/net/network.h"
#include "include/Utils/common.h"
/* some allocator for Layer*/
typedef NetSimpleAlloc<Layer, MyNetAlloc> LayerAllocator;
namespace NetworkOP{
    Network makeNetwork(int n){
        Network network = {0};
        network.totalLayerNum = n;
        network.layers = LayerAllocator::allocate(network.totalLayerNum, sizeof(Layer));
        network.seen = SizeAllocator::allocate(1, sizeof(int));
        network.cost = DataCitemAllocator::allocate(1, sizeof(float));
        return network;
    }
}