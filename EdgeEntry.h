#pragma once
#include <stdio.h>

class EdgeEntry
{
public:
	int getEndPoint() { return endPoint; }
	int getCapacity() { return capacity; }
	int getFlow() { return flow; }
	bool isSaturated() { return (flow == capacity); }
	int getResCapacity() { return (capacity - flow); }

	void push(int value) { flow += value; }

	EdgeEntry* getNext() { return nextEdge; }
	EdgeEntry* getPrev() { return prevEdge; }
	EdgeEntry* getOther() { return otherEdge; }

	EdgeEntry(int end, int cap, EdgeEntry* prev);

	EdgeEntry *nextEdge;
    EdgeEntry *prevEdge;
    EdgeEntry *otherEdge; //the reversed edge

private:
	int endPoint;
    int capacity; //label
    int flow;     //label2
	
};