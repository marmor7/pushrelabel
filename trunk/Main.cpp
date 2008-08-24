#include "Graph.h"
#include "PushRelabel.h"

/**
 ** TODOS:
 ** - search - done!
 ** - stage1:
 ** - - pulse:
 ** - - - FIFO selection
 ** - - - HL selection
 ** - - - edge selection method (min?)
 ** - - - relabling heuristic (periodic)
 ** - stage2:
 ** - - pulse2: ...
 **/


int main(int argc, char **argv)
{
	Node* preflowNodes = new Node[200000];
	int max;
	Graph *g = new Graph("3X3.grp");
	max = PushRelabel::calc(g, preflowNodes);
	Graph *g1 = new Graph("4X4.grp");
	max = PushRelabel::calc(g1, preflowNodes);
	Graph *g2 = new Graph("4X4X5.grp");
	max = PushRelabel::calc(g2, preflowNodes);
	Graph *g3 = new Graph("6X6.grp");
	max = PushRelabel::calc(g3, preflowNodes);
	Graph *g4 = new Graph("7X7.grp");
	max = PushRelabel::calc(g4, preflowNodes);
	Graph *g5 = new Graph("8X8.grp");
	max = PushRelabel::calc(g5, preflowNodes);
	Graph *g6 = new Graph("10X10.grp");
	max = PushRelabel::calc(g6, preflowNodes);
	Graph *g7 = new Graph("30X30.grp");
	max = PushRelabel::calc(g7, preflowNodes);
	Graph *g8 = new Graph("100X100.grp");
	max = PushRelabel::calc(g8, preflowNodes);
	Graph *g9 = new Graph("440X440.grp");
	max = PushRelabel::calc(g9, preflowNodes);

	//int newMaxFlow;
	//Node* preflowNodes;
	//Graph *g = new Graph("30X30.grp");
	//preflowNodes = new Node[g->getNodesNum() + 1];
	//int max = PushRelabel::calc(g, preflowNodes);

	//if (!g->incEdgeCapacity(614, 648, 10000)){
	//	newMaxFlow = PushRelabel::recalc(g, g->getNodeArray(), 614, 648, 10000);
	//	cout << newMaxFlow << endl;
	//}

	//for (int i=10000; i > 0; i--){
	//	for (int j=10000; j > 0; j--){
	//		if (!g->incEdgeCapacity(i, j, 10000)){
	//			cout << "recalc " << i << " " << j << endl;
	//			newMaxFlow = PushRelabel::calc(g, preflowNodes);
	//			if (newMaxFlow > max)
	//				break;
	//		}
	//	}
	//	if (newMaxFlow > max)
	//		break;
	//}

	printf("Done\n");
	return(0);
}