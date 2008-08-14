#include "PriorityQueue.h"

PriorityQueue::PriorityQueue(void)
{

}

PriorityQueue::~PriorityQueue(void)
{
}

void PriorityQueue::addNode(Node* node)
{
	nodeQueue.push(node);
	count++;
}

Node* PriorityQueue::getNode()
{
	Node* n = nodeQueue.top();
	nodeQueue.pop();
	count--;
	return n;
}
