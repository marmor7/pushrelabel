#pragma once
#include "NodePool.h"
#include <queue>

//A class implemeneting the priority queue used for the High Label algorithm
//This classes used the STD priority queue
class PriorityQueue : public NodePool
{
public:
	PriorityQueue(void);
	~PriorityQueue(void);

	void addNode(Node* node);
	Node* getNode();

private:
	priority_queue<Node*,vector<Node*>, greater<Node*>> nodeQueue;

};
