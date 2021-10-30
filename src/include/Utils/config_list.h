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


    
    // extern int readConfig(const std::string& line, NodeList* configList);
    template<class NetAlloc=MyNetAlloc>
    int readConfig(const std::string& line, NodeList* configList){
        // typedef NetSimpleAlloc<char, NetAlloc> NodeAllocator;
        size_t i;
        size_t len  =   line.size();
        for(i = 0; i < len; ++i){if(line[i] == '='){break;}}
        if(i == len-1) return 0;
        std::cout << "key'length -> " << len - i + 1 << std::endl;
        std::cout << "value'length -> " << i + 1 << std::endl;
        // char* val   =   NetSimpleAlloc::allocate(len - i + 1);
        // char* key   =   NetSimpleAlloc::allocate(i + 1);
        // strcpy(key, line.substr(0, i+1).data());
        // strcpy(val, line.substr(i, len-i+1).data());
        return 0;
    }



    // template<class _Tp>
    // void            configInsert(NodeList<_Tp>* l, char* key, char* value);

    // template<class _Tp>
    // char*           configFind(NodeList<_Tp>* l, char* key);


    /*
        @brief read the .data and .cfg file with configuration of dataset.
        @param filename: the path of configuration file.
    */
    template<class NetAlloc=MyNetAlloc>
    NodeList* readDataAndCfg(const std::string& filename){
        std::ifstream dataFile(filename);
        std::string line;
        int row_num = 1;
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
            readConfig<NetAlloc>(line, nullptr);
        }
        dataFile.close();
        return nullptr;
    }
}



#endif