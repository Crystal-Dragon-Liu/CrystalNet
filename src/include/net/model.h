#pragma once
#include <vector>
#include "include/net/module.h"
struct NodeList;
struct ConfigSection;
// using namespace std;
namespace CrystalNet{


class Optimizer{
		public:
				Optimizer(){learningRate_ = .001;}
				~Optimizer(){}
				float learningRate_;
};

class AdamOptimizer: public Optimizer{
		public:
				float momentum_;
				float decay_;
				AdamOptimizer(): Optimizer(){
					momentum_ = .9;
					decay_ = .0001;
				}
};

class Model;

class BasicLayer: public Module{
			public:
					/*
					 * @brief BasicLayer is based class for all kinds of layers. 
					 */
					BasicLayer();
					virtual ~BasicLayer();
					//forward/backward/update function ptr.	
					void (*forward)(BasicLayer layer, Model network);
					void (*backward)(BasicLayer layer, Model network);
					void (*update)(BasicLayer layer, Model network);
					//TODO the shape of crystalnet
};

class Model{
		public:
				Model(size_t layerNum);
				~Model();
				//TODO	refact operator=, implement the assignment operator through
				Model& operator=(const Model& model);
		public:
				int totalLayerNum_;
				int batchSize_;
				int timeSteps_;
				int subdivisions_;
				Optimizer*	optimizer_;
				std::vector<BasicLayer*>* layers_;

				std::vector<size_t>* seen_;
				std::vector<float>* cost_;

				std::vector<float>* inputData_;
				std::vector<float>* outputData_;
				std::vector<float>* workspace_;
				std::vector<float>* deltas_;
		public:
				//TODO	refact the copy constructor
				Model(const Model& model);
};


typedef struct LastParam{
		size_t	batch_;
		size_t	numInputs_;
		size_t	height_;
		size_t	width_;
		size_t	channels_;
		Model	net_;
}LastParam;


/*
 * @brief	free the Model through function freeModel.
 * 			after training, testing or inference,
 * 			the initialized model should be released.
 */
extern void freeModel(Model* model);

/*
 * @brief	parse configuration file with format '.cfg'
 */
extern Model parseModelConfig(const char* fileName);

/*
 * @brief load general configuration and config the operator.
 * 			this method is used to initialize optimizer.
 */
extern void loadCommonConfig(NodeList* nodelist, Model* model);

/*
 * @brief check the Model Config to ensure the configuration works.
 */
extern bool checkModelConfig(ConfigSection* section);

}
