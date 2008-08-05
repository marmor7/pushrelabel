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
	excess = 0;
	adjList = new EdgeEntry(0, 0, 0, NULL);
	lastEdge = adjList;
}

void Node::decExcess(int value)
{
	if (excess < value)
		cout << "BUG";
	else
		excess -= value; 
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

void Node::debugNodeDump()
{
	cout << getID() << ": ";
	cout << getDegree() << ", \t";
	cout << Utils::printValue(getLabel()) << ", \t";
	cout << getExcess();
	cout << endl;
}