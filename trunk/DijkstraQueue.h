#pragma once
#include "HighLabelQueue.h"
#include <queue>

//The DijkstraQueue class inherits from the HighLabelQueue class.
//This is because the HighLabelQueue class can be used as the priority queue
//needed for Dijkstra's algorithm with a few minor changes
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
