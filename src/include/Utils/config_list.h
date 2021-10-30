#ifndef CONFIG_LIST_H
#define CONFIG_LIST_H
#include "include/Utils/list.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "include/Utils/utils.h"
/* no idea if it should be removed. */
struct KeyValuePair
{
    char* key_;
    char* value_;
    int used_;
};

namespace ConfigIO{
    // template<class _Tp>
    // int             readConfig(char* s, NodeList<_Tp>* configList);

    // template<class _Tp>
    // void            configInsert(NodeList<_Tp>* l, char* key, char* value);

    // template<class _Tp>
    // char*           configFind(NodeList<_Tp>* l, char* key);


    /*
        @brief read the .data and .cfg file with configuration of dataset.
        @param filename: the path of configuration file.
    */
    template<class _Tp>
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
        }
        dataFile.close();
        return nullptr;
    }
}



#endif