#include "EdgeEntry.h"

EdgeEntry::EdgeEntry(int end, int cap, int flw, EdgeEntry* prev)
{
	endPoint = end;
	capacity = cap;
	flow = flw;
	nextEdge = NULL;
	prevEdge = prev;
	reverseEdge = NULL;
}

void EdgeEntry::push(int value)
{
	flow += value;
	reverseEdge->incFlow(-value);
}

void EdgeEntry::setReversed(EdgeEntry* other)
{
	reverseEdge = other;
}

