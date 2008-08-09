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
	Graph *g = new Graph("3X3.grp");
	PushRelabel::calc(g);
	Graph *g1 = new Graph("4X4.grp");
	PushRelabel::calc(g1);
	Graph *g2 = new Graph("4X4X5.grp");
	PushRelabel::calc(g2);
	Graph *g3 = new Graph("6X6.grp");
	PushRelabel::calc(g3);
	Graph *g4 = new Graph("7X7.grp");
	PushRelabel::calc(g4);
	Graph *g5 = new Graph("8X8.grp");
	PushRelabel::calc(g5);
	Graph *g6 = new Graph("10X10.grp");
	PushRelabel::calc(g6);
	Graph *g7 = new Graph("30X30.grp");
	PushRelabel::calc(g7);
	Graph *g8 = new Graph("100X100.grp");
	PushRelabel::calc(g8);
	

	printf("Done\n");
	return(0);
}
