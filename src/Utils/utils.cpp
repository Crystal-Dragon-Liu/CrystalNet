#include "include/Utils/utils.h"
#include <iostream>
namespace UtilFunc
{
    void file_error(char* filename){
        std::cout << std::string(filename) << std::endl;
        exit(0);
    }

    /*
        @brief screen out the '\t', ' ', '\n' from a string obj.
    */
    void stripUselessChr(std::string& str){
        //TODO screen out '\t', ' ', '\n'.
        size_t i;
        size_t len      = str.size();
        size_t offset   = 0;
        for(i = 0; i < len; i++){
            char c      =   str[i];
            if(c == ' ' || c == '\t' || c == '\n') ++ offset;
            else str[i - offset] = c;
        }
        str = str.substr(0, len-offset);
    }
    
}