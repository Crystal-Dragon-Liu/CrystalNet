#include "include/net/convolutional_layer.h"
#include "include/Utils/common.h"
#include "include/net/network.h"
#include "include/Utils/utils.h"
#include "include/algorithm/im2col.h"
#include "include/algorithm/gemm.h"
namespace CONVOLUTIONAL_OP{
    ConvolutionalLayer makeConvolutionalLayer(int batch, int h, int w, int c, int n, int size, int stride, int padding, ACTIVATION activation, int batch_normalize, int binary, int xnor, int adam){
        ConvolutionalLayer l = LayerOP::makeLayer();
        l.type = LAYER_TYPE::CONVOLUTIONAL; // type of layer.
        l.height = h;
        l.width = w;
        l.channel = c;
        l.filterNum = n;
        l.weightBinaryzation = binary;
        l.batchSize = batch;
        l.stride = stride;
        l.filterSize = size;
        l.padSize = padding;
        l.batchNormalize = batch_normalize;
        // initialize weights. num of weights = channels * filterNum * filterSize* filterSize.
        l.weights = new std::vector<float>(c*n*size*size);
        l.weightUpdates = new std::vector<float>(c*n*size*size);
        l.biases = new std::vector<float>(n);
        l.biasesUpdates = new std::vector<float>(n);
        l.numWeights = c*n*size*size;
        l.numBiases  = n;
        LayerOP::initializeWeightNormal(&l, c*size*size);
        // calculate the size of output by convolutional layer.
        int outputWidth = getConvOutputWidth(l);
        int outputHeight = getConvOutputHeight(l);
        l.outputHeight = outputHeight;
        l.outputWidth  = outputWidth;
        l.outputChannel = n;
        l.numOutputs = outputWidth * outputHeight * n;
        // calculate num of input.
        l.numInputs = l.width * l.height * l.channel;
        // initialize relative data.
        l.outputData = new std::vector<float>(l.batchSize * l.numOutputs);
        // l.outputData = ALLOC_FLOAT_PTR(l.batchSize * l.numOutputs);
        l.deltas    = new std::vector<float>(l.batchSize * l.numOutputs);
        // initialize functions for forwarding, backwarding, updating parameters.
        l.forward  = forwardConvLayer;
        l.backward = backwardConvLayer;
        l.update  = updateConvLayer;

        if(binary) LayerOP::binaryWeightInit(&l, c*n*size*size, n);
        if(xnor) LayerOP::xnorInit(&l, c*n*size*size);
        if(batch_normalize) LayerOP::batchNormalInit(&l, n);
        if(adam) LayerOP::adamInit(&l, c*n*size*size, n);

        //TODO GPU

        // initialize size of workspace
        l.workspaceSize = getWorkSpaceSize(l);
        l.activation = activation;
        PRINT_BLUE("conv ", n, size, "x", size, "/", stride, w, "x", h, "x", c, "-> ", l.outputWidth, "x", l.outputHeight, "x", l.outputChannel);
        return l;
    }
    
    int                 getConvOutputHeight(Layer l){
        return (l.height + l.padSize * 2  - l.filterSize) / l.stride + 1;
    }
    
    int                 getConvOutputWidth(Layer l){
        return  (l.width + l.padSize * 2 - l.filterSize ) / l.stride + 1;
    }
    
    void                forwardConvLayer(Layer l, Network network){
        int out_h = l.outputHeight;
        int out_w = l.outputWidth;
        // initialize outputs by zero.
        UtilFunc::initDataByCPU(l.numOutputs*l.batchSize, 0, l.outputData);
        // TODO binary op for inputs and weights??
        int m = l.filterNum;
        int k = l.filterSize*l.filterSize*l.channel; // size of each kernel in this Conv layer
        int n = out_h* out_w;

        std::vector<float>* weights = l.weights;
        std::vector<float>* workspace = network.workspace_;
        std::vector<float>* outputData = l.outputData;
		showConvLayerInfo(l);
        for(int i = 0; i < l.batchSize; ++i){
            // Convolutional Algorithm for each batch.
			// run img2col
            //network.inputData is initialized by forward_network.
            // workspace contains one re-ordered image data with 1-dimenion
            // a -> [l.channel * l.kernelSize* l.kernelSize, l.outputWidth * l.outputHeight]
            // b -> [l.filterNum, l.channel * l.kernelSize * l.kernelSize]
            im2ColCPU(network.inputData_, l.channel, l.height, l.width, l.filterSize, l.stride, l.padSize, workspace);
            GEMM_ALGO::gemm(false, false, m, n, k, 1, weights, k, workspace, n, 1, outputData, n);
            // update the pointer of outputData.
            outputData += m*n;
            // update the pointer of batchData.
            network.inputData_ += l.channel * l.height * l.width;
        }
        // TODO batch normalization
        // TODO activation
    }
    
    void                backwardConvLayer(Layer layer, Network network){}
    
    void                updateConvLayer(Layer layer, Network network){}
    
    size_t              getWorkSpaceSize(Layer l){
        return static_cast<size_t>(l.outputHeight * l.outputWidth * l.channel * l.filterSize * l.filterSize);
    }

    void				  showConvLayerRelativeMatrix(Layer l){
		 auto weights = l.weights;
		 auto outputData = l.outputData;
		 
		 int weightShape0 = l.filterNum;
		 int weightShape1 = l.channel*l.filterSize*l.filterSize;
		 int inputShape0 = l.channel*l.filterSize*l.filterSize;
		 int inputShape1 = l.outputWidth * l.outputHeight;
		 int outputShape0 = l.filterNum;
		 int outputShape1 = l.outputHeight * l.outputWidth;
		 
		 PRINT("Weights'shape in conv layer -> ", "[", weightShape0, ", ", weightShape1, "], ", weights->size(), " in total.");
		 PRINT("Regrouped inputs matrix 'shape -> [", inputShape0, ", ", inputShape1, "], ", inputShape0 * inputShape1, " in total.");
		 PRINT("Output matrix 'shape -> ", "[", outputShape0, ", ", outputShape1, "], ", outputData->size(), " in total.");

	}


	//TODO show the name of layer.
    void				  showConvLayerInfo(Layer l){
		PRINT("=========CONV 2D INFO==========");
		PRINT("Filters'shape -> ", "[",l.filterSize, ", ", l.filterSize, "]");
		PRINT("Size of filters -> ", l.filterNum);
		PRINT("Padding size -> ", l.padSize);
		PRINT("Input'shape -> [", l.channel, ", ",  l.height, ", ", l.width, "]");
		PRINT("Output'shape -> [", l.filterNum, ", ", l.outputHeight, ", ", l.outputWidth, "]");
		showConvLayerRelativeMatrix(l);
		PRINT("=========CONV 2D INFO==========");
	}
}
