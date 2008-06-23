/*
ACYCLIC NETWORK GENERATOR FOR MAX-FLOW (by G. Waissi) 
(revised 11/25/90)
(revised 01/04/91)
(rewritten in C and modified by J. Setubal,
July 91. Program changed to generate instances
for DIMACS algorithm implementation challenge only.)

usage: ac <num_nodes> <seed>
graph written to standard output
*/

#include <stdio.h>
#include <math.h>
#include <cstdlib> 
#include <iostream>
#include <assert.h>

int cap;
int p,q,i;
int num_arcs,num_nodes,capacity;
int tail,head;
int source;
int sink;
int input_seed;

/* RandomInteger -- return a random integer from the range 1 .. high.
*/
int RandomInteger(int high)
{
	return (rand() % high) + 1;
}

void AcyclicNet1()
{

	for (p = 1; p <= (num_nodes-1); p++)
	{
		tail = p;
		for (q = p+1; q <=  num_nodes; q++)
		{
			head = q;
			cap = RandomInteger(capacity);
			printf("a %d %d %d\n",tail,head,cap);
		}
	}
}

void Banner4()
{
	printf("c Fully Dense Acyclic Network\n");
	printf("c for Max-Flow\n");
	printf("c Arcs with random capacities\n");
	printf("p max %d %d\n",num_nodes,num_arcs);
	printf("n %d s\n",source);
	printf("n %d t\n",sink);
	printf("s %d\n", input_seed);
}

void UserValues()
{
	source = 1;
	sink = num_nodes;
	num_arcs = 0;
	for (i = 1; i <= (num_nodes-1); i++) 
		num_arcs += i;
	capacity = 1000000;
	Banner4();
	AcyclicNet1();
}

void main(int argc, char* argv[])
{
	assert(argv[0]);
	assert(argv[1]);
	assert(argv[2]);
	num_nodes = atoi(argv[1]);
	input_seed = atoi(argv[2]);
	srand(input_seed);
	UserValues();
}