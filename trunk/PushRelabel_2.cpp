#include "PushRelabel_2.h"
#include <time.h>
#include <algorithm>
#include <ctime>

#define LEVEL_UP -1
#define NEW_NODE -2
#define QUEUE_NODE -3

//Static Members
Graph* PushRelabel_2::g;
Node* PushRelabel_2::nodeArr;
unsigned long PushRelabel_2::numOfPushes;
unsigned long PushRelabel_2::numOfRelables;


int PushRelabel_2::calc(Graph* gr)
{
	g = gr;
	nodeArr = g->getNodeArray();

	if (DEBUG >= LOG_3)
		g->printGraph();

	clock_t start, finish;
	start = clock();

	//Traverse all nodes (BFS) and initialize distance labels
	updateLabels(true);

	//Set source's excess flow
	nodeArr[g->getSource()].setExcess(INFINITY);

	//Put the source in the pool
	g->getPool()->addNode(&nodeArr[g->getSource()]);

	if (DEBUG >= LOG_2)
		g->debugDump();

	PushRelabel_2::numOfPushes = 0;
	PushRelabel_2::numOfRelables = 0;

	//Calc pre-flow
	preflow();

//	if (DEBUG >= LOG_2)
//		g->debugDump();
//
//	if (DEBUG >= LOG_NONE){
//		cout << "# pushes: " << PushRelabel_2::numOfPushes << endl;
//		cout << "# relables: " << PushRelabel_2::numOfRelables << endl;
//	}
//
//	int maxFlow = nodeArr[g->getTarget()].getExcess();
//	cout << "Max flow value is " << maxFlow << endl;
//	//Reset the distance labels - this time from the source
//	updateLabels(false);
//
//	//Enqueue all the nodes with Excess > 0
//	assert(g->getPool()->isEmpty()); //At this point the queue should be empty
//	for (int i=0 ; i< g->getNodesNum() ; i++){ //No +1 here because we don't want the sink
//		if (nodeArr[i].getExcess() > 0){
//			g->getPool()->addNode(&nodeArr[i]);
//		}
//	}
//
//	//pre-flow to flow (remove excesses)
//	flow();
//
//	finish = clock();
//
//	// Try to find an edge for recalc
//	//updateLabels(true);
//	//for (int i=g->getNodesNum(); i > 0; i--){
//	//	for (int j=g->getNodesNum(); j > 0; j--){
//	//		if (!g->incEdgeCapacity(i, j, 10000)){
//	//			updateLabels(true);
//	//			if (nodeArr[g->getSource()].getLabel() < INFINITY)
//	//			{
//	//				cout << i << "->" << j << endl;
//	//				break;
//	//			}
//	//		}
//	//	}
//	//	if (nodeArr[g->getSource()].getLabel() < INFINITY)
//	//		break;
//	//}
//
//	
//	cout << "Total clocks: " << (finish - start) << endl;
//
//	return maxFlow;
	return 0;
}

//int PushRelabel_2::recalc(Graph* gr, Node* preflowNodes, int from, int to, int by)
//{
//	assert(gr != NULL);
//	assert(preflowNodes != NULL);
//
//	g = gr;
//	nodeArr = g->getNodeArray();
//	memcpy(nodeArr, preflowNodes, sizeof(Node)*g->getNodesNum());
//
//	//Traverse all nodes (BFS) and initialize distance labels
//	updateLabels(true);
//
//	PushRelabel_2::numOfPushes = 0;
//	PushRelabel_2::numOfRelables = 0;
//
//	//If sink is unreachable from the source (label = INF), then stop
//	if (nodeArr[g->getSource()].getLabel() == INFINITY)
//		return 0;
//
//	if (DEBUG >= LOG_3)
//		g->printGraph();
//
//	clock_t start, middle, finish;
//	start = clock();
//
//	//Put the source in the pool
//	//g->getPool()->addNode(&nodeArr[from]);
//	g->getPool()->addNode(&nodeArr[g->getSource()]);
//
//	nodeArr[g->getSource()].setExcess(by); //TMP
//	//nodeArr[from].setExcess(by); //TMP
//	//nodeArr[from].setLabel(nodeArr[from].getLabel()+1);
//
//	if (DEBUG >= LOG_2)
//		g->debugDump();
//
//	//Calc pre-flow
//	preflow();
//
//	if (DEBUG >= LOG_2)
//		g->debugDump();
//
//	if (DEBUG >= LOG_NONE){
//		cout << "# pushes: " << PushRelabel_2::numOfPushes << endl;
//		cout << "# relables: " << PushRelabel_2::numOfRelables << endl;
//	}
//
//	int maxFlow = nodeArr[g->getTarget()].getExcess();
//	cout << "Max flow value is " << maxFlow << endl;
//	//Reset the distance labels - this time from the source
//	updateLabels(false);
//
//	//Enqueue all the nodes with Excess > 0
//	assert(g->getPool()->isEmpty()); //At this point the queue should be empty
//	for (int i=0 ; i< g->getNodesNum() ; i++) //No +1 here because we don't want the sink
//		if (nodeArr[i].getExcess() > 0)
//			g->getPool()->addNode(&nodeArr[i]);
//
//	middle = clock();
//	cout << "Total ticks to preflow: " << middle - start << endl;
//
//	cout << "preflow done..." << " calc flow..." << endl;
//
//	//pre-flow to flow (remove excesses)
//	flow();
//
//	finish = clock();
//	cout << "Total ticks to flow: " << finish - start << endl;
//	cout << "Total ticks from preflow to flow: " << finish - middle << endl;
//
//	return maxFlow;
//}


int PushRelabel_2::updateLabels(bool fromTarget)
{
	int source;

	if (fromTarget)
		source = g->getTarget();
	else
		source = g->getSource();

	queue<int> nodeQueue;
	int cur;

	int level = 0;

	//Init all to NEW_NODE
	for (int i = 1; i <= g->getNodesNum(); i++)
		nodeArr[i].setLabel(NEW_NODE);

	nodeQueue.push(source);
	nodeQueue.push(LEVEL_UP);
	nodeArr[source].setLabel(QUEUE_NODE);

	if (DEBUG >= LOG_2)
		cout << "Updating Labels..." << endl;

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
			PushRelabel_2::nodeArr[cur].setLabel(level);
			if (cur == g->getSource())
				g->setMaxDistance(level);

			//Enqueue all children
			EdgeEntry* edgePtr = PushRelabel_2::nodeArr[cur].getAdjList();
			//Skip dummy
			edgePtr = edgePtr->getNext();
			while (edgePtr != NULL)
			{				
				if (nodeArr[edgePtr->getEndPoint()].getLabel() == NEW_NODE)
				{
					//TBD
//					if (((edgePtr->getFlow() != 0) && (fromTarget)))
//					|| ((edgePtr->getResCapacity() != 0) && (!fromTarget)))
						nodeQueue.push(edgePtr->getEndPoint());
					nodeArr[edgePtr->getEndPoint()].setLabel(QUEUE_NODE);
				}

				edgePtr = edgePtr->getNext();
			}
		}
	}

	if (DEBUG >= LOG_3)
	{
		for (int i = 1; i <= g->getNodesNum(); i++)
			cout << i << ": " << nodeArr[i].getLabel() << endl;
		cout << "max distance: " << g->getMaxDistance() << endl;
	}

	return 0;
}

int PushRelabel_2::preflow()
{
	//NodePool* pool = g->getPool();
	//while (!pool->isEmpty())
	//{
	//	discharge(pool->getNode());
	//}

	return 0;
}

int PushRelabel_2::discharge(Node* node)
{
//	bool search = false;
//	int push_value = 0;
//	int edges = 0;
//
//	//Nodes with no paths to target and the sink need not to be discharged
//	if ((node->getID() == g->getTarget()) || (node->getLabel() == INFINITY))
//		return 0;
//
//	//Set the first edge to cur (skip the dummy)
//	//EdgeEntry* cur = findLowestLabelEdge(node);
//	EdgeEntry* cur = node->getAdjList()->getNext();
//
//	while ((cur != NULL) && (node->getExcess() > 0))
//	{
//		edges++;
//		//check if the arc is admissible (not saturated and label is 1 + end node label)
//		if (isAdmissible(node, cur) && (cur->getEndPoint() != g->getSource()))
//		{
//			// If the excess of the end node is 0, we add it to the pool
//			// (nodes with excess 0 are not in the pool)
//			if (nodeArr[cur->getEndPoint()].getExcess() == 0)
//				g->getPool()->addNode(&nodeArr[cur->getEndPoint()]);
//
//			// Push on the edge, and push the opposite on the reverse edge
//			push_value = min(node->getExcess(), cur->getResCapacity());
//			push(node->getID(), cur, push_value);
//
//			// If we push to the target, we need to update the labels
//			if (node->getID() == g->getTarget())
//				search = true;
//		}
//
//		if (edges < node->getNumEdges())
//			//cur = findLowestLabelEdge(node);
//			cur = cur->getNext();
//		else
//			cur = NULL;
//	}
//
//	if (search) 
//		updateLabels(false);
//
//	if (node->getExcess() > 0)
//		relabel(node);
//
	return 0;
}

bool PushRelabel_2::isAdmissible(Node* start, EdgeEntry* edge)
{
	return ((!edge->isSaturated()) && //this means: a residual arc
		(start->getLabel() == nodeArr[edge->getEndPoint()].getLabel() + 1));
}

bool PushRelabel_2::isActive(Node* node)
{
	if ((node->getID() != g->getSource()) &&
		(node->getID() != g->getTarget()) &&
		(node->getLabel() < g->getNodesNum()))
		return true; 
	else
		return false;
}

//int PushRelabel_2::push(int start, EdgeEntry* edge, int value)
//{
//	PushRelabel_2::numOfPushes++;
//	if (DEBUG >= LOG_2)
//		cout << "push from " << start << " to " 
//		<< nodeArr[edge->getEndPoint()].getID() << 
//		" (" << value << "), excess was: " <<
//		Utils::printValue(nodeArr[edge->getEndPoint()].getExcess()) << ", excess now: ";
//
//	edge->push(value);
//	nodeArr[edge->getEndPoint()].incExcess(value);
//	nodeArr[start].decExcess(value);
//
//	if (DEBUG >= LOG_2)
//	{
//		cout << Utils::printValue(nodeArr[edge->getEndPoint()].getExcess()) << endl;
//	}
//	return 0;
//}

int PushRelabel_2::relabel(Node* node)
{
	//PushRelabel_2::numOfRelables++;
	//if (DEBUG >= LOG_2)
	//	cout << "relabel " << node->getID() << " from: " << node->getLabel();
	//int min = INFINITY;
	//EdgeEntry* cur = node->getAdjList()->getNext();
	////EdgeEntry* cur = findLowestLabelEdge(node);

	//while (cur != NULL)
	//{
	//	if ((nodeArr[cur->getEndPoint()].getLabel() < min) &&
	//		(cur->getResCapacity() > 0))
	//		min = nodeArr[cur->getEndPoint()].getLabel();

	//	cur = cur->getNext();
	//}
	////if (cur == NULL)
	////	min = INFINITY;
	////else
	////	min = nodeArr[cur->getEndPoint()].getLabel();

	//if ((min == INFINITY) || (min+1 >= g->getNodesNum()-1))
	//	node->setLabel(INFINITY);
	//else 
	//{
	//	node->setLabel(min + 1);
	//	g->getPool()->addNode(node);

	//}

	//if (DEBUG >= LOG_2)
	//	cout << " to: " << (min == INFINITY ? INFINITY : min+1) << endl;

	return 0;
}

//int PushRelabel_2::flow()
//{
//	if (DEBUG >= LOG_2) //Print the excess of each node
//	{
//		cout << "Excess before flow" << endl;
//		for (int i=0 ; i<g->getNodesNum() ; i++)
//			if (nodeArr[i+1].getExcess() > 0)
//				cout << "Node :" << i+1 << " Excess :" << nodeArr[i+1].getExcess() << endl;
//	}
//	NodePool* pool = g->getPool();
//	while (!pool->isEmpty())
//	{
//		discharge_back(pool->getNode());
//	}
//
//	if (DEBUG >= LOG_2) //Print the excess of each node
//	{
//		cout << "Excess after flow" << endl;
//		for (int i=0 ; i<g->getNodesNum() ; i++)
//			if (nodeArr[i+1].getExcess() > 0)
//				cout << "Node :" << i+1 << " Excess :" << nodeArr[i+1].getExcess() << endl;
//	}
//	return 0;
//}
//
//int PushRelabel_2::discharge_back(Node *node)
//{
//	int extra, level, quantity;
//	bool search = false;
//	EdgeEntry *edge;
//	Node *end_point;
//
//	//Nodes with no paths to target and the sink need not to be discharged
//	if ((node->getID() == g->getSource()) || (node->getLabel() == INFINITY))
//		return 0;
//
//	extra = node->getExcess();
//
//	while (extra > 0) {
//		level = findClosestPushBack(node);
//		if (level == INFINITY) break;
//
//		edge = node->getAdjList();
//		while (edge != NULL && extra > 0) 
//		{
//			if ((edge->getFlow() < edge->getCapacity()) && (nodeArr[edge->getEndPoint()].getLabel() == level)) 
//			{
//				quantity = edge->getCapacity()-edge->getFlow();
//				if (quantity > extra) 
//					quantity = extra;
//				edge->push(quantity);
//
//				end_point = &nodeArr[edge->getEndPoint()];
//
//				if (end_point->getExcess() == 0)
//					g->getPool()->addNode(end_point);
//
//				if (end_point->getID() == g->getSource()) 
//					search = true;
//
//				end_point->incExcess(quantity);
//				extra -= quantity;
//				node->decExcess(quantity);
//			}
//			edge = edge->getNext();
//		}
//	}
//
//	if (search) 
//		updateLabels(false);
//	else 
//	{
//		if (extra == 0) 
//			level = findClosestPushBack(node);
//
//		if (level >= g->getTarget()) 
//			node->setLabel(INFINITY);
//		else 
//			node->setLabel(level+1);
//	}
//
//	node->setExcess(extra);
//	return 0;
//}
//
//int PushRelabel_2::findClosestPushBack(Node* node)
//{
//	EdgeEntry* edge = node->getAdjList();
//	Node* end_point;
//    int min = INFINITY;
//
//	while (edge!=NULL) {
//		if ((edge->getFlow() < edge->getCapacity()) && edge->isReverseEdge()) 
//		{
//			end_point = &nodeArr[edge->getEndPoint()];
//			if (end_point->getLabel() < min)
//				min = end_point->getLabel();
//		}
//		edge = edge->getNext();
//	}
//
//	if (min < g->getTarget()) return(min);
//	else return(INFINITY);
//
//}
//
//EdgeEntry* PushRelabel_2::findLowestLabelEdge(Node* node)
//{
//	int min = INFINITY;
//	EdgeEntry *tmp = node->getAdjList()->getNext();
//	EdgeEntry *returnEdge = NULL;
//	while (tmp != NULL)
//	{
//		if ((nodeArr[tmp->getEndPoint()].getLabel() < min) && (tmp->getResCapacity() > 0))
//		{
//			min = nodeArr[tmp->getEndPoint()].getLabel();
//			returnEdge = tmp;
//		}
//		tmp = tmp->getNext();
//	}
//
//	return returnEdge;
//}
	