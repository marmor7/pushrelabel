#pragma once
#include "NodePool.h"
#include <queue>


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
