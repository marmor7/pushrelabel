#pragma once
#include "Utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

class EdgeEntry
{
public:
	void push(int value);

	int getEndPoint() { return endPoint; }
	int getCapacity() { return capacity; }
	void incFlow(signed int value) { flow += value; }
	int getFlow() { return flow; }
	bool isSaturated() { return (flow == capacity); }
	int getResCapacity() { return (capacity - flow); }	

	EdgeEntry* getNext() { return nextEdge; }
	EdgeEntry* getPrev() { return prevEdge; }
	EdgeEntry* getReverse() { return reverseEdge; }
	void setReversed(EdgeEntry* other);

	EdgeEntry(int end, int cap, int flow, EdgeEntry* prev);
	EdgeEntry *nextEdge;
    EdgeEntry *prevEdge;

private:
	int endPoint;
    int capacity; //label
    signed int flow;     //label2
    EdgeEntry *reverseEdge; //the reversed edge
};