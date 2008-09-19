#pragma once
#include "NodePool.h"
#include <queue>

//A Node FIFO, based on the std queue.
//Inherits from the NodePool class
class FifoQueue : public NodePool
{
public:
	FifoQueue(void);
	~FifoQueue(void);

	void addNode(Node* node);
	Node* getNode();

private:
	queue<Node*> fifo;

};
