#pragma once
#include "HighLabelQueue.h"
#include <queue>


class DijkstraQueue : public HighLabelQueue
{

public:
	DijkstraQueue(int node_num);
	~DijkstraQueue(void);

	void findNewPtr();
	void addNode(Node* node, int distance);
	Node* getNode();
	Node* getNode(int ptr, int id);

private:
	int arrayPtr;

};
