#include "PushRelabel.h"
#include <time.h>
#include <algorithm>
#include <ctime>
#include <stack>

//These defines are used for the BFS algorithm
#define LEVEL_UP -1 //Indicating we are one level further from the source
#define NEW_NODE -2 //Node was not reached yet
#define QUEUE_NODE -3 //Node is in the queue

//Static Members
Graph* PushRelabel::g;
Node* PushRelabel::nodeArr;
unsigned long PushRelabel::numOfPushes;
unsigned long PushRelabel::numOfRelabels;
int PushRelabel::dist;


//The main function - calculates the flow in a given graph
int PushRelabel::calc(Graph* gr)
{
	//Save pointers
	g = gr;
	nodeArr = g->getNodeArray();

	if (DEBUG >= LOG_3)
		g->printGraph();

	//Start timing
	clock_t start, finish;
	start = clock();

	//Traverse all nodes (BFS) and initialize distance labels
	updateLabels(true, false);
	//Save the distance between the source and the sink
	dist = nodeArr[g->getSource()].getLabel();

	//Set source's excess flow
	nodeArr[g->getSource()].setExcess(INFINITY);

	//Put the source in the pool
	g->getPool()->addNode(&nodeArr[g->getSource()]);

	if (DEBUG >= LOG_2)
		g->debugDump();

	//Reset the number of pushes and relabels
	PushRelabel::numOfPushes = 0;
	PushRelabel::numOfRelabels = 0;

	//Calc pre-flow
	preflow();

	if (DEBUG >= LOG_2)
		g->debugDump();

	//Print out the number of pushes and relabels done by the algorithm
	if (DEBUG >= LOG_NONE){
		cout << "# pushes: " << PushRelabel::numOfPushes << endl;
		cout << "# relables: " << PushRelabel::numOfRelabels << endl;
	}

	//The excess of the target is the maximum flow
	int maxFlow = nodeArr[g->getTarget()].getExcess();
	cout << "Max flow value is " << maxFlow << endl;
	finish = clock();
	cout << "Phase 1 total clocks: " << (finish - start) << endl << endl << endl;
	//Reset the distance labels - this time from the source (in order to push excess flow back to the source)
	updateLabels(false, true);

	//Enqueue all the nodes with Excess > 0
	assert(g->getPool()->isEmpty()); //At this point the queue should be empty
	for (int i=1 ; i< g->getNodesNum() ; i++){ //No +1 here because we don't want the sink
		if (nodeArr[i].getExcess() > 0)
		{
			g->getPool()->addNode(&nodeArr[i]);
		}
	}

	//pre-flow to flow (remove excesses)
	flow();

	//Time the end of the run
	finish = clock();
	
	cout << "Total clocks: " << (finish - start) << endl << endl << endl;

	return maxFlow;
}

//Run a BFS and update the distance labels accordingly
int PushRelabel::updateLabels(bool fromTarget, bool calcPrev)
{
	int source;

	if (fromTarget)
		source = g->getTarget();
	else
		source = g->getSource();
	//The BFS algorithm uses a standard FIFO
	queue<int> nodeQueue;
	int cur;

	int level = 0;

	//Init all to NEW_NODE
	for (int i = 1; i <= g->getNodesNum(); i++)
	{
		nodeArr[i].setLabel(NEW_NODE);
		if (calcPrev)
			g->prevArray[i] = 0;
	}
	//Push the source (distance 0) and set a level up
	nodeQueue.push(source);
	nodeQueue.push(LEVEL_UP);
	nodeArr[source].setLabel(QUEUE_NODE);

	if (DEBUG >= LOG_2)
		cout << "Updating Labels..." << endl;

	//The main loop
	while (!nodeQueue.empty())
	{	
		//Pop the next node in the queue
		cur = nodeQueue.front();
		nodeQueue.pop();

		//Level up means we are one level further from the source
		if (cur == LEVEL_UP)
		{
			if (nodeQueue.empty())
				continue;
			level++;
			nodeQueue.push(LEVEL_UP);
		}
		//We take a node marked as queue node and set its distance
		else
		{
			if (DEBUG >= LOG_3)
				cout << cur << " (" << level << "), " ;

			//Update Label
			PushRelabel::nodeArr[cur].setLabel(level);
			if (cur == g->getSource())
				g->setMaxDistance(level);

			//Enqueue all children
			EdgeEntry* edgePtr = PushRelabel::nodeArr[cur].getAdjList();
			//Skip dummy
			edgePtr = edgePtr->getNext();
			while (edgePtr != NULL)
			{
				//Only new nodes are enqueued
				if (nodeArr[edgePtr->getEndPoint()].getLabel() == NEW_NODE)
				{
					nodeQueue.push(edgePtr->getEndPoint());
					nodeArr[edgePtr->getEndPoint()].setLabel(QUEUE_NODE);
					//This is used to utilize the BFS to find a previous edges path
					if (calcPrev)
						g->prevArray[edgePtr->getEndPoint()] = cur;
				}

				edgePtr = edgePtr->getNext();
			}
		}
	}

	if (DEBUG >= LOG_3)
	{
		for (int i = 1; i <= g->getNodesNum(); i++)
			cout << i << ": " << nodeArr[i].getLabel() << "\t";
	}

	return 0;
}

//After flow() we have the max flow at the target but we still need to have excess == 0 in all
//the other nodes in the graph, this function pushes the flow back to the source
int PushRelabel::preflow()
{
	NodePool* pool = g->getPool();
	while (!pool->isEmpty())
	{
		discharge(pool->getNode());
	}

	return 0;
}

//Discharge a node, push all the extra excess according to the Push-Relable algorithm
int PushRelabel::discharge(Node* node)
{
	bool search = false;
	int push_value = 0;
	int edges = 0;
	int level = 0;
	EdgeEntry* cur;

	//Nodes with no paths to target and the sink need not to be discharged
	if ((node->getID() == g->getTarget()) || (node->getLabel() == INFINITY))
		return 0;

	//Set the first edge to cur (skip the dummy)
	level = node->getLabel();
	cur = node->getAdjList()->getNext();

	//Scan the edges
	while ((cur != NULL) && (node->getExcess() > 0))
	{
		edges++;
		//check if the arc is admissible (not saturated and label is 1 + end node label)
		if (isAdmissible(node, cur) && (cur->getEndPoint() != g->getSource()))
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

	//Once we reach the target we "normalize" the labels
	if (search) 
		updateLabels(true, false);
	else
	{
		//If the node has no more excess set the new label
		if (node->getExcess() == 0)
		{
			cur = findLowestLabelEdge(node);
			if (cur != NULL)
				level = nodeArr[cur->getEndPoint()].getLabel();
			else
				level = INFINITY;
		}
		//We don't relabel the source and the target
		if ((node->getID() != g->getSource())
			&& (node->getID() != g->getTarget()))
		{
			//Since the source and the targets labels don't chance the max label is 2Xdist-1
			if (level > (2*PushRelabel::dist)-1)
				node->setLabel(INFINITY);
			else
				node->setLabel(level+1);
			PushRelabel::numOfRelabels++;
		}
	}
	//If the node should be returned to the queue for further discharge we return it
	if (node->getExcess() > 0 && node->getLabel() <=(2*PushRelabel::dist)-1 && (node->getID() != g->getSource()) && (node->getID() != g->getTarget()))
		g->getPool()->addNode(node);

	return 0;
}

//Check if an edge is admissible for a push
bool PushRelabel::isAdmissible(Node* start, EdgeEntry* edge)
{
	return ((!edge->isSaturated()) && 
		(start->getLabel() == nodeArr[edge->getEndPoint()].getLabel() + 1));
}

//Push 'value' flow on 'edge' in node id 'start'
int PushRelabel::push(int start, EdgeEntry* edge, int value)
{
	PushRelabel::numOfPushes++;
	if (DEBUG >= LOG_2)
		cout << "push from " << start << " to " 
		<< nodeArr[edge->getEndPoint()].getID() << 
		" (" << value << "), excess was: " <<
		Utils::printValue(nodeArr[edge->getEndPoint()].getExcess()) << ", excess now: ";

	edge->push(value);
	nodeArr[edge->getEndPoint()].incExcess(value);
	nodeArr[start].decExcess(value);

	if (DEBUG >= LOG_2)
	{
		cout << Utils::printValue(nodeArr[edge->getEndPoint()].getExcess()) << endl;
	}
	return 0;
}

//Push flow from the source to the sink, after this function
//some nodes will be left with excess > 0 but the target will have its max flow
int PushRelabel::flow()
{
	if (DEBUG >= LOG_2) //Print the excess of each node
	{
		cout << "Excess before flow" << endl;
		for (int i=0 ; i<g->getNodesNum() ; i++)
			if (nodeArr[i+1].getExcess() > 0)
				cout << "Node :" << i+1 << " Excess :" << nodeArr[i+1].getExcess() << endl;
	}
	//Extract nodes from the pull and push the flow back
	NodePool* pool = g->getPool();
	while (!pool->isEmpty())
	{
		discharge_back(pool->getNode());
	}

	if (DEBUG >= LOG_2) //Print the excess of each node
	{
		cout << "Excess after flow" << endl;
		for (int i=0 ; i<g->getNodesNum() ; i++)
			if (nodeArr[i+1].getExcess() > 0)
				cout << "Node :" << i+1 << " Excess :" << nodeArr[i+1].getExcess() << endl;
	}
	return 0;
}

//Push the flow back from a given node
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
	//Main loop - as long as we still have excess in the node
	while (extra > 0) {
		//Find the closest node to the source to push back
		level = findClosestPushBack(node);
		//No where to push back
		if (level == INFINITY) break;
		//Find an edge to push back
		edge = node->getAdjList();
		while (edge != NULL && extra > 0) 
		{
			//If we found a pushback - push back
			if ((edge->getFlow() < edge->getCapacity()) && (nodeArr[edge->getEndPoint()].getLabel() == level)) 
			{
				quantity = edge->getCapacity()-edge->getFlow();
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
				node->decExcess(quantity);
			}
			edge = edge->getNext();
		}
	}
	//We reached the source - update the labels
	if (search) 
		updateLabels(false, false);
	else 
	{
		//Update the labels
		if (extra == 0) 
			level = findClosestPushBack(node);
		if (level > (2*PushRelabel::dist)-1)
			node->setLabel(INFINITY);
		else
			node->setLabel(level+1);

	}

	node->setExcess(extra);
	return 0;
}

//Find the level closest to the source where you can push back
int PushRelabel::findClosestPushBack(Node* node)
{
	EdgeEntry* edge = node->getAdjList();
	Node* end_point;
    int min = INFINITY;
	//Scan the edges to find the min label
	while (edge!=NULL) {
		if ((edge->getFlow() < edge->getCapacity()) && edge->isReverseEdge()) 
		{
			end_point = &nodeArr[edge->getEndPoint()];
			if (end_point->getLabel() < min)
				min = end_point->getLabel();
		}
		edge = edge->getNext();
	}

	if (min < g->getTarget()) return(min);
	else return(INFINITY);

}

//Find the edge with the lowest label to return
EdgeEntry* PushRelabel::findLowestLabelEdge(Node* node)
{
	int min = INFINITY;
	EdgeEntry *tmp = node->getAdjList()->getNext();
	EdgeEntry *returnEdge = NULL;
	//Scan the edges
	while (tmp != NULL)
	{
		if ((nodeArr[tmp->getEndPoint()].getLabel() < min) && (tmp->getResCapacity() > 0))
		{
			min = nodeArr[tmp->getEndPoint()].getLabel();
			returnEdge = tmp;
		}
		tmp = tmp->getNext();
	}

	return returnEdge;
}

//Find a path using Dijkstra's algorithm
int PushRelabel::dijkstraPath(void)
{
	int *dist;
	int *prev;
	int i,max = INFINITY;
	EdgeEntry *tmp_edge;
	stack<int> s;
	//Time the recalc
	clock_t start, finish;
	start = clock();
	//Allocate the dist and prev ararys
	dist = new int[g->getNodesNum()+1];
	memset(dist,0,sizeof(int)*(g->getNodesNum()+1));
	prev = new int[g->getNodesNum()+1];
	memset(prev,0,sizeof(int)*(g->getNodesNum()+1));

	//Calc the distances from the source and from the target
	g->dijkstra(g->getSource(), dist, prev, max);
	//Scan the path found by the algorithm
	//Since we want to go from source to target we reverse the path using a stack
	i = g->getTarget();
	while( i!=0 )
	{
		s.push(i);
		i = prev[i];
	}
	//Now we follow the stack from source to target pushing the max flow found by Dijkstra's algorithm
	i = s.top();
	s.pop();
	while(i != g->getTarget())
	{
		tmp_edge = nodeArr[i].getAdjList();
		while (tmp_edge->getEndPoint() != s.top())
			tmp_edge = tmp_edge->getNext();
		push(i,tmp_edge,max);
		i = s.top();
		s.pop();
	}
	//Free the arrays
	free(dist);
	free(prev);
	//End time
	finish = clock();
	cout << "New max flow: " << nodeArr[g->getTarget()].getExcess() << endl;
	cout << "Total clocks for recalc: " << (finish - start) << endl << endl << endl;

	return 0;
	
}