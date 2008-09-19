#pragma once
#include "Node.h"

//This is an abstract class from which all the different pool will inherit
class NodePool
{
public:
	virtual void addNode(Node* node) = 0;
	virtual Node* getNode() = 0;
	int getCount() { return count; }
	void incCount() { count++ ; }
	int isEmpty() { return (count == 0); }
protected:
	int count;
};
