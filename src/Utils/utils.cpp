#include "include/Utils/utils.h"
#include <iostream>
#include "include/Utils/common.h"
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

    void copyCharArray(char* des, const std::string& line){
        size_t i = 0;
        for(i = 0; i < line.size();i++){
            des[i] = line[i];
        }
        des[i] = '\0';
    }
    
    void printCharArray(char* data){
        if(!data) return;
        int index = 0;
        std::cout << "[";
        while(data[index] != '\0'){
            std::cout <<  data[index] << " ";
            index++;
        }
        std::cout << "]";
    }

    void printkyp(void* data){
        KeyValuePair* pKeyValue = reinterpret_cast<KeyValuePair*>(data);
        printCharArray(pKeyValue->key_);
        std::cout << ": ";
        printCharArray(pKeyValue->value_);
        std::cout << std::endl;
    }

    void freeKyp(void* data){
        KeyValuePair* pKeyValue = reinterpret_cast<KeyValuePair*>(data);
        if(pKeyValue)
		{
				free(pKeyValue->key_);
				free(pKeyValue->value_);
                free(data);
		}
        else{__DETECT_BAD_ALLOC;}
    }

}