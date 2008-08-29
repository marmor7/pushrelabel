#include "Graph.h"
#include "DijkstraQueue.h"
#include <queue>

Graph::Graph(string file)
{
	cout << endl << "Creating graph from file " << file << endl;
	readGraph(file);
	pool = new HighLabelQueue(nodesNum);
	//pool = new FifoQueue();
}

Graph::~Graph(void)
{
	free(pool);
	free(adjMatrix);
}

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

	if (file.length() == 0)
	{
		cerr << "readGraph: File name must exist" << endl;
		exit(1);
	}
	else 
		graphFile.open (file.c_str());

	if (graphFile.is_open())
	{
		char ch;
		int nodeId, to, cap;
		int numEdges, curNode;
		char* str = new char[10];

		while (! graphFile.eof() )
		{
			getline (graphFile, line);
			if (DEBUG >= LOG_3)
				cout << line << " " << line[0] << endl;
			if (line.length() == 0)
				continue;

			switch (line.at(0))
			{
			case 'c':
			case 's':
				break;
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
			case 'p':
				pos = line.find_first_of(" ");
				pos = line.find_first_of(" ", pos + 1);
				pos2 = line.find_first_of(" ", pos + 1);
				nodesNum = atoi(line.substr(pos, pos2).c_str());
				adjMatrix = new bool*[nodesNum];
				for (int i = 0; i <= nodesNum ; i++)
				{
					adjMatrix[i] = new bool[nodesNum];
					memset(adjMatrix[i],0,sizeof(bool)*nodesNum);
				}
				pos = pos2;
				pos2 = line.find_first_of(" ", pos + 1);
				edgesNum = atoi(line.substr(pos, line.length()).c_str());
				if (DEBUG >= LOG_1)
					cout << nodesNum << " " << edgesNum << endl;

				nodeArray = new Node[nodesNum + 1]; //Adding 1 to conform with nodes in graph file
				for (int i = 0; i <= nodesNum ; i++)
					nodeArray[i].setID(i);
				break;
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
				//Add an entry in the adjMatrix
				adjMatrix[nodeId][to] = true;
				adjMatrix[to][nodeId] = true;

				//Add the edges
				edge = new EdgeEntry(to, cap, 0, 
					nodeArray[nodeId].getLastEdge());
				reverse = new EdgeEntry(nodeId, cap, cap,
					nodeArray[to].getLastEdge());
				reverse->setReversed(edge);
				edge->setReversed(reverse);

				nodeArray[nodeId].addEdge(edge);
				nodeArray[to].addEdge(reverse);
			}
		}
		graphFile.close();
	}
	else 
	{
		cout << "readGraph: Unable to open file"; 
		return 1;
	}

	return 0;
}

int Graph::incEdgeCapacity(int from, int to, int value)
{
	Node node = getNodeArray()[from];
	EdgeEntry* list = node.getAdjList();
	if (list != NULL)
		list = list->getNext(); //1st call skips dummy
	while (list != NULL){
		if (list->getEndPoint() == to)
			break;
		list = list->getNext(); //1st call skips dummy
	}
	if (list != NULL){
		if (DEBUG >= LOG_2)
			cout << "increasing edge " << from << "->" << to << " capacity from " <<
					list->getCapacity() << " to " << list->getCapacity()+value << endl;
		list->incCapacity(value);
		return 0;
	}

	if (DEBUG >= LOG_2)
		cout << "Requested edge from " << from << 
				" to " << to << " wasn't found." << endl;

	return 1;	
}
//
//int Graph::decEdgeCapacity(int from, int to, int value)
//{
//	Node node = getNodeArray()[from];
//	EdgeEntry* list = node.getAdjList();
//	if (list != NULL)
//		list = list->getNext(); //1st call skips dummy
//	while (list != NULL){
//		if (list->getEndPoint() == to)
//			break;
//		list = list->getNext(); //1st call skips dummy
//	}
//	if (list != NULL){
//		if (DEBUG >= LOG_2)
//			cout << "decreasing edge " << from << "->" << to << " capacity from " <<
//					list->getCapacity() << " to " << list->getCapacity()-value << endl;
//		list->decCapacity(value);
//		return 0;
//	}
//
//	if (DEBUG >= LOG_2)
//		cout << "Requested edge from " << from << 
//				" to " << to << " wasn't found." << endl;
//
//	return 1;	
//}

int Graph::dijkstraPath(void)
{
	bool path_from_source = false;
	bool path_from_target = false;
	int *dist_array_source;
	int *dist_array_target;
	int i,j, total_dist, source_to_traget_dist;

	//TMP - print all the saturated edges
	//EdgeEntry* tmp;
	//for (i=1; i<nodesNum+1 ; i++)
	//{
	//	tmp = nodeArray[i].getAdjList();
	//	tmp = tmp->getNext();
	//	while (tmp != NULL)
	//	{
	//		if ((tmp->getResCapacity() == 0) && (!tmp->isReverseEdge()))
	//			cout << "Edge from " << i << " to " << tmp->getEndPoint() << " saturated" << endl;
	//		tmp = tmp->getNext();
	//	}
	//}

	dist_array_source = new int[nodesNum+1];
	memset(dist_array_source,0,sizeof(int)*(nodesNum+1));
	dist_array_target = new int[nodesNum+1];
	memset(dist_array_target,0,sizeof(int)*(nodesNum+1));
	source_to_traget_dist = nodeArray[targetID].getLabel();

	//Calc the distances from the source and from the target
	dijkstra(sourceID, dist_array_source);
	dijkstra(targetID, dist_array_target);

	//Verify with the adjMatrix
	for (i=1 ; i < nodesNum ; i++)
		for (j=1 ; j < nodesNum ; j++)
			if (adjMatrix[i][j]) //if an edge exists test it
			{
				path_from_source = (dist_array_source[i] < nodesNum) && (dist_array_source[j] == nodesNum);
				path_from_target = (dist_array_target[j] < nodesNum) && (dist_array_target[i] == nodesNum);
				total_dist = dist_array_source[i] + dist_array_target[j];
				if (path_from_source && path_from_target && (total_dist < source_to_traget_dist)) //This edge can increase the flow
					cout << "Found edge to increase flow from " << i << " to " << j << endl;
			}


	return 0;
	
}

int Graph::dijkstra(int source_id, int* &dist)
{
	int i, tmp_dist;
	Node *u, tmp_node, *relocation_node;
	EdgeEntry* edge_ptr = NULL;
	DijkstraQueue *pool = new DijkstraQueue(nodesNum+2);
	bool reverese = (source_id == targetID);


	for (i=1; i<nodesNum+1; i++)
		dist[i] = nodesNum;

	dist[source_id] = 0;

	for (i=1; i<nodesNum+1; i++)
	{
		pool->addNode(&nodeArray[i],dist[i]);
	}

	while (!pool->isEmpty())
	{
		u = pool->getNode();
		//If the new node was not reached before it means there is no path to it, continue
		if (dist[u->getID()] == nodesNum)
			continue;
		edge_ptr = u->getAdjList();
		if (edge_ptr != NULL)
			edge_ptr = edge_ptr->getNext(); //1st call skips dummy
		while (edge_ptr != NULL)
		{
			//We don't want to go backwards, so we check only the relevant edges
			if ((reverese && !edge_ptr->isReverseEdge()) ||
				(!reverese && edge_ptr->isReverseEdge()))
			{
				edge_ptr = edge_ptr->getNext();
				continue;
			}
			if ((edge_ptr->getResCapacity() == 0 && !edge_ptr->isReverseEdge()) ||
				(edge_ptr->getFlow() == 0 && edge_ptr->isReverseEdge()))
			{//We don't want saturated edges
				edge_ptr = edge_ptr->getNext();
				continue;
			}
			tmp_node = nodeArray[edge_ptr->getEndPoint()];
			tmp_dist = dist[u->getID()] + 1;
			if (dist[tmp_node.getID()] > tmp_dist)
			{
				relocation_node = pool->getNode(dist[tmp_node.getID()],tmp_node.getID());
				dist[tmp_node.getID()] = tmp_dist;
				pool->addNode(relocation_node,tmp_dist);
			}
			edge_ptr = edge_ptr->getNext();
		}
	}
	
	free(pool);
	return 0;
}

int Graph::printGraph ()
{

	cout << "Source ID: " << sourceID << ", Target ID: " << targetID << endl;

	for (int i = 0; i < nodesNum; i++)
	{
		cout << i << " ";
		nodeArray[i].printNode();
	}

	cout << "Done printing" << endl;

	return 0;
}

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
