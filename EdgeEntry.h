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

	inline int getEndPoint() { return endPoint; }
	inline int getCapacity() { return capacity; }
	inline void incCapacity(int value) { capacity += value; }
	//inline void decCapacity(int value) { capacity -= value; }
	inline void incFlow(signed int value) { flow += value; }
	inline int getFlow() { return flow; }
	inline bool isSaturated() { return (flow == capacity); }
	inline int getResCapacity() { return (capacity - flow); }	

	inline EdgeEntry* getNext() { return nextEdge; }
	inline EdgeEntry* getPrev() { return prevEdge; }
	inline EdgeEntry* getReverse() { return reverseEdge; }
	void setReversed(EdgeEntry* other);
	bool isReverseEdge();

	EdgeEntry(int end, int cap, int flow, EdgeEntry* prev);
	EdgeEntry *nextEdge;
    EdgeEntry *prevEdge;

private:
	int endPoint;
    int capacity; //label
    signed int flow;     //label2
    EdgeEntry *reverseEdge; //the reversed edge
};