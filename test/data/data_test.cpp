#include "gtest/gtest.h"
#include <vector>
#include <algorithm>
#include "include/Utils/list.h"
#include "include/Utils/config_list.h"



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
	// std::cout << "<" << filename << "> loaded." << std::endl;
}
