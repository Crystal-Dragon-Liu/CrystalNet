#include "include/Utils/list.h"

namespace NodeOP{

void** generateArrayfromList(NodeList* l){
		void **a = (void**)calloc(l->size_, sizeof(void*));
    	int count = 0;
    	Node *n = l->front_;
    	while(n){
        	a[count++] = n->value_;
        	n = n->next_;
    	}
    	return a;
	}
	
void printAllNodes(NodeList* nodeList, callbackPrintNodes callback){
	Node* currentNode = nodeList->front_;
	while(currentNode){
		(*callback)(currentNode->value_);
		currentNode = currentNode->next_;
	}
}
}


