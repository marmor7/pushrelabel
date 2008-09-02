#pragma once
#include "Graph.h"
#include <queue>
#include "FifoQueue.h"


class PushRelabel_2
{
public:
	PushRelabel_2(void);
	~PushRelabel_2(void);

	static int calc(Graph* g);
	static int recalc(Graph* gr, Node* preflowNodes, int from, int to, int by);
	static EdgeEntry* findLowestLabelEdge(Node* node);

	static unsigned long numOfPushes;
	static unsigned long numOfRelables;

	bool isActive(Node* node);


private:
	static int updateLabels(bool fromTarget);
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
