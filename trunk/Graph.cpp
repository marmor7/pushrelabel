#include "Graph.h"

Graph::Graph(string file)
{
	readGraph(file);
	pool = new FifoQueue();
}

Graph::~Graph(void)
{
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
		int numNodes, numEdges, curNode;
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
				pos = pos2;
				pos2 = line.find_first_of(" ", pos + 1);
				edgesNum = atoi(line.substr(pos, line.length()).c_str());

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
				
				edge = new EdgeEntry(to, cap, 
					nodeArray[nodeId].getLastEdge());
				reverse = new EdgeEntry(nodeId, cap, 
					nodeArray[to].getLastEdge());
				reverse->reverseEdge = edge;
				edge->reverseEdge = reverse;

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
};

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