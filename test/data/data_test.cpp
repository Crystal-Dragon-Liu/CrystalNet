#include "gtest/gtest.h"
#include <vector>
#include <algorithm>
#include "include/Utils/list.h"
#include "include/Utils/config_list.h"
// using namespace NodeSpace;

TEST(BasicListTest, Test1){
	const std::string filename("/root/test_data/coco.data");
	ConfigIO::readDataAndCfg<int>(filename);
	std::cout << "<" << filename << "> loaded." << std::endl;
}
