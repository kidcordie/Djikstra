#include "Djik.hpp"
#include <iostream>
#include <queue>
#include <cmath>
#include <string>
#include <cstdlib>
#include <functional>

/*							*
 * A Node structure that 
 * resembles a vertex in a
 * graph.  It also has the 
 * tools to place it in an
 * Adjacency list as well
 * as running a Breadth
 * First Search on it
 *							*/
struct Node{
	float dist;	// distance from searching node
	Node* Parent;	// Parent of the node in the BFS
	EdgeList* Edge;	// A linked list of edges to adjacent nodes
	int degree;	// total degree of the node (in + out)
};

/*							*
 * A structure for the linked
 * list of edges in the 
 * adjacency list.  It has
 * a pointer to it's
 * corresponding node
 * As well as the next adjacent
 * node.
 * 							*/
struct EdgeList{
	Node* adj;	// the node this edge points to
	int weight;	// weight of the node
	EdgeList* Next;	// A Pointer to the next adjacent edge
};

/*							*
 * A function that adds a node
 * to the vector that represents
 * the Adjacency List
 * 							*/
void Adj_List::addnode(Node* toadd){
	// Makes sure the pointer to the adjacent nodes is initially null
	toadd->Edge = NULL;
	// sets initial degree to 0
	toadd->degree = 0;
	// Puses the node to add to the back of the vector
	nodes.push_back(toadd);
}

/*							*
 * A function that adds an edge
 * to the corresponding node's
 * linked list of adjacent
 * nodes.
 * Adds a weight to the edge
 * 							*/
void Adj_List::addedge(Node* node, Node* edge, int weight){
	// creates a new edge for the node's edgelist
	// increase degree out of from node
	node->degree++;
	// increase degree in of to node
	edge->degree++;
	EdgeList* newedge = new EdgeList;
	// set weight of new edge
	newedge->weight = weight;
	//sets the edge's node to the corresponding adjacent node
	newedge->adj = edge;
	// makes sure the adjacent pointer to NULL
	newedge->Next = NULL;
	// if the node does not have any adjacent nodes set this as the first one
	if(node->Edge == NULL){
		node->Edge = newedge;
	}
	// Else search through till the end of the list
	else{
		// make toedge an element to traverse the linked list
		EdgeList* toedge = node->Edge;
		// run through the linked list moving to edge accordingly until
		// the last node does not point to anything
		while(toedge->Next!=NULL){
			toedge = toedge->Next;
		}
		// Have the last node point to the new edge
		toedge->Next = newedge;
	}
}

/* 										*
 * This funciton overides the compare	*
 * operator of the Priority queue to 	*
 * make it a Min Heap by comparing the 	*
 * distances of each node				*
 *										*/
bool Adj_List::operator()(const Node* lhs, const Node* rhs) const
{
    return lhs->dist < rhs->dist;
}

/*									*
 * This function creates a 			*
 * random Adjacencly List			*
 * where n is the number of nodes	*
 * and c is the expected degree		*
 * of each node						*
 * It also adds a random weight		*
 *  to each edge					*
 * 									*/
void Adj_List::randomAdj(int n, int c){
	// calculate the probability that two nodes will be adjacent
	// the probability = c/(n-1)
	// This had to be altered to n-1/c so it could be modulated
	// with a random number to test if two nodes will be adjacent
	int p = ((n-1)/c);
	// weight of the edge to be added
	int weight;
	// set a seed for the random number generator
	srand(time(NULL));
	// create n nodes
	for(int i=0 ; i<n ; ++i){
		Node* toadd = new Node;
		addnode(toadd);
	}
	// run all nodes against each other to make them adjacent
	for(unsigned i=0 ; i<nodes.size() ; ++i){
		for(unsigned j=0 ; j<nodes.size() ; ++j){
			// if the nodes are not equal (no self loops)
			// and the random number modulated with p = 1
			// Then create an directed edge between the two nodes
			if(nodes[i]!=nodes[j] && rand()%p == 1){
				// a random weight below 300 to be added
				weight = rand()%300;
				// an directed edge between the first node and the second
				addedge(nodes[i], nodes[j], weight);
			}
		}
	}
	float sum=0;
	float avg;
	for(unsigned i=0; i<nodes.size(); ++i){
		if(nodes[i]->Edge!= NULL){
			EdgeList* toedge = nodes[i]->Edge;
			while(toedge->Next!=NULL){
				sum++;
				toedge = toedge->Next;
			}
			sum++;
		}
	}
	avg = sum/nodes.size();
	std::cout << sum << " " <<avg << std::endl;
}

/*										*
 * A function to find the node with		*
 * The highest degree and use it		*
 * to run djikstra's algorithms as to 	*
 * be a part of the largest component.	*
 * 										*/
Node* Adj_List::FindStart()
{
	Node* Start;
	int maxdeg = 0;
	for(unsigned i=0; i < nodes.size(); i++){
		// if degree of the current node is higher than max degree found
		if(nodes[i]->degree > maxdeg){
			// set max degree to new degree
			maxdeg = nodes[i]->degree;
			// designate new node as start for djikstra's algorithm
			Start = nodes[i];
		}
	}
	return Start;
}

/*
void Adj_List::print(){
	for(unsigned i=0; i<nodes.size(); ++i){
			std::cout << nodes[i]->dist << std::endl;
		}
	}
*/


/*									*
 * This algorithm finds the Single 
 * Source Shortest Path in a 
 * directed graph with starting Node
 * Start.
 * This is an implementation of 
 * Djikstra's Algorithm
 * 									*/
void Adj_List::djikstra(Node* Start){
	// a Min heap to keep track of the visited nodes
	std::priority_queue<Node*> Q;
	// a pointer to the working node
	Node* X = new Node;
	// A pointer to an adjacent node
	Node* Y = new Node;
	// A pointer to the edges between adjacent nodes
	EdgeList* Neighbor = new EdgeList;
	// variables to keep track of time
	long long int atomic=0;
		// a for loop that resets each node to it's pre-search value
	for(unsigned j=0; j<nodes.size(); ++j){
		nodes[j]->Parent = NULL;	// sets the parent to NULL
		nodes[j]->dist = INFINITY;	// set's the distance between the 2 nodes to infinity;
		atomic+=2;
	}
		Start->dist = 0;	// set the distance to the starting node as 0
		// Push the starting node onto the Heap
		Q.push(Start);
		atomic+= 3;
		// While the Queue is not empty
		while(!Q.empty()){
			// Take X from the top of the minheap (smallest distance)
			X = Q.top();
			// Remove X from the Heap
			Q.pop();
			// Neighbor searches through X's adjacent edges
			Neighbor = X->Edge;
			atomic+= 3;
			while(Neighbor != NULL){
				// Y is an adjacent node to X
				Y = Neighbor->adj;
				atomic+= 1;
				// If Y's distance is less than that of X's distance + the weight between them
				// the edge between them is a tense edge and must be relaxed
				if(Y->dist > (X->dist+Neighbor->weight)){
					// Set Y's parent to X
					Y->Parent = X;
					//Set Y's distance to X's distance + Neighbor's weight
					Y->dist = X->dist + Neighbor->weight;
					atomic+= 4;
					// Add Y to the Queue to be further explored
					Q.push(Y);
					atomic+=log(Q.size());
				}
				// increment among X's adjacent nodes
				Neighbor = Neighbor->Next;
				atomic++;
			}
	}
	std::cout << " atomic operations: " << atomic << std::endl;
}
