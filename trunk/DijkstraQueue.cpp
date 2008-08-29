#include "DijkstraQueue.h"

DijkstraQueue::DijkstraQueue(int node_num)
{
	listArray = new DList[node_num - 1];
	arrayPtr = node_num;
}

DijkstraQueue::~DijkstraQueue(void)
{
	free(listArray);
}

void DijkstraQueue::addNode(Node* node, int distance)
{
	if (arrayPtr > distance)
		arrayPtr = distance;
	listArray[distance].insertFront(node);
	count++;
}

Node* DijkstraQueue::getNode()
{
	count--;
	Node* n = listArray[arrayPtr].removeFront();
	if (listArray[arrayPtr].front == NULL)
		findNewPtr();
	return n;
}

Node* DijkstraQueue::getNode(int ptr, int id)
{
	Node* n;
	while (1)
	{
		n = listArray[ptr].removeFront();
		if (n->getID() == id)
		{
			if (listArray[ptr].front == NULL)
				findNewPtr();
			count--;
			break;
		}
		listArray[ptr].insertBack(n);
	}
	return n;
}

void DijkstraQueue::findNewPtr()
{
	bool flag = false;
	int i = 0;

	while (!flag)
	{
		if (listArray[i].front != NULL)
		{
			arrayPtr = i;
			flag = true;
		}
		else
			i++;
	}
}
