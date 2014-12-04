
#include <vector>
// White is a Node that has not been visited yet
#define WHITE 1	
// Gray is  a Node that has been reached by the algorithm but hasn't been fully visited yet
#define GRAY 2
// Black is a Node that has already been visited
#define BLACK 3

struct EdgeList;

struct Node;


class Adj_List{
	public:
		// A vector to store all nodes in the adjacency list
		std::vector<Node*> nodes;
		// A function to add nodes into the adjacency list
		void addnode(Node*);
		// A function to add weighted edges between the nodes in the adjacency list
		void addedge(Node*, Node*, int);
		// a function to overide the priority queue's comparison to mae it a minheap
		bool operator()(const Node*, const Node*) const;
		// A function to create a random adjacency list
		//Where n is the number of nodes and c is the expected degree
		void randomAdj(int, int);
		//A function to find he node with the largest degree
		Node* FindStart();
		// A function to create a SSSP tree utilizing Djikstra's algorithm
		void djikstra(Node*);
};
