#include "Node.h"


//Add an edge to the edge list of the node
void Node::addEdge(EdgeEntry* edge) 
{
	assert(lastEdge); //lastEdge is never null (initialized with dummy)
	lastEdge->nextEdge = edge;
	edge->prevEdge = lastEdge;
	lastEdge = edge;
	numEdges++;
}

//Node constructor
Node::Node()
{
	degree = 0;
    label = 0;
	id = 0;
	excess = 0;
	adjList = new EdgeEntry(0, 0, 0, NULL);
	lastEdge = adjList;
	numEdges = 0;
}

//Decrement the excess of the node
void Node::decExcess(int value)
{
	if (excess < value)
		cout << "Error found in excess";
	else
		excess -= value; 
}

//Print a node
void Node::printNode(ofstream & output)
{
	EdgeEntry* listPtr = adjList->getNext();
	while (listPtr != NULL)
	{
		output << this->getID() << "->" << listPtr->getEndPoint() << " (" << listPtr->getFlow() << ")" << endl;
		listPtr = listPtr->getNext();
	}
}

//Print the node debug dump info
void Node::debugNodeDump()
{
	cout << getID() << ": ";
	cout << getDegree() << ", \t";
	cout << Utils::printValue(getLabel()) << ", \t";
	cout << getExcess();
	cout << endl;
}

//Overwrite the node "greater then" and "less then" operators
bool Node::operator>(const Node* node) const
{
	return label > node->label;   
}

bool Node::operator<(const Node* node) const
{
	return label < node->label;   
}

