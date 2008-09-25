#include "Graph.h"
#include "DijkstraQueue.h"
#include <queue>
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

//Read the graph from the file and create the graph object
Graph::Graph(string file, bool hlQueue)
{
	cout << endl << "Creating graph from file " << file << 
		". Using " << (hlQueue ? "HL" : "FIFO") << " queue method" << endl;
	readGraph(file);
	if (hlQueue)
		pool = new HighLabelQueue(nodesNum);
	else
		pool = new FifoQueue();
}

//Graph destructor
Graph::~Graph(void)
{
	free(pool);
	free(nodeArray);
	free(prevArray);
}

//Read the graph from the file
//File is assumed to be in the DIMACS format
int Graph::readGraph(string file)
{
	ifstream graphFile;
	char c;
	int edges, degree, vlabel, elabel, adjNode, i, j;
	int pos, pos2;
	int xcoord, ycoord;
	int sourceId;
	int sinkId;
	string line;
	EdgeEntry *edge, *reverse;

	//Empty file name
	if (file.length() == 0)
	{
		cerr << "readGraph: File name must exist" << endl;
		exit(1);
	}
	else //Try to open the file
		graphFile.open (file.c_str());

	if (graphFile.is_open())
	{
		char ch;
		int nodeId, to, cap;
		int numEdges, curNode;
		char* str = new char[10];
		//Read the graph line by line and parse each line
		while (! graphFile.eof() )
		{
			getline (graphFile, line);
			if (DEBUG >= LOG_3)
				cout << line << " " << line[0] << endl;
			if (line.length() == 0) //Ignore empty lines
				continue;

			switch (line.at(0))
			{
			//Comments lines - ignore
			case 'c':
			case 's':
				break;
			//Source and target
			case 'n': 
				ch = line[line.find_last_of(" ") + 1];
				nodeId = atoi(line.substr(line.find_first_of(" "), line.find_last_of(" ")).c_str());
				if (DEBUG >= LOG_1)
					cout << ch << " " << nodeId << endl;
				if (ch == 's')
					sourceID = nodeId;
				else if (ch == 't')
					targetID = nodeId;
				else 
				{
					cerr << "readGraph: Bad Format!" << endl;
					return 1;
				}
				break;
			//Number of edges and nodes
			case 'p':
				pos = line.find_first_of(" ");
				pos = line.find_first_of(" ", pos + 1);
				pos2 = line.find_first_of(" ", pos + 1);
				nodesNum = atoi(line.substr(pos, pos2).c_str());
				pos = pos2;
				pos2 = line.find_first_of(" ", pos + 1);
				edgesNum = atoi(line.substr(pos, line.length()).c_str());
				if (DEBUG >= LOG_1)
					cout << nodesNum << " " << edgesNum << endl;
				//Create the node array
				nodeArray = new Node[nodesNum + 1]; //Adding 1 to conform with nodes in graph file
				//The previous array is used for the dijkstra calculations
				prevArray = new int[nodesNum + 1];
				//Set the ID for each node
				for (int i = 0; i <= nodesNum ; i++)
					nodeArray[i].setID(i);
				break;
			//An edge
			case 'a':
				pos = line.find_first_of(" ");
				pos2 = line.find_first_of(" ", pos + 1);
				nodeId = atoi(line.substr(pos, pos2).c_str());
				pos = pos2;
				pos2 = line.find_first_of(" ", pos + 1);
				to = atoi(line.substr(pos, pos2).c_str());
				pos = pos2;
				pos2 = line.find_first_of(" ", pos + 1);
				cap = atoi(line.substr(pos, line.length()).c_str());
				//Add the edges
				edge = new EdgeEntry(to, cap, 0, 
					nodeArray[nodeId].getLastEdge());
				reverse = new EdgeEntry(nodeId, cap, cap,
					nodeArray[to].getLastEdge());
				reverse->setReversed(edge);
				edge->setReversed(reverse);
				//Add the edges to the node edge list
				nodeArray[nodeId].addEdge(edge);
				nodeArray[to].addEdge(reverse);
			}
		}
		//Finished reading the file - close it
		graphFile.close();
	}
	else 
	{
		cout << "readGraph: Unable to open file"; 
		return 1;
	}

	return 0;
}

//Dijkstra's algorithms implemnted, find distances from 'source_id'
//run until you reach the other side (source->target, target->source)
//Save the path in the 'prev' array, max is used to save the max flow available
//on the path
int Graph::dijkstra(int source_id, int* &dist, int* &prev, int &max)
{
	int i, tmp_dist;
	Node *u, tmp_node, *relocation_node;
	EdgeEntry* edge_ptr = NULL;
	//The DijkstraQueue class is used here as the priority queue
	DijkstraQueue *pool = new DijkstraQueue(nodesNum+2);
	bool reverse = (source_id == targetID);
	max = INFINITY;
	//Reset the dist array for max distance
	for (i=1; i<nodesNum+1; i++)
		dist[i] = nodesNum;
	//Init the source distance
	dist[source_id] = 0;
	//Add all the nodes to the dijkstra queue
	for (i=1; i<nodesNum+1; i++)
	{
		pool->addNode(&nodeArray[i],dist[i]);
	}
	
	//The main loop
	while (!pool->isEmpty())
	{
		//Get the node with lowest distance
		u = pool->getNode();
		if (u->getID() == targetID)
			break;
		//Get all the edges
		edge_ptr = u->getAdjList();
		if (edge_ptr != NULL)
			edge_ptr = edge_ptr->getNext(); //1st call skips dummy
		//Check all edges
		while (edge_ptr != NULL)
		{
			if (!reverse)
			{
				if (edge_ptr->isReverseEdge())
				{
					edge_ptr = edge_ptr->getNext();
					continue;
				}
				else if (edge_ptr->getResCapacity() == 0)
				{
					edge_ptr = edge_ptr->getNext();
					continue;
				}
			}
			else //reverse
			{
				if (!edge_ptr->isReverseEdge())
				{
					edge_ptr = edge_ptr->getNext();
					continue;
				}
				else if (edge_ptr->getFlow() == 0)
				{
					edge_ptr = edge_ptr->getNext();
					continue;
				}
			}

			tmp_node = nodeArray[edge_ptr->getEndPoint()];
			tmp_dist = dist[u->getID()] + 1;
			//If we found a shorter path we update the node
			if (dist[tmp_node.getID()] > tmp_dist)
			{
				//Get the node to update from the queue
				relocation_node = pool->getNode(dist[tmp_node.getID()],tmp_node.getID());
				dist[tmp_node.getID()] = tmp_dist;
				prev[tmp_node.getID()] = u->getID();
				//Insert the node in its new place
				pool->addNode(relocation_node,tmp_dist);
				
			}
			//Move to the next edge
			edge_ptr = edge_ptr->getNext();
		}
	}

	//Now that the 'prev' array is ready, we can find the max flow available on
	//the path between the source and the sink
	int t = targetID;
	EdgeEntry *tmp_edge;
	while(t != sourceID)
	{
		tmp_edge = nodeArray[prevArray[t]].getAdjList();
		while (tmp_edge->getEndPoint() != t)
			tmp_edge = tmp_edge->getNext();
		if (tmp_edge->getResCapacity() < max)
			max = tmp_edge->getResCapacity();
		t = prevArray[t];
	}
	//Free the pool
	free(pool);
	return 0;
}

//Print the graph
int Graph::printGraph(string file)
{
	ofstream output;
	output.open(file.c_str());

	output << "Source ID: " << sourceID << ", Target ID: " << targetID << endl;

	for (int i = 0; i < nodesNum; i++)
	{
		nodeArray[i].printNode(output);
	}

	output.close();

	return 0;
}

//Show debug information
int Graph::debugDump()
{
	cout << "src: " << sourceID << ", dst: " << targetID << endl;
	for (int i = 0; i < nodesNum; i++)
	{
		nodeArray[i].debugNodeDump();
	}

	cout << "Debug end" << endl;

	return 0;
}

//Increase the capacity of path from source to sink by 'val'
int Graph::incPrevPathCap(int val)
{
	int i = targetID;
	EdgeEntry *tmp_edge;
	while(i != sourceID)
	{
		tmp_edge = nodeArray[prevArray[i]].getAdjList();
		while (tmp_edge->getEndPoint() != i)
			tmp_edge = tmp_edge->getNext();
		tmp_edge->incCapacity(val);
		tmp_edge->getReverse()->incCapacity(val);
		i = prevArray[i];
	}

	return 0;
}
