#include "include/Utils/config_list.h"
#include "stdlib.h"
#include "stdio.h"
#include "include/Utils/utils.h"
#include <fstream>
#include <string>
#include <sstream>

namespace ConfigIO{

    char*  configFind(NodeList* l, char* key){
        Node* currentNode = l->front_;
        if(!currentNode) return nullptr;
        while(currentNode){
            // process the currentNode.
            KeyValuePair* pKeyValue = reinterpret_cast<KeyValuePair*>(currentNode->value_);
            if(strcmp(pKeyValue->key_,key) == 0){
                pKeyValue->used_ = 1;
                return pKeyValue->value_;
            }
            currentNode =   currentNode->next_;
        }
        return nullptr;
    }

}