#include "HighLabelQueue.h"

HighLabelQueue::HighLabelQueue(int node_num)
{
	listArray = new DList[node_num - 1];
	arrayPtr = 0;
}

HighLabelQueue::~HighLabelQueue(void)
{
	free(listArray);
}

void HighLabelQueue::addNode(Node* node)
{
	if (node->getLabel() == INFINITY)
		return;
	if (arrayPtr < node->getLabel())
		arrayPtr = node->getLabel();
	listArray[node->getLabel()].insertFront(node);
	count++;
}

Node* HighLabelQueue::getNode()
{
	count--;
	Node* n = listArray[arrayPtr].removeFront();
	if (listArray[arrayPtr].front == NULL)
		findNewPtr();
	return n;
}

void HighLabelQueue::findNewPtr()
{
	bool flag = false;
	int i = arrayPtr-1;

	while (!flag)
	{
		if (listArray[i].front != NULL)
		{
			arrayPtr = i;
			flag = true;
		}
		else
			i--;
	}
}

//insert a ListNode before the front ListNode 
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
		ListNode *newNode;
		newNode=new ListNode();
		newNode->prev=NULL;
		newNode->next =this->front;
		newNode->node =node;
		this->front->prev = newNode;
		this->front = newNode;
	}
}


//remove the front ListNode 
Node* HighLabelQueue::DList::removeFront()
{
	Node* n = this->front->node;

	ListNode* listNode = this->front;
	this->front=this->front->next;
	if (this->front != NULL)
		this->front->prev=NULL;

	free (listNode);
	return n;
}
