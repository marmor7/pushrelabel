#pragma once
#include <stdio.h>

class EdgeEntry
{
public:
	void push(int value);

	int getEndPoint() { return endPoint; }
	int getCapacity() { return capacity; }
	void incFlow(int value) { flow += value; }
	int getFlow() { return flow; }
	bool isSaturated() { return (flow == capacity); }
	int getResCapacity() { return (capacity - flow); }	

	EdgeEntry* getNext() { return nextEdge; }
	EdgeEntry* getPrev() { return prevEdge; }
	EdgeEntry* getReverse() { return reverseEdge; }

	EdgeEntry(int end, int cap, EdgeEntry* prev);

	EdgeEntry *nextEdge;
    EdgeEntry *prevEdge;
    EdgeEntry *reverseEdge; //the reversed edge

private:
	int endPoint;
    int capacity; //label
    int flow;     //label2
};