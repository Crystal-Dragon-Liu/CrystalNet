#ifndef NETWORK_H
#define NETWORK_H
#include <stddef.h>
#include "include/net/layer.h"
#include "include/Utils/tree.h"
#include <map>
#include <string>
#include <vector>

using LayerVector = std::vector<Layer*>;
/*@brief  learning rate policy. */
enum class LEARNING_RATE_POLICY{
    CONSTANT, STEP, EXP, POLY, STEPS, SIG, RANDOM
};

enum class NETWORK_MODE{
    TRAIN,
    INFER
};

typedef struct Network{
    int                 totalLayerNum_;
    int                 batch_; /// the batch size of image set.
	int					timeSteps_;
	int					subdivisions_;

    std::vector<size_t>*     seen_; /// num of image which have been processed. //! ptr
    float               learningRate_;
    float               momentum_;
    float               decay_;
	Layer*              layers_; //! ptr
    // int                 subdivisions_;

    int                 adam_;
    // parameters for adam algorithm. b1_, b2_, eps_
    float               b1_;
    float               b2_;
    float               eps_;

    int                 height_;
    int                 width_;
    int                 channel_;
    int                 numInputs_;
    int                 maxCrop_;
    int                 minCrop_;
    LEARNING_RATE_POLICY  learningRatePolicy_;
    // parameters for policy STEP, SIG
    int                 step_;
    float               scale_;
    // parameters for policy STEPS
    std::vector<int>*   steps_;
    // int*                steps_;  //! ptr
    std::vector<float>* scales_;
    // float*              scales_; //! ptr
    int                 numSteps_;
    // parameters for policy EXP, SIG
    float               gamma_;
    int                 maxBatches_;
    // Once burnIn_ is set, when number of updates is less than burnIn_
    // instead of using the configured learning rate update policy, the following formula is used
    // lr = base_lr * power(batch_num/burn_in, pwr)
    int                 burnIn_; 
    float               power_;
    float               clip_;
    //output
    int                 numOutputs_;
    std::vector<float>* outputData_;
    // float*              outputData_;
    int                 truth_; // no idea what this stands for
    std::vector<float>* truthData_;
    // float*              truthData_; // the label data for inputData_;
    // inputData_ stands for the input data of current layer, this kind of data could be from the output of former layer in network.
    std::vector<float>* inputData_;
    // float*              inputData_;
    std::vector<float>* deltas_;
    // float*              deltas_; 
    // workspace 
    std::vector<float>* workspace_;
    // TrainMode or InferMode
    NETWORK_MODE         networkMode_;
    std::vector<float>* cost_;
} Network;

typedef struct SizeParams{
    size_t batch;
    size_t inputs;
    size_t width;
    size_t height;
    size_t channals;
    size_t index;
    size_t time_steps;
    Network net;
}SizeParams;

struct ConfigSection;
struct NodeList;

typedef Layer (*parseNetLayerFunc)(NodeList *options, SizeParams& params);
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
    extern void                 freeNetwork(Network* net);

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
    extern LEARNING_RATE_POLICY   parseLearningRatePolicy(char* policy);

    /*
        @brief resize the network according to new w and new h.
    */
    extern void                 resizeNetwork(Network* net, int w, int h);

    /* @brief functions for initializing the parameters for learning rate policy*/
    extern void                 initializePolicy(LEARNING_RATE_POLICY policy);

    extern void                 stepInitialize(Network* net, NodeList* options);

    extern void                 stepsInitialize(Network* net, NodeList* options);

    extern void                 expInitialize(Network* net, NodeList* options);

    extern void                 sigInitialize(Network* net, NodeList* options);

    extern void                 polyInitialize(Network* net, NodeList*options);

    extern void                 randomInitialize(Network*net, NodeList*options);

    /* @brief functions for initializing involving parameters via LearningRatePolicy expected. */
    extern void                 initLrParam(Network* net, NodeList* options);

	/*
	 	 @brief get the function pointer(parseNetLayerFunc) through enum LAYER_TYPE which supports a bunch of layers. parseNetLayerFunc could be used to initialize the corresponding layers parsed by this function.
		 @param:
		 		LAYER_TYPE a enum class including various layers CrystalNet.
	 */
    extern parseNetLayerFunc    getParseNetFunc(LAYER_TYPE layerType);

	/*
	 * 	@brief initialize a convolutional layer through configuration and the information from last layer passed through.
	 * 	@param:
	 * 			options: a class that contains group of parameters of convolutional layers.
	 * 			params:  instance of SizeParams contains the shape information from last layer. In the most case, we initialized the height and width of output and the input by this instances. 
	 */
    extern Layer                parseConvolutionalLayer(NodeList* options, SizeParams& params);

     /*
	  * @brief initialize a fully connected layer through configuration and the info    rmation from last layer passed through.
	  * @param:
      *          options: a class that contains group of parameters of fully connected layers.
      *          params:  instance of SizeParams contains the shape information fr om last layer. In the most case, we initialized the height and width of output and     the input by this instances. 
	 */
    extern Layer                parseFullyConnectedLayer(NodeList* options, SizeParams& params);

    /*
        @brief get the layer of output.
    */
    extern Layer                getOutputLayer(Network net);

    /*
        @brief the implement of forward propagation of network
    */
    extern std::vector<float>*    predictNetwork(Network net, std::vector<float>* inputData);
    /*
        @brief method forwardLayers is called by forwardNetwork().
    */
    extern void                 forwardNetwork(Network net);

}

#endif
