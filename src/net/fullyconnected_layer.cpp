#include "include/net/fullyconnected_layer.h"
#include "include/Utils/common.h"
#include "include/net/network.h"
#include "include/Utils/utils.h"
namespace FullyConnectedLayer_OP{

    FullyConnectedLayer makeFullyConnectedLayer(int batch, int inputs, int outputs, ACTIVATION activation, int batch_normalize){
        FullyConnectedLayer l = LayerOP::makeLayer();
        l.type = LAYER_TYPE::CONNECTED;
        l.numInputs = inputs;
        l.numOutputs = outputs;
        l.batchSize = batch;
        l.batchNormalize = batch_normalize;
        l.height = 1;
        l.width  = 1;
        l.channel = inputs;
        l.outputHeight = 1;
        l.outputWidth = 1;
        l.outputChannel = outputs;
        // set output data.
        l.outputData = new std::vector<float>(batch * outputs);
        l.deltas = new std::vector<float>(batch * outputs);
        // set parameters for update.
        l.weightUpdates = new std::vector<float>(inputs * outputs);
        l.biasesUpdates = new std::vector<float>(outputs);
        
        // set parameters
        l.weights = new std::vector<float>(inputs* outputs);
        l.biases = new std::vector<float>(outputs);

        // set forward, backward and update function.
        l.forward = forwardFullyConnectedLayer;
        l.backward = backwardFullyConnectedLayer;
        l.update = updateFullyConnectedLayer;

        // initialize weights
        LayerOP::initializeWeightUniform(&l, inputs, outputs*inputs);
        // initialize bias
        LayerOP::zeroBiases(&l, outputs);
        if(batch_normalize) LayerOP::batchNormalInit(&l, outputs);
        l.activation = activation;
        fprintf(stderr, "connected                            %4d  ->  %4d\n", inputs, outputs);
        return l;
    }

    void                forwardFullyConnectedLayer(Layer layer, Network network){}
    
    void                backwardFullyConnectedLayer(Layer layer, Network network){}
    
    void                updateFullyConnectedLayer(Layer layer, Network network){}
}
