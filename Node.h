#pragma once
#include "EdgeEntry.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <math.h>

using namespace std;


class Node
{
public:

	Node();

	int getDegree() { return degree; }
	void setDegree(int deg) { degree = deg; }
	int getLabel() { return label; }
	void setLabel(int lab) { label = lab; }
	int getExcess() { return excess; }
	void setExcess(int exc) { excess = exc; }
	void incExcess(int value) { excess += value; }
	void decExcess(int value);
	int getID() {return id; }
	void setID(int node_id) { id = node_id; }
	EdgeEntry* getAdjList() { return adjList; }
	EdgeEntry* getLastEdge() { return lastEdge; }

	void addEdge(EdgeEntry* edge);
	void printNode();
	void debugNodeDump();

private :
	int degree;
    unsigned int label;
	int excess;
	int id;
    EdgeEntry *adjList;
	EdgeEntry *lastEdge;
};

//
//extern Graph ReadGraph(),NewGraph(),CopyGraph();
//extern int RemoveEdge(),NumEdges();
//extern Edge FindEdge();
//
//#define Degree(graph,n)    (graph[n].degree)
//#define NLabel(graph,n)    (graph[n].label)
//#define Xcoord(graph,n)    (graph[n].x)
//#define Ycoord(graph,n)    (graph[n].y)
//#define FirstEdge(graph,n) (graph[n].adj_list)
//
//#define EndPoint(e) (e->endpoint)
//#define ELabel(e)   (e->label)
//#define ELabel2(e)  (e->label2)
//#define Other(e)    (e->otheredge)
//#define NextEdge(e) (e->nextedge)
//
//
//extern Graph Prim();
//extern int *EulerTraverse(),*Match(),*Weighted_Match(),*Dijkstra(),*Concomp();
//
///* Euclidean graph type */
//typedef int (*EuclidGraph)[2];
//
//extern Graph EuclidPrim();
//extern EuclidGraph ReadEuclid(),NewEuclid();
//extern int eucdist(),eucdistsq();
//
//extern int *CvxHull();
//
///* Distance matrix graph type */
//typedef int *MatrixGraph;
//
//extern int *MatrixDijkstra();
//extern Graph MatrixPrim();
//extern Graph MatrixMaxFlow();
//extern MatrixGraph ReadMatrix(), NewMatrix();
//
//
