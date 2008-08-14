#pragma once
#include "NodePool.h"
#include <queue>


class PriorityQueue : public NodePool
{
public:
	PriorityQueue(void);
	~PriorityQueue(void);

	void addNode(Node* node);
	Node* getNode();

private:
	priority_queue<Node*,vector<Node*>, less<Node*>> nodeQueue;

};
