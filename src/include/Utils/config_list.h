#ifndef CONFIG_LIST_H
#define CONFIG_LIST_H
#include "include/Utils/list.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "include/Utils/utils.h"
#include "include/Utils/common.h"
using namespace MyAllocFunc;
/* no idea if it should be removed. */


namespace ConfigIO{
    template<class NetAlloc=MyAllocFunc::MyNetAlloc>
    void  configInsert(NodeList* l, char* key, char* value){
        typedef NetSimpleAlloc<KeyValuePair, NetAlloc> KvpAllocator;
         // kvp也是一个结构体，包含两个C风格字符数组指针：key和val，对应键值和值，
    // 此处key为变量名，val为变量的值（比如类别数，路径名称，注意都是字符类型数据）
    // kvp *p = malloc(sizeof(kvp));
    // p->key = key;
    // p->val = val;
    // p->used = 0;
    // (l, p);
        std::cout << "begining load key&value." << std::endl;
        KeyValuePair* pKeyValue =   KvpAllocator::allocate();
        pKeyValue->key_     =   nullptr;
        pKeyValue->value_   =   nullptr;
        pKeyValue->key_     =   key;
        pKeyValue->value_   =   value;
        pKeyValue->used_    =   0;
        // UtilFunc::printCharArray(pKeyValue->key_);
        // UtilFunc::printCharArray(pKeyValue->value_);
        NodeOP::insertNode(l, pKeyValue);
    }

    // template<class _Tp>
    // char*           configFind(NodeList<_Tp>* l, char* key);


    // extern int readConfig(const std::string& line, NodeList* configList);
    template<class NetAlloc=MyNetAlloc>
    int readConfig(const std::string& line, NodeList* configList){
        if(!configList) return 0;
        typedef NetSimpleAlloc<char, NetAlloc> NodeAllocator;
        size_t i;
        size_t len  =   line.size();
        for(i = 0; i < len; ++i){if(line[i] == '='){break;}}
        if(i == len-1) return 0;
        char* val   =   NodeAllocator::allocate(len - i);
        char* key   =   NodeAllocator::allocate(i + 1);
        if(val == nullptr || key == nullptr) return 0;
        // TODO THERE ARE SOME BUGS
        UtilFunc::copyCharArray(key, line.substr(0, i));
        UtilFunc::copyCharArray(val, line.substr(i+1, len-i-1));
        configInsert(configList, key, val);
        // NodeAllocator::deallocate(val);
        // NodeAllocator::deallocate(key);
        return 1;
    }


    /*
        @brief read the .data and .cfg file with configuration of dataset.
        @param filename: the path of configuration file.
    */
    template<class NetAlloc=MyNetAlloc>
    NodeList* readDataAndCfg(const std::string& filename){
        std::ifstream dataFile(filename);
        std::string line;
        int row_num = 1;
        NodeList* list = NodeOP::makeNodeList();
        while(getline(dataFile, line)){
            int len = line.size();
            if(len == 0 || line[0] == '\0' || line[0] == '#' || line[0] == ';') continue;
            UtilFunc::stripUselessChr(line);
            row_num++;
            // TODO parse each item.
            if(readConfig<NetAlloc>(line, list) == 0) return nullptr;
        }
        std::cout << "done" << std::endl;
        dataFile.close();
        return list;
    }
}



#endif