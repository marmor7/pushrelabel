#pragma once
#include "FifoQueue.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

class Graph
{
public:
	Graph(string file);
	~Graph(void);

	int getSource() { return sourceID; }
	int getTarget() { return targetID; }
	int getNodesNum() { return nodesNum; }
	int getEdgesNum() { return edgesNum; }
	NodePool* getPool() { return pool; }

	int readGraph (string file);
	int printGraph();
	int debugDump();
	Node* getNodeArray() { return nodeArray; }

	int incEdgeCapacity(int from, int to, int value);

private:
	int sourceID;
	int targetID;
	int nodesNum;
	int edgesNum;
	NodePool* pool;
	Node* nodeArray;
};
