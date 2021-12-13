#include "include/Utils/utils.h"
#include <iostream>
#include "include/Utils/common.h"
#include "include/Utils/config_list.h"
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

    void printConfigSection(void* data){
        ConfigSection* section = reinterpret_cast<ConfigSection*>(data);
        //TODO display type.
        if(section->type == nullptr){__LOG_MESSAGE__("no type detected.");}
        else{__LOG_MESSAGE_X__(section->type);}
        //TODO display item.
        if(section->config == nullptr){
            std::cout << "no data in section." << std::endl;
            return;
        }
        else{
            Node* n = section->config->front_;
            while(n){
                void* kvp = n->value_;
                printkyp(kvp);
                Node* next = n->next_;
                n = next;
            }
        }
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

    void freeConfigSection(void* data){
        //TODO free the type of item.
        ConfigSection* section = reinterpret_cast<ConfigSection*>(data);
        if(section->type != nullptr)
            free(section->type);
        if(section->config == nullptr){
            std::cout << "no data in section." << std::endl;
        }
        else{
            Node* n = section->config->front_;
            while(n){
                void* kvp = n->value_;
                freeKyp(kvp);
                Node* next = n->next_;
                free(n);
                n = next;
            }
        }
        free(section->config);
        free(section);
    }
    void freeNothing(void* data){return;}


}