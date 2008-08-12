#pragma once
#include "Graph.h"
#include <queue>
#include "FifoQueue.h"


class PushRelabel
{
public:
	PushRelabel(void);
	~PushRelabel(void);

	static int calc(Graph* g, Node* preflowNodes);
	static int recalc(Graph* gr, Node* preflowNodes, int from, int to, int by);

	static int numOfPushes;
	static int numOfRelables;

private:
	static int updateLabels(int source);
	static Graph *g;
	static Node* nodeArr;

	static int preflow();
	static int flow();
	static int discharge(Node* node); //for the preflow
	static int discharge_back(Node* node); //for the flow
	static int findClosestPushBack(Node* node);
	static bool isAdmissible(Node* start, EdgeEntry* edge);
	static int push(int start, EdgeEntry* edge, int value);
	static int relabel(Node* node);
};
