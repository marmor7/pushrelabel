#include "Graph.h"
#include "PushRelabel.h"


int main(int argc, char **argv)
{
	int max;
	string file;
	ifstream graphFile;

	bool hlQueue = false;
	bool doInc = false;
	int incVal = 0;
	string incFile;
	bool printStats = false;
	bool doOutFile;
	string resFile;

	if ((argc > 10) || (argc < 2))
	{
		cout << "Wrong number of arguments" << endl;
		cout << "Usage: pushrelabel FILE [-c INC NEW-OUT] [-s] [-o OUT] [-m HL/FIFO]" << endl;
		exit(1);
	}
	else
	{
		file = argv[1];

		int i = 2;
		while (i < argc)
		{
			if (strncmp(argv[i], "-c", 3) == 0)
			{
				if (i+2 > argc)
				{
					cout << "Wrong number of arguments" << endl;
					cout << "Usage: pushrelabel FILE [-c INC NEW-OUT] [-s] [-o OUT] [-m HL/FIFO]" << endl;
					exit(1);
				}
				doInc = true;
				i++;
				incVal = atoi(argv[i]);
				i++;
				incFile = argv[i];
			}
			else if (strncmp(argv[i], "-s", 3) == 0)
			{
				printStats = true;
			}
			else if (strncmp(argv[i], "-o", 3) == 0)
			{
				if (i+1 > argc)
				{
					cout << "Wrong number of arguments" << endl;
					cout << "Usage: pushrelabel FILE [-c INC NEW-OUT] [-s] [-o OUT] [-m HL/FIFO]" << endl;
					exit(1);
				}
				doOutFile = true;
				i++;
				resFile = argv[i];
			}
			else if (strncmp(argv[i], "-m", 3) == 0)
			{
				if (i+1 > argc)
				{
					cout << "Wrong number of arguments" << endl;
					cout << "Usage: pushrelabel FILE [-c INC NEW-OUT] [-s] [-o OUT] [-m HL/FIFO]" << endl;
					exit(1);
				}
				i++;
				if (strncmp(argv[i], "HL", 3) == 0)
					hlQueue = true;
				else
					hlQueue = false;
			}
			else 
			{
				cout << "Bad argument - " << argv[i] << endl;
				cout << "Usage: pushrelabel FILE [-c INC NEW-OUT] [-s] [-o OUT] [-m HL/FIFO]" << endl;
				exit(1);
			}

			i++;
		}
	}

	Graph *g = new Graph(file, hlQueue);
	max = PushRelabel::calc(g, printStats);
	if (doOutFile)
		g->printGraph(resFile);
	if (doInc)
	{
		g->incPrevPathCap(incVal);
		PushRelabel::dijkstraPath();
		g->printGraph(incFile);
	}

	printf("Done\n");
	return(0);
}
