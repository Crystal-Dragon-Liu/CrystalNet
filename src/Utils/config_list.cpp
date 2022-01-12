#include "include/Utils/config_list.h"
#include "stdlib.h"
#include "stdio.h"
#include "include/Utils/utils.h"
#include <fstream>
#include <string>
#include <sstream>

namespace ConfigIO{
    char*  configFind(NodeList* l, const char* key){
        Node* currentNode = l->front_;
        if(!currentNode) return nullptr;
        while(currentNode){
            // process the currentNode.
            KeyValuePair* pKeyValue = reinterpret_cast<KeyValuePair*>(currentNode->value_);
            if(strcmp(pKeyValue->key_,key) == 0){
                pKeyValue->used_ = 1;
                return pKeyValue->value_;
            }
            currentNode = currentNode->next_;
        }
        return nullptr;
    }

    int     configFindToInt(NodeList*l, const char* key, int def, bool quiet){
        return configFindToValue<int>(l, key, def, UtilFunc::charToInt, quiet);
    }

    char*   configFindToStr(NodeList*l, const char* key, char* def, bool quiet){
        return configFindToValue<char*>(l, key, def, UtilFunc::constCharToChar);
        // return nullptr;
    }

    float   configFindToFloat(NodeList*l, const char* key, float def, bool quiet){
        return configFindToValue<float>(l, key, def, UtilFunc::charToFloat, quiet);
    }

}