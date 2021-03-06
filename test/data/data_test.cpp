#include "gtest/gtest.h"
#include <vector>
#include <algorithm>
#include "include/Utils/list.h"
#include "include/Utils/config_list.h"
#include "include/data/matrix.h"
#include "include/data/image.h"
#include "include/net/network.h"
#include "include/net/model.h"
using namespace ImageFunc;
Matrix* initMatrixWithValue(size_t row, size_t col, float value){
	Matrix* matrix = new Matrix(row, col);
	auto data = matrix->getData();
	// initialize data with 1.
	for(size_t i = 0; i < matrix->getRowCount();i++){
		for(size_t j = 0; j < matrix->getColCount(); j++){
			data[i][j] = value;
		}
	}
	return matrix;
}

/*
	Test List:
	1. read the .data configuration file.
	2. parse the configuration and insert it into NodeList.
	3. find a value according to a key provided.
	4. free NodeList.
*/
TEST(BasicListTest, DatasetConfigTest){
	
	const std::string filename("/Users/crystal-dragon-lyb/CrystalNet/cfg/cifar.data");
	NodeList* list = ConfigIO::readDataAndCfg(filename);
	if(!list) std::cout << "failed to load configuration." << std::endl;
	// callbackPrintNodes pCallback = UtilFunc::printkyp;
	// NodeOP::printAllNodes(list, pCallback);
	char key_search[8] = "classes";
	char* val_search = ConfigIO::configFind(list, key_search);
	UtilFunc::printCharArray(val_search);
	std::cout << std::endl;
	NodeOP::freeNodeList(list);
}

/*
	Test List:
	1. create a 2d array 'matrix'.
	2. create a new 'matrix_2' with the data copied from 'matrix'.
	3. free two these Matrix obj.
*/
TEST(BasicListTest, MatrixTest_1)
{
	// Matrix* matrix = new Matrix(5, 5);
	// auto data = matrix->getData();
	// // initialize data with 1.
	// for(size_t i = 0; i < matrix->getRowCount();i++){
	// 	for(size_t j = 0; j < matrix->getColCount(); j++){
	// 		data[i][j] = 1;
	// 	}
	// }
	Matrix* matrix = initMatrixWithValue(5, 5, 1);
	Matrix* matrix_2 = new Matrix(5, 5);
	*matrix_2 = *matrix;
	float scale = 2.0;
	MatrixFunc::scaleMatrix(matrix_2, scale);
	MatrixFunc::printMatrix(*matrix);
	MatrixFunc::matrixAddMatrix(matrix, matrix_2);
	MatrixFunc::printMatrix(*matrix_2);
	delete matrix;
	delete matrix_2;
}


/*
	Test List:
	1. create a matrix with index 1
	2. create a matrix with index 2 which are sampled from matrix 1.
	3. free them correctly.
*/
TEST(BasicListTest, MatrixTest_2){
	Matrix* matrix_1 	=	initMatrixWithValue(5, 5, 1);
	Matrix* matrix_2 	= new Matrix;
	MatrixFunc::holdOutMatrix(matrix_2, matrix_1, 3);
	auto val = matrix_1->getData();
	val[1][2] = 3;
	MatrixFunc::printMatrix(*matrix_1);
	MatrixFunc::printMatrix(*matrix_2);
	delete matrix_1;
	delete matrix_2;
}

/*
	Test List:
	1. Create a image.
	2. Free image created from 1.
	3. try to load a image and return a image obj.
	4. resize the loaded image.
	5. call the loadImage, and free the image returned.
*/
TEST(BasicListTest, ImageLoadTest){
	// test 1
	Image* image = ImageFunc::makeImage(2, 2, 2);
	// test 2
	ImageFunc::freeImage(image);	

	const std::string image_path("/Users/crystal-dragon-lyb/CrystalNetDataSet/cifar/test/0_cat.png");
	// test 3
	Image* new_image = ImageFunc::_loadImage(image_path.data(), 3);	
	// test 4
	Image* resized_image = ImageFunc::resizeImage(new_image, 64, 64);	
	ImageFunc::freeImage(new_image);
	std::cout << "image loaded with size [" << resized_image->getWidth() << ", " << resized_image->getHeight() << ", " << resized_image->getChannels() << "]" << std::endl;
	ImageFunc::freeImage(resized_image);
	// test 5
	new_image = ImageFunc::loadImage(image_path.data(), 64, 64, 3);
	ImageFunc::freeImage(new_image);
}


/*
	Test list:
	1. Create a network for classifying cifar dataset.
*/
TEST(BasicListTest, ModelLoadTest){
	std::string fileName("/Users/crystal-dragon-lyb/CrystalNet/cfg/classifier_model.cfg");
	
	Network net = NetworkOP::parseNetworkConfig(fileName.data());
	NetworkOP::freeNetwork(&net);
}

/*
	Test list:
	1. Read cifar.data
	2. Read an image to Read.
*/

TEST(BasicListTest, ModelLoadAndForwardTest){
	// Read cifar.data.
	std::string fileName("/Users/crystal-dragon-lyb/CrystalNet/cfg/cifar.data");
	NodeList* dataConfig = ConfigIO::readDataAndCfg(fileName);
	if(!dataConfig){
		LOG("failed to load configuration.");
		exit(0);
	}
	NodeOP::printAllNodes(dataConfig, UtilFunc::printkyp);

		// Read a network
	std::string modelPath("/Users/crystal-dragon-lyb/CrystalNet/cfg/classifier_test.cfg");
	Network net = NetworkOP::parseNetworkConfig(modelPath.data());

	// Read an image.
	std::string imgPath("/Users/crystal-dragon-lyb/CrystalNetDataSet/cifar/test/0_cat.png");
	Image* new_image = ImageFunc::loadImage(imgPath.data());
	int size = net.width_;
	Image* resized = ImageFunc::resizeImageMin(new_image, size);
	ImageFunc::printShape(resized);
	std::vector<float>* X = resized->getData();
	PRINT("resized image' shape", X->size());

	auto prediction = NetworkOP::predictNetwork(net, X);
	PRINT("prediction'shape -> ", prediction->size());
    // int x_len = UtilFunc::getLengthOfArray(X);
	// PRINT("input data [2351] -> ", X[2351]);
	// PRINT("input data's length -> ", x_len);
	// release allocated space 
	NodeOP::freeNodeList(dataConfig, UtilFunc::freeKyp);
	if(resized->getData() != new_image->getData()) ImageFunc::freeImage(resized);
	ImageFunc::freeImage(new_image);
	NetworkOP::freeNetwork(&net);
}


/*
	Test list:
	1. test the Model, check its constructor and desturctor.
	2. test the parseNetworkConfig.
*/
TEST(BasicModelTest, ModelTest){
		CrystalNet::Model model_1(1);
		CrystalNet::Model model_2(2);
		model_1 = model_2;
		CrystalNet::freeModel(&model_1);
}
