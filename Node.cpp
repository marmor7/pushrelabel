#include "Node.h"


void Node::addEdge(EdgeEntry* edge) 
{
	assert(lastEdge); //lastEdge is never null (initialized with dummy)
	lastEdge->nextEdge = edge;
	edge->prevEdge = lastEdge;
	lastEdge = edge;
}

Node::Node()
{
	degree = 0;
    label = 0;
	id = 0;
    x = 0; //TODO: WHY?
    y = 0; //TODO: WHY?
	excess = 0;
	adjList = new EdgeEntry(-1, -1, NULL);
	lastEdge = adjList;
}

void Node::printNode()
{
	EdgeEntry* listPtr = adjList->getNext();
	while (listPtr != NULL)
	{
		cout << listPtr->getEndPoint() << " (" << listPtr->getCapacity() << ")" << ", ";
		listPtr = listPtr->getNext();
	}
	cout << endl;
}