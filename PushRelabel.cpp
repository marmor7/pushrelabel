#include "PushRelabel.h"
#include <algorithm>

#define LEVEL_UP -1
#define NEW_NODE -2
#define QUEUE_NODE -3

//Static Members
Graph* PushRelabel::g;
Node* PushRelabel::nodeArr;

int PushRelabel::calc(Graph* gr)
{
	g = gr;
	nodeArr = g->getNodeArray();
	if (DEBUG >= LOG_3)
		g->printGraph();

	//Traverse all nodes (BFS) and initialize distance labels
	updateLabels(g->getTarget());

	//Set source's excess flow
	nodeArr[g->getSource()].setExcess(INFINITY);

	//Put the source in the pool
	g->getPool()->addNode(&nodeArr[g->getSource()]);

	//Calc pre-flow
	preflow();

	cout << "Max flow value is " << nodeArr[g->getTarget()].getExcess() << endl;

	//pre-flow to flow (remove excesses)
	//flow();

	return 0;
}

int PushRelabel::updateLabels(int source)
{
	queue<int> nodeQueue;
	int cur;

	int level = 0;

	//Init all to NEW_NODE
	for (int i = 1; i <= g->getNodesNum(); i++)
		nodeArr[i].setLabel(NEW_NODE);

	nodeQueue.push(source);
	nodeQueue.push(LEVEL_UP);
	nodeArr[source].setLabel(QUEUE_NODE);

	cout << "Init Search: " << endl;

	//Set distance to level
	while (!nodeQueue.empty())
	{
		cur = nodeQueue.front();
		nodeQueue.pop();
		
		if (cur == LEVEL_UP)
		{
			if (nodeQueue.empty())
				continue;
			level++;
			nodeQueue.push(LEVEL_UP);
		}
		else
		{
			if (DEBUG >= LOG_3)
				cout << cur << " (" << level << "), " ;

			//Update Label
			PushRelabel::nodeArr[cur].setLabel(level);

			//Enqueue all children
			EdgeEntry* edgePtr = PushRelabel::nodeArr[cur].getAdjList();
			//Skip dummy
			edgePtr = edgePtr->getNext();
			while (edgePtr != NULL)
			{
				if (nodeArr[edgePtr->getEndPoint()].getLabel() == NEW_NODE)
				{
					nodeQueue.push(edgePtr->getEndPoint());
					nodeArr[edgePtr->getEndPoint()].setLabel(QUEUE_NODE);
				}
				edgePtr = edgePtr->getNext();
			}
		}
	}

	cout << endl;
	return 0;
}

int PushRelabel::preflow()
{
	NodePool* pool = g->getPool();
	while (!pool->isEmpty())
	{
		discharge(pool->getNode());
	}

	return 0;
}

int PushRelabel::discharge(Node* node)
{
	bool search = false;

	//Nodes with no paths to target and the sink need not to be discharged
	if ((node->getID() == g->getTarget()) || (node->getLabel() == INFINITY))
		return 0;

	//Set the first edge to cur (skip the dummy)
	EdgeEntry* cur = node->getAdjList()->getNext();

	while ((cur != NULL) && (node->getExcess() > 0))
	{
		//check if the arc is admissible
		if (isAdmissible(node, cur))
		{
			// If the excess of the end node is 0, we add it to the pool
			// (nodes with excess 0 are not in the pool)
			if (nodeArr[cur->getEndPoint()].getExcess() == 0)
				g->getPool()->addNode(&nodeArr[cur->getEndPoint()]);

			push(node->getID(), cur, min(node->getExcess(), cur->getResCapacity()));

			// If we push to the target, we need to update the labels
			if (node->getID() == g->getTarget())
				search = true;
		}
		
		cur = cur->getNext();
	}
	
	if (search) 
		updateLabels(g->getSource());

	if (node->getExcess() > 0)
		relabel(node);

	return 0;
}

bool PushRelabel::isAdmissible(Node* start, EdgeEntry* edge)
{
	return ((!edge->isSaturated()) && 
		(start->getLabel() == nodeArr[edge->getEndPoint()].getLabel() + 1));
}

int PushRelabel::push(int start, EdgeEntry* edge, int value)
{
	if (DEBUG >= LOG_1)
		cout << "push from " << start << " to " 
		<< nodeArr[edge->getEndPoint()].getID() << 
		" (" << value << "), excess now: ";

	edge->push(value);
	nodeArr[edge->getEndPoint()].incExcess(value);
	nodeArr[start].decExcess(value);

	if (DEBUG >= LOG_1)
		cout << nodeArr[edge->getEndPoint()].getExcess() << endl;
	return 0;
}

int PushRelabel::relabel(Node* node)
{
	if (DEBUG >= LOG_1)
		cout << "relabel " << node->getID() << " from: " << node->getLabel();
	int min = INFINITY;
	EdgeEntry* cur = node->getAdjList()->getNext();

	while (cur != NULL)
	{
		if ((nodeArr[cur->getEndPoint()].getLabel() < min) &&
			(cur->getResCapacity() > 0))
			min = nodeArr[cur->getEndPoint()].getLabel();

		cur = cur->getNext();
	}
	
	if ((min == INFINITY) || (min >= 11))
		node->setLabel(INFINITY);
	else 
	{
		node->setLabel(min + 1);
		g->getPool()->addNode(node);
	}

	if (DEBUG >= LOG_1)
		cout << " to: " << (min == INFINITY ? INFINITY : min+1) << endl;

	return 0;
}