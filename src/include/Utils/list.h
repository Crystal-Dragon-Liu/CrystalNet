#ifndef	LIST_H
#define	LIST_H
#include "include/Utils/common.h"
#include <stdlib.h>
#include "include/Utils/utils.h"
using namespace MyAllocFunc;

typedef void	(*callbackPrintNodes)(void*);
typedef void	(*callbackDeleteValues)(void*);
/* Node as a element of list we defined. */
struct Node{
	Node():value_(nullptr), next_(nullptr), prev_(nullptr){}
	void*	value_; /// the value of Node which has no specific type.
	struct  Node*	next_; /// ptr to next Node.
	struct	Node*	prev_; /// ptr to previous Node.
};

/* @brief	NodeList is a list that stores a bunch of nodes.*/
struct	NodeList{
	NodeList():size_(0), front_(nullptr), back_(nullptr){}
	int 		size_;
	Node*	front_;
	Node*	back_;
};




namespace NodeOP
{
	template<class NetAlloc=MyNetAlloc>
	NodeList* makeNodeList(){
		typedef NetSimpleAlloc<NodeList, NetAlloc> NodeListAllocator; // define a allocator.
		NodeList* newList = NodeListAllocator::allocate();
		newList->back_ 	= nullptr;
		newList->front_	= nullptr;
		newList->size_	=	0;
		return newList;
		}

	/*@brief	insert Node into NodeList obj.*/
	template<class NetAlloc=MyNetAlloc>
	void insertNode(NodeList* nodeList, 
					void *val)
	{
		typedef NetSimpleAlloc<Node, NetAlloc> NodeAllocator; // define a allocator.
		Node* newNode 			=	NodeAllocator::allocate();
		newNode->value_			=	val;
		newNode->next_ 			=	nullptr;
		if(!nodeList->back_){
			nodeList->front_ 	= 	newNode;
			newNode->prev_		=	nullptr;
		}
		else{
			nodeList->back_->next_ = newNode;
			newNode->prev_ 		=	nodeList->back_;
		}
		// update the back_
		nodeList->back_			=	newNode;
		++nodeList->size_;
	}

	/* @brief	delete all the node with the ptr to value_ */
	template<class NetAlloc=MyNetAlloc>
	void freeNode(Node* node, callbackDeleteValues deleteCallback = UtilFunc::freeKyp){
		typedef NetSimpleAlloc<Node, NetAlloc> NodeAllocator; /// define a allocator.
		Node* next = nullptr;
		while(node){
			next = node->next_;
			(*deleteCallback)(node->value_);
			NodeAllocator::deallocate(node);
			node = next;
		}
	}

	/* @brief	delete NodeList. a deleter is needed.*/
	template<class NetAlloc=MyNetAlloc>
	void freeNodeList(NodeList* nodeList){
		typedef NetSimpleAlloc<NodeList, NetAlloc> NodeListAllocator; /// define a allocator.
		freeNode(nodeList->front_); /// delete all the nodes with value_.
		NodeListAllocator::deallocate(nodeList); /// delete nodeList obj.

	}

		// TODO pop operation
	/* @brief	pop the last node from NodeList obj. */
	template<class NetAlloc=MyNetAlloc>
	void* popNodeList(NodeList* nodeList){
		typedef NetSimpleAlloc<Node, NetAlloc> NodeAllocator;
		if(!nodeList->back_) return nullptr;
		Node* b			=	nodeList->back_;
		void*	val		= 	b->value_;
		nodeList->back_	=	b->prev_;
		if(nodeList->back_)
			nodeList->back_->next_ = nullptr;
		b->prev_		=	nullptr;
		NodeAllocator::deallocate(b);
		nodeList->size_--;
		return val;
	}

	/* @brief	print all the nodes which belong to nodeList expected.*/
	extern void printAllNodes(NodeList* nodeList, callbackPrintNodes callback);

	/*
		@brief	generate a 2d array from NodeList,
				each value_ of Node obj in NodeList is similar with std::string.*/
	extern void** generateArrayfromList(NodeList* l);
}

	



#endif
