#include "include/net/model.h"
#include "include/Utils/config_list.h"
namespace CrystalNet{
    Model::Model(size_t layerNum){
        totalLayerNum_ = layerNum;
					//initialize layers;
					layers_ = new std::vector<BasicLayer*>(layerNum, nullptr);
					// initialize seens;
					seen_	=	new std::vector<size_t>(1, 0);
					cost_	=	new std::vector<float>(1, 0);	
					// initialize dataset and outputs.
					inputData_ = nullptr;
					outputData_ = nullptr;
					workspace_ = nullptr;
					deltas_ = nullptr;
					// initialize optimizer
					// it would be set to null by default. 
					// the optimizer of a network could be specified by parse function.
					optimizer_ = nullptr;
    }

    Model::~Model(){}

	Model& Model::operator=(const Model& model){
					//TODO maybe this is not safe enough.
					freeModel(this);
					layers_	=	model.layers_;
					seen_	=	model.seen_;
					cost_	=	model.cost_;
					inputData_	=	model.inputData_;
					outputData_	=	model.outputData_;
					workspace_	=	model.workspace_;
					deltas_		=	model.deltas_;
					optimizer_	=	model.optimizer_;
					return *this;
	}
	
	Model::Model(const Model& model){
					freeModel(this);
					this->layers_	=	model.layers_;
					this->seen_	=	model.seen_;
					this->cost_	=	model.cost_;
					this->inputData_	=	model.inputData_;
					this->outputData_	=	model.outputData_;
					this->workspace_	=	model.workspace_;
					this->deltas_	=	model.deltas_;
					this->optimizer_	=	model.optimizer_;
	}
	
	void freeModel(Model* model){
					
					for(std::vector<BasicLayer*>::size_type i = 0; i < model->layers_->size(); i++){
							if((*model->layers_)[i]){
								// if layers[i] should be released if
								// the relative space has been allocated
								// while parsing the layer through .data
								delete (*model->layers_)[i];
								(*model->layers_)[i] = nullptr; // reverse the layers[i]
							}
					}
					delete model->layers_;
					delete model->seen_;
					delete model->cost_;
					if (model->inputData_)
					{
							delete model->inputData_;
							model->inputData_ = nullptr;
					}
					if (model->outputData_){
							delete model->outputData_;
							model->outputData_ = nullptr;
					}
					if (model->workspace_){ 
							delete model->workspace_;
							model->workspace_ = nullptr;
					}
					if (model->deltas_) {
							delete model->deltas_;
							model->deltas_ = nullptr;
					}
					if (model->optimizer_) {
							delete model->optimizer_;
							model->optimizer_ = nullptr;
					}
	}

	// TODO parseModelConfig	
	Model parseModelConfig(const char* fileName){
				// read network configuration from .cfg file.
				NodeList* sections = ConfigIO::readModelConfig(fileName);
				// parse the configuration [net]
				Node* node = sections->front_;	
				NodeOP::checkNode(node); // if we failed to load [net], just terminate pragram.
				Model net = Model(sections->size_-1);
				// TODO load basic configuration for net.
				ConfigSection* s = reinterpret_cast<ConfigSection*>(node->value_);
				NodeList* options = s->config;
				if(!checkModelConfig(s)) UtilFunc::errorOccur("First section must be [net] or [network]");
				// TODO implementation of function loadCommonConfig
				loadCommonConfig(options, &net);
				// TODO create temp structure to save the config of 
				// each layer.

				
				return net;
	}
	
	void loadCommonConfig(NodeList* nodelist, Model* model){
				
	}

	bool checkModelConfig(ConfigSection* section){
				return (strcmp(section->type, "[net]") == 0
						|| strcmp(section->type, "[network]") == 0);
	}

}
