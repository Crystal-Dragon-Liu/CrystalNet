#ifndef MODULE_H
#define MODULE_H
#include <vector>
namespace CrystalNet{

/*
 * @brief 	Module is a base class containing wieghts and biases which are implemented by std::vector.
 */
class Module{
		public:
				/*
				 * @brief	Module contains fundamental data in the basic layers.
				 * 			
				 */
				std::vector<float>* weights_;
				std::vector<float>* biases_;
				std::vector<float>* weightsUpdate_;
				std::vector<float>* biasesUpdate_;
				std::vector<float>* outputData_;

				/*
				 * @brief 	contructor of Module class to initialize weights and biases, both of them are set to null by default.
				 */
				Module();
				~Module();
};

}

#endif
