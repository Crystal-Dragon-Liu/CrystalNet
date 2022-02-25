#ifndef FULLYCONNECTED_LAYER_H
#define FULLYCONNECTED_LAYER_H
#include "include/net/layer.h"
typedef Layer FullyConnectedLayer;

namespace FullyConnectedLayer_OP{
    extern FullyConnectedLayer makeFullyConnectedLayer(int batch, int inputs, int outputs, ACTIVATION activation, int batch_normalize);
    extern void                forwardFullyConnectedLayer(Layer layer, Network network);
    extern void                backwardFullyConnectedLayer(Layer layer, Network network);
    extern void                updateFullyConnectedLayer(Layer layer, Network network);
}
#endif