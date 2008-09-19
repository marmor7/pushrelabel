#include "EdgeEntry.h"

//EdgeEntry constructor
EdgeEntry::EdgeEntry(int end, int cap, int flw, EdgeEntry* prev)
{
	endPoint = end;
	capacity = cap;
	flow = flw;
	nextEdge = NULL;
	prevEdge = prev;
	reverseEdge = NULL;
}

//Push flow on the edge, thus decreasing the flow on the reversed edge
void EdgeEntry::push(int value)
{
	flow += value;
	reverseEdge->incFlow(-value);
}

//Set the reverse edge of a specific EdgeEntry to 'other'
void EdgeEntry::setReversed(EdgeEntry* other)
{
	reverseEdge = other;
}

//Check if a given edge is reveresed
bool EdgeEntry::isReverseEdge()
{
	int source, target;
	target = endPoint;
	source = reverseEdge->getEndPoint();
	return (source > target);
}