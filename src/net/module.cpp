#include "include/net/module.h"
namespace CrystalNet{
		Module::Module(){
				weights_	=	nullptr;
				biases_		=	nullptr;
				weightsUpdate_ = nullptr;
				biasesUpdate_ = nullptr;
				outputData_	=	nullptr;
		}
		Module::~Module(){
				if(weights_) delete weights_;
				if(biases_) delete biases_;
				if(weightsUpdate_) delete weightsUpdate_;
				if(biasesUpdate_) delete biasesUpdate_;
				if(outputData_) delete outputData_;
		}
}
