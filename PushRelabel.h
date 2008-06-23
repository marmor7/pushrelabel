#pragma once
#include "Graph.h"
#include <queue>
#include "FifoQueue.h"


class PushRelabel
{
public:
	PushRelabel(void);
	~PushRelabel(void);

	static int calc(Graph* g);

private:
	static int updateLabels(int source);
	static Graph *g;
	static Node* nodeArr;

	static int preflow();
	static int discharge(Node* node);
	static bool isAdmissible(Node* start, EdgeEntry* edge);
	static int push(EdgeEntry* edge, int value);
	static int relabel(Node* node);
};
