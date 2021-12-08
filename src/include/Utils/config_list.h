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

    /*
        @brief  insert a pair into configuration sturct which
                is represented by a pointer to NodeList.
    */
    template<class NetAlloc=MyAllocFunc::MyNetAlloc>
    void  configInsert(NodeList* l, char* key, char* value){
        typedef NetSimpleAlloc<KeyValuePair, NetAlloc> KvpAllocator;
        KeyValuePair* pKeyValue =   KvpAllocator::allocate();
        pKeyValue->key_     =   key;
        pKeyValue->value_   =   value;
        pKeyValue->used_    =   0;
        NodeOP::insertNode(l, pKeyValue);
    }


    /*
        @brief  read configuration of one line, and insert the information
                detected into the configuration struct represented by a
                pointer to NodeList.
    */
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
            if(readConfig<NetAlloc>(line, list) == 0) return nullptr;
        }
        std::cout << "done" << std::endl;
        dataFile.close();
        return list;
    }

    //TODO find item of configuration from nodelist.
    /*
        @brief  find value according to the key expected from NodeList.
    */
    extern char*  configFind(NodeList* l, char* key);

    /* @brief   load the network struct */
    template<class NetAlloc=MyNetAlloc>
    extern NodeList* readModelConfig(const char* fileName){
        std::ifstream dataFile(fileName);
        std::string line;
        int row_num = 0;
        NodeList* sections = NodeOP::makeNodeList<NetAlloc>();
        //TODO continue to read .cfg files.
        return nullptr;
    }

}



#endif