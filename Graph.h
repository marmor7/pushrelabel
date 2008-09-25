#pragma once
#include "FifoQueue.h"
#include "PriorityQueue.h"
#include "HighLabelQueue.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

class Graph
{
public:
	Graph::Graph(string file, bool hlQueue);
	~Graph(void);

	int getSource() { return sourceID; }
	int getTarget() { return targetID; }
	int getNodesNum() { return nodesNum; }
	int getEdgesNum() { return edgesNum; }
	int getMaxDistance() { return maxDistance; }
	void setMaxDistance(int max) { maxDistance = max; }

	NodePool* getPool() { return pool; }

	int readGraph (string file);
	int printGraph(string file);
	int debugDump();
	Node* getNodeArray() { return nodeArray; }
	int dijkstra(int source_id, int* &dist, int* &prev, int &max);
	int* prevArray; //This will hold the previous node in the BFS path for each node
	int incPrevPathCap(int val);

private:
	int sourceID;
	int targetID;
	int nodesNum;
	int edgesNum;
	int maxDistance;
	NodePool* pool;
	Node* nodeArray;
};
