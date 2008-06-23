#include "FifoQueue.h"

FifoQueue::FifoQueue(void)
{

}

FifoQueue::~FifoQueue(void)
{
}

void FifoQueue::addNode(Node* node)
{
	fifo.push(node);
	count++;
}

Node* FifoQueue::getNode()
{
	Node* n = fifo.front();
	fifo.pop();
	count--;
	return n;
}
