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
	//Reset the distance labels - this time from the source
	updateLabels(g->getSource());

	//Enqueue all the nodes with Excess > 0
	assert(g->getPool()->isEmpty()); //At this point the queue should be empty
	for (int i=0 ; i< g->getNodesNum() ; i++) //No +1 here because we don't want the sink
		if (nodeArr[i].getExcess() > 0)
			g->getPool()->addNode(&nodeArr[i]);

	//pre-flow to flow (remove excesses)
	flow();

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

	cout << "Updating Labels" << endl;

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
	int push_value = 0;

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

			// Push on the edge, and push the opposite on the reverse edge
			push_value = min(node->getExcess(), cur->getResCapacity());
			push(node->getID(), cur, push_value);

			// If we push to the target, we need to update the labels
			if (node->getID() == g->getTarget())
				search = true;
		}

		cur = cur->getNext();
	}

	if (search) 
		updateLabels(g->getSource());

	if (node->getExcess() > 0 && node->getID() != g->getSource())
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
	{
		int tmp = nodeArr[edge->getEndPoint()].getExcess();
		if (tmp == INFINITY)
			cout << "INF" << endl;
		else
			cout << nodeArr[edge->getEndPoint()].getExcess() << endl;
	}
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

int PushRelabel::flow()
{
	NodePool* pool = g->getPool();
	while (!pool->isEmpty())
	{
		discharge_back(pool->getNode());
	}

	if (DEBUG >= LOG_1) //Print the excess of each node
	{
		for (int i=0 ; i<g->getNodesNum() ; i++)
			if (nodeArr[i+1].getExcess() > 0)
				cout << "Node :" << i+1 << " Excess :" << nodeArr[i+1].getExcess() << endl;
	}
	return 0;
}

int PushRelabel::discharge_back(Node *node)
{
	int extra, level, quantity;
	bool search = false;
	EdgeEntry *edge;
	Node *end_point;

	//Nodes with no paths to target and the sink need not to be discharged
	if ((node->getID() == g->getSource()) || (node->getLabel() == INFINITY))
		return 0;

	extra = node->getExcess();

	while (extra > 0) {
		level = findClosestPushBack(node);
		if (level == INFINITY) break;

		edge = node->getAdjList();
		while (edge != NULL && extra > 0) 
		{
			if ((edge->getFlow() < 0) && (nodeArr[edge->getEndPoint()].getLabel() == level)) 
			{
				quantity = -edge->getFlow();
				if (quantity > extra) 
					quantity = extra;
				edge->push(quantity);

				end_point = &nodeArr[edge->getEndPoint()];

				if (end_point->getExcess() == 0)
					g->getPool()->addNode(end_point);

				if (end_point->getID() == g->getSource()) 
					search = true;

				end_point->incExcess(quantity);
				extra -= quantity;
			}
			edge = edge->getNext();
		}
	}

	if (search) 
		updateLabels(g->getSource());
	else 
	{
		if (extra == 0) 
			level = findClosestPushBack(node);

		if (level >= g->getTarget()) 
			node->setLabel(INFINITY);
		else 
			node->setLabel(level+1);
	}

	node->setExcess(extra);
	return 0;
}

int PushRelabel::findClosestPushBack(Node* node)
{
	EdgeEntry* edge = node->getAdjList();
	Node* end_point;
    int min = INFINITY;

	while (edge!=NULL) {
		if (edge->getFlow() < 0) {
			end_point = &nodeArr[edge->getEndPoint()];
			if (end_point->getLabel() < min)
				min = end_point->getLabel();
		}
		edge = edge->getNext();
	}

	if (min < g->getTarget()) return(min);
	else return(INFINITY);

}
