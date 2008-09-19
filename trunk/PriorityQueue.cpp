#include "PriorityQueue.h"

//PriorityQueue constructor
PriorityQueue::PriorityQueue(void)
{

}

//PriorityQueue destructor
PriorityQueue::~PriorityQueue(void)
{

}

//Add a node to the queue
void PriorityQueue::addNode(Node* node)
{
	nodeQueue.push(node);
	count++;
}

//Get the node with the highest label
Node* PriorityQueue::getNode()
{
	Node* n = nodeQueue.top();
	nodeQueue.pop();
	count--;
	return n;
}
