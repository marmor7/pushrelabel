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
	Graph *g = new Graph("30X30.grp");

	PushRelabel::calc(g);


	printf("Done\n");
	return(0);
}
