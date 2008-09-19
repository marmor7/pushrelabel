#include "HighLabelQueue.h"

//HighLabelQueue constructor
HighLabelQueue::HighLabelQueue(void)
{
	count = 0;
}

//HighLabelQueue constructor
HighLabelQueue::HighLabelQueue(int node_num)
{
	listArray = new DList[node_num];
	arrayPtr = 0;
}

//HighLabelQueue destructor
HighLabelQueue::~HighLabelQueue(void)
{
	free(listArray);
}

//Add a node to the queue
void HighLabelQueue::addNode(Node* node)
{
	if (node->getLabel() == INFINITY)
		return;
	if (arrayPtr < node->getLabel())
		arrayPtr = node->getLabel();
	listArray[node->getLabel()].insertFront(node);
	count++;
}

//Get a the node with highest label
Node* HighLabelQueue::getNode()
{
	count--;
	Node* n = listArray[arrayPtr].removeFront();
	if (listArray[arrayPtr].front == NULL)
		findNewPtr();
	return n;
}

//Find the new pointer to the index with the highest label node in the queue
void HighLabelQueue::findNewPtr()
{
	bool flag = false;
	int oldPtr = arrayPtr;
	int i;

	for (i = arrayPtr-1; i>0 ; i--)
	{
		if (listArray[i].front != NULL)
		{
			arrayPtr = i;
			break;
		}
	}

	if (arrayPtr == oldPtr)
		arrayPtr = 0;
}

//
//Doubly linked list implementation
//

//insert a node before the front node
void HighLabelQueue::DList::insertFront (Node* node)
{
	ListNode *newNode;
	if(this->front==NULL)
	{
		newNode=new ListNode();
		this->front=newNode;
		this->back =newNode;
		newNode->prev=NULL;
		newNode->next=NULL;
		newNode->node=node;

	}
	else
	{
		newNode=new ListNode();
		newNode->prev=NULL;
		newNode->next =this->front;
		newNode->node =node;
		this->front->prev = newNode;
		this->front = newNode;
	}
}

//insert a node after the back node 
void HighLabelQueue::DList::insertBack (Node* node)
{
	ListNode *newNode;
	if(this->front==NULL)
	{
		newNode=new ListNode();
		this->front=newNode;
		this->back =newNode;
		newNode->prev=NULL;
		newNode->next=NULL;
		newNode->node=node;

	}
	else
	{
		newNode=new ListNode();
		newNode->next=NULL;
		newNode->prev =this->back;
		newNode->node =node;
		this->back->next = newNode;
		this->back = newNode;
	}
}

//remove the front node 
Node* HighLabelQueue::DList::removeFront()
{
	Node* n = this->front->node;

	ListNode* listNode = this->front;
	this->front=this->front->next;
	if (this->front != NULL)
		this->front->prev = NULL;
	else
		this->back = NULL;

	free (listNode);
	return n;
}
