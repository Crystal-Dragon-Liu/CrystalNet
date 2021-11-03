#include "gtest/gtest.h"
#include <vector>
#include <algorithm>
#include "include/Utils/list.h"
#include "include/Utils/config_list.h"
#include "include/data/matrix.h"

/*
	Test List:
	1. read the .data configuration file.
	2. parse the configuration and insert it into NodeList.
	3. find a value according to a key provided.
	4. free NodeList.
*/
TEST(BasicListTest, Test1){
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
TEST(BasicListTest, Test2)
{
	Matrix* matrix = new Matrix(5, 5);
	auto data = matrix->getData();
	// initialize data with 1.
	for(size_t i = 0; i < matrix->getRowCount();i++){
		for(size_t j = 0; j < matrix->getColCount(); j++){
			data[i][j] = 1;
		}
	}
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


