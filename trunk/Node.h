#pragma once
#include "EdgeEntry.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <math.h>

using namespace std;


class Node
{
public:

	Node();

	int getDegree() { return degree; }
	void setDegree(int deg) { degree = deg; }
	int getLabel() { return label; }
	void setLabel(int lab) { label = lab; }
	int getExcess() { return excess; }
	void setExcess(int exc) { excess = exc; }
	void incExcess(int value) { excess += value; }
	void decExcess(int value);
	int getID() {return id; }
	void setID(int node_id) { id = node_id; }
	EdgeEntry* getAdjList() { return adjList; }
	EdgeEntry* getLastEdge() { return lastEdge; }

	void addEdge(EdgeEntry* edge);
	void printNode();
	void debugNodeDump();

private :
	int degree;
    unsigned int label;
	int excess;
	int id;
    EdgeEntry *adjList;
	EdgeEntry *lastEdge;
};
