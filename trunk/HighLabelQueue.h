#pragma once
#include "NodePool.h"
#include <queue>


class HighLabelQueue : public NodePool
{

	//the doubly linked list sub-class
	class ListNode
	{
	public:
		Node* node;           //value stored in the node 
		ListNode *next;          //pointer to next node 
		ListNode *prev;          //pointer to previous node 
	};

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
		Node* removeFront();
		void insertBefore(Node* node,ListNode *nodeB);
	};

public:
	HighLabelQueue(int node_num);
	~HighLabelQueue(void);

	void findNewPtr();
	void addNode(Node* node);
	Node* getNode();

private:
	DList* listArray;
	int arrayPtr;

};
