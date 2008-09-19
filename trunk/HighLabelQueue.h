#pragma once
#include "NodePool.h"
#include <queue>

//The HighLabelQueue implements a priority queue as described in the article
//It holds an array of doubly linked lists, and an index to the array indicating
//the place of the list with the highest labels
class HighLabelQueue : public NodePool
{
protected:
	//
	//the list node sub-class
	//
	class ListNode
	{
	public:
		Node* node;           //value stored in the node 
		ListNode *next;          //pointer to next node 
		ListNode *prev;          //pointer to previous node 
	};

	//
	//the doubly linked list sub-class
	//
	class DList
	{
	public:
		ListNode *front;       //pointer to front of list   
		ListNode *back;        //pointer to back of list  

		DList()
		{
			front=NULL;
			back=NULL;
		}

		void insertFront(Node* node);             
		void insertBack(Node* node);             
		Node* removeFront();
		void insertBefore(Node* node,ListNode *nodeB);
	};
	//End of the doubly linked list sub-class
public:
	HighLabelQueue(int node_num);
	HighLabelQueue(void);
	~HighLabelQueue(void);

	void findNewPtr();
	void addNode(Node* node);
	Node* getNode();

protected:
	DList* listArray;
	int arrayPtr;

};
