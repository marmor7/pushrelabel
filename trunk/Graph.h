#pragma once
#include "FifoQueue.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

const int LOG_NONE = 0;
const int LOG_1 = 1; //important
const int LOG_2 = 2;
const int LOG_3 = 3; //not important
const int DEBUG = LOG_2;
const int INFINITY = INT_MAX;

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
	int printGraph ();
	Node* getNodeArray() { return nodeArray; }

private:
	int sourceID;
	int targetID;
	int nodesNum;
	int edgesNum;
	NodePool* pool;
	Node* nodeArray;
};
