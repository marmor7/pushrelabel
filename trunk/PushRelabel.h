#pragma once
#include "Graph.h"
#include <queue>
#include "FifoQueue.h"

//The PushRelabel class is the class where most of the graph algorithms are implements
//it is responsible to calc the max flow using the push relable algorithm
class PushRelabel
{
public:
	PushRelabel(void);
	~PushRelabel(void);

	static int calc(Graph* g, bool stats);
	static EdgeEntry* findLowestLabelEdge(Node* node);
	static int dijkstraPath();

	static unsigned long numOfPushes;
	static unsigned long numOfRelabels;
	static int dist;


private:
	static int updateLabels(bool fromTarget, bool calcPrev);
	static Graph *g;
	static Node* nodeArr;

	static int preflow();
	static int flow();
	static int discharge(Node* node); //for the preflow
	static int discharge_back(Node* node); //for the flow
	static int findClosestPushBack(Node* node);
	static bool isAdmissible(Node* start, EdgeEntry* edge);
	static int push(int start, EdgeEntry* edge, int value);

};
