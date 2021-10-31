#include "gtest/gtest.h"
#include <vector>
#include <algorithm>
#include "include/Utils/list.h"
#include "include/Utils/config_list.h"
TEST(BasicListTest, Test1){

	const std::string filename("/root/test_data/coco.data");
	NodeList* list = ConfigIO::readDataAndCfg(filename);
	if(!list) std::cout << "failed to load configuration." << std::endl;
	callbackPrintNodes pCallback = UtilFunc::printkyp;
	NodeOP::printAllNodes(list, pCallback);
	NodeOP::freeNodeList(list);
	std::cout << "<" << filename << "> loaded." << std::endl;
}
