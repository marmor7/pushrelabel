#pragma once
#include "Node.h"

class NodePool
{
public:
	NodePool() { count = 0; }
	virtual void addNode(Node* node) = 0;
	virtual Node* getNode() = 0;
	int getCount() { return count; }
	void incCount() { count++ ; }
	int isEmpty() { return (count == 0); }
protected:
	int count;
};
