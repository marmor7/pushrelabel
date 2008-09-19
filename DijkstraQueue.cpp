#include "DijkstraQueue.h"

//Create the Dijkstra Queue
DijkstraQueue::DijkstraQueue(int node_num)
{
	listArray = new DList[node_num - 1];
	arrayPtr = node_num;
}

//Destroy the Dijkstra Queue
DijkstraQueue::~DijkstraQueue(void)
{
	free(listArray);
}

//Add a node with a given distance
void DijkstraQueue::addNode(Node* node, int distance)
{
	if (arrayPtr > distance)
		arrayPtr = distance;
	listArray[distance].insertFront(node);
	count++;
}

//Get the node with the lowest distance label;
Node* DijkstraQueue::getNode()
{
	count--;
	Node* n = listArray[arrayPtr].removeFront();
	//If there are no more nodes in the list find the new lowest pointer
	if (listArray[arrayPtr].front == NULL)
		findNewPtr();
	return n;
}

//Get a node from ptr index with a specific ID
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

//Find the new array pointer, meaning the lowest index with a non empty list
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
