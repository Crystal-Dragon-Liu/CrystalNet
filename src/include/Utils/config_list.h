#ifndef CONFIG_LIST_H
#define CONFIG_LIST_H
#include "include/Utils/list.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "include/Utils/utils.h"
/* no idea if it should be removed. */
struct KeyValuePair
{
    char* key_;
    char* value_;
    int used_;
};

namespace ConfigIO{


    




    template<class NetAlloc=MyNetAlloc>
    void  configInsert(NodeList* l, char* key, char* value){
        typedef NetSimpleAlloc<KeyValuePair, NetAlloc> KvpAllocator;
         // kvp也是一个结构体，包含两个C风格字符数组指针：key和val，对应键值和值，
    // 此处key为变量名，val为变量的值（比如类别数，路径名称，注意都是字符类型数据）
    // kvp *p = malloc(sizeof(kvp));
    // p->key = key;
    // p->val = val;
    // p->used = 0;
    // (l, p);
        KeyValuePair* pKeyValue =   KvpAllocator::allocate();
        pKeyValue->key_     =   nullptr;
        pKeyValue->value_   =   nullptr;
        pKeyValue->used_    =   0;
        pKeyValue->key_     =   key;
        pKeyValue->value_   =   value;
        std::cout << pKeyValue->used_ << std::endl;
        std::cout << "key: " << pKeyValue->key_[0];
        int i = 0;
        if(!key) std::cout << "zero" << std::endl;
        while(key[i] != '\0'){
            std::cout <<  key[i] << " ";
            i++;
        }
        std::cout << std::endl;
        // NodeOP::insertNode(l, pKeyValue);
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
        // const char* keyStr = line.substr(0, i).data();
        // strcpy(key, keyStr);
        // const char* val_str = line.substr(i+1, len-i-1).data();
        // strcpy(val, val_str);
        UtilFunc::copyCharArray(key, line.substr(0, i));
        int index = 0;
        while(key[index] != '\0'){
            std::cout <<  key[index] << " ";
            index++;
        }
        std::cout << "key loaded" << std::endl;
        // configInsert(configList, key, val);
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
            len = line.size();
            std::cout << "<" << line  << ">" << std::endl;
            std::cout << "row_line_num : " << len << std::endl;
            std::cout << "[";
            for(int i = 0; i < len; i++){
                std::cout << line[i] << ",";
                if(line[i] == '\n' || line[i] == '\0')
                std::cout << "<unexpected char>"; 
            }
            std::cout << "]";
            std::cout << std::endl;
            row_num++;
            // TODO parse each item.
            readConfig<NetAlloc>(line, list);
        }
        dataFile.close();
        return list;
    }
}



#endif