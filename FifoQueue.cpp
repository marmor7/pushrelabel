#include "FifoQueue.h"

//FifoQueue constructor
FifoQueue::FifoQueue(void)
{
	count = 0;
}
//FifoQueue destructor
FifoQueue::~FifoQueue(void)
{
}

//Add a node to the FIFO
void FifoQueue::addNode(Node* node)
{
	fifo.push(node);
	count++;
}

//Get a node from the FIFO
Node* FifoQueue::getNode()
{
	Node* n = fifo.front();
	fifo.pop();
	count--;
	return n;
}
