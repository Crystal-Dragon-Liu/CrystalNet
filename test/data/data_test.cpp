#include "gtest/gtest.h"
#include <vector>
#include <algorithm>
#include "include/Utils/list.h"
#include "include/Utils/config_list.h"
#include "include/data/matrix.h"
#include "include/data/image.h"
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
TEST(BasicListTest, DISABLED_Test1){
	const std::string filename("/root/test_data/coco.data");
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
TEST(BasicListTest, DISABLED_Test2)
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
TEST(BasicListTest, DISABLED_Test3){
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
TEST(BasicListTest, Test4){
	// test 1
	Image* image = ImageFunc::makeImage(2, 2, 2);
	// test 2
	ImageFunc::freeImage(image);	
	const std::string image_path("/root/test_data/figure_test.png");
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