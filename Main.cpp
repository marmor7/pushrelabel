#include "Graph.h"
#include "PushRelabel_2.h"


int main(int argc, char **argv)
{
	Node* preflowNodes = new Node[200000];
	int max;
	string file,line;
	ifstream graphFile;

	Graph *g = new Graph("3x3.grp");
	max = PushRelabel::calc(g);
	g->incPrevPathCap(5);
	PushRelabel::dijkstraPath();

	Graph *g1 = new Graph("4x4.grp");
	max = PushRelabel::calc(g1);
	g1->incPrevPathCap(5);
	PushRelabel::dijkstraPath();

	Graph *g2 = new Graph("4x4x5.grp");
	max = PushRelabel::calc(g2);
	g2->incPrevPathCap(5);
	PushRelabel::dijkstraPath();

	Graph *g3 = new Graph("6x6.grp");
	max = PushRelabel::calc(g3);
	g3->incPrevPathCap(5);
	PushRelabel::dijkstraPath();

	Graph *g4 = new Graph("7x7.grp");
	max = PushRelabel::calc(g4);
	g4->incPrevPathCap(5);
	PushRelabel::dijkstraPath();
	
	Graph *g5 = new Graph("8x8.grp");
	max = PushRelabel::calc(g5);
	g5->incPrevPathCap(5);
	PushRelabel::dijkstraPath();

	Graph *g6 = new Graph("10x10.grp");
	max = PushRelabel::calc(g6);
	g6->incPrevPathCap(5);
	PushRelabel::dijkstraPath();

	Graph *g7 = new Graph("30x30.grp");
	max = PushRelabel::calc(g7);
	g7->incPrevPathCap(5);
	PushRelabel::dijkstraPath();

	Graph *g8 = new Graph("100x100.grp");
	max = PushRelabel::calc(g8);
	g8->incPrevPathCap(5);
	PushRelabel::dijkstraPath();

	Graph *g10 = new Graph("102x102.grp");
	max = PushRelabel::calc(g10);
	g10->incPrevPathCap(5);
	PushRelabel::dijkstraPath();

	Graph *g11 = new Graph("101X101.grp");
	max = PushRelabel::calc(g11);
	g11->incPrevPathCap(5);
	PushRelabel::dijkstraPath();
	//Graph *g9 = new Graph("440x440.grp");
	//max = PushRelabel::calc(g);

	//Graph *g11 = new Graph("101X101.grp");
	//max = PushRelabel::calc(g11);
	//Graph *g12 = new Graph("gf.grp");
	//max = PushRelabel::calc(g12);
	//Graph *g13 = new Graph(argv[1]);
	//max = PushRelabel::calc(g13);	
	//Graph *g14 = new Graph("1000X4.grp");
	//max = PushRelabel::calc(g14);

	//int newMaxFlow;
	//Node* preflowNodes;
	//Graph *g = new Graph("100X100.grp");
	//preflowNodes = new Node[g->getNodesNum() + 1];
	//int max = PushRelabel::calc(g);

	//if (!g->incEdgeCapacity(614, 648, 10000)){
	//	newMaxFlow = PushRelabel::recalc(g, g->getNodeArray(), 614, 648, 10000);
	//	cout << newMaxFlow << endl;
	//}

	//g->dijkstraPath();

	//printf("Done\n");
	return(0);
}
