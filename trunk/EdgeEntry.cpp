#include "EdgeEntry.h"

EdgeEntry::EdgeEntry(int end, int cap, EdgeEntry* prev)
{
	endPoint = end;
	capacity = cap;
	flow = 0;
	nextEdge = NULL;
	prevEdge = prev;
	otherEdge = NULL; //TODO: create the opposite edge
}
