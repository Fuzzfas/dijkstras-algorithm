/**@file Graph.h*/

/*
Tin Khuong
	Student ID: 0421141
	CSS 502B
	Program 3
	Prof. Clark Olson
	2/11/20
*/

/*
	This file contains all of the declarations for the Graph class.
	The Graph class can contruct a graph from a text file, storing edges in an adjacency list. It can then compute the shortest path between every pair of vertices using Dijkstra's algorithm.
	This class can also output a shortest path table and output individual paths.

	Assumptions:
	This class assumes that the input file follows a specific format:
		The first line will contain the number of vertices n.
		Following are text descriptions of the vertices 1 through n (50 chars max length).
		After that, each line consists of 3 ints representing an edge.
		If there is a directed edge from vertex 1 to vertex 2 with a weight of 10, the line will be: 1 2 10.
		A zero for the first integer signals the end of the data.

	findShortestPath method must be called before displayAll or display() can be called.

	Input example:
	4
	Olson’s office
	Stiber's office
	STEM office
	The Commons
	1 2 10
	1 3  5
	2 4 10
	2 1 15
	3 1  5
	3 4 20
	0 0  0
*/

#include "Vertex.h"
#include <climits> //needed to represent infinity
#include <queue>  //needed for priority queue (min-heap)
using namespace std;

const bool DEBUG = false; //set to true for debugging messages

class Graph 
{

private:
	
	static const int MAX_VERTICES = 101;

	struct EdgeNode // can change to a class, if desired
	{			
		int adjVertex = 0;			// subscript of the adjacent vertex 
		int weight = 0;				// weight of edge
		int vertexID = 0;
		EdgeNode *nextEdge = nullptr;
	};

	struct myComparator
	{
		int operator()(const EdgeNode& e1, const EdgeNode& e2)
		{
			return e1.weight > e2.weight;
		}
	};

	struct VertexNode 
	{
		EdgeNode *edgeHead = nullptr;		// head of the list of edges
		Vertex *data = nullptr;		// store vertex data here

	//-------------------------------- VertexNode methods ---------------------------------

	/*-------------------------------- Default Constructor ---------------------------------
	@pre A Graph object has been constructed.
	@post Creates a new Vertex object and sets data pointer to this new Vertex object
	*/
	VertexNode();
	};
	//-------------------------------- End VertexNode methods ---------------------------------

	// table of information for Dijkstra's algorithm
	struct Table 
	{
		bool visited = false;			// whether vertex has been visited
		int dist = INT_MAX;				// shortest known distance from source, initialized to INT_MAX to represent infinity
		int path = 0;				// previous vertex in path of min dist
	};

	VertexNode vertices[MAX_VERTICES]; // array of VertexNodes, starting at index 1. Index 0 will be unused.
	int size = 0;					// number of vertices in the graph
	Table T[MAX_VERTICES][MAX_VERTICES];
	// stores visited, distance, path -
	// two dimensional in order to solve
	// for all sources
	//Index 0 in both dimensions will be unused

	//-------------------------------- Private methods ---------------------------------

	/*-------------------------------- copyHelper ---------------------------------
	Private helper method called by Copy Constructor and operator=
	Uses insertEdge to assist with deep copy pointer operations
	@pre Some other Graph object exists to be copied. Does nothing if other Graph has size 0.
		insertEdge assumed to be implemented.
	@post Copies contents of other Graph object into this one (deep copy if needed)
	*/
	void copyHelper(const Graph& other);	

	/*-------------------------------- showPath ---------------------------------
	Helper method used by displayAll to output shortest path (if it exists) between two vertices.
	Implemented using recursion.
	@pre buildGraph and findShortestPath were executed successfully. displayAll has been executed, calling this method
	@post Returns a string containing shortest path sequence from source to destination vertex (left to right)
	*/
	string showPath(int src, int next, string& result) const;

	/*-------------------------------- pathHelper ---------------------------------
	Helper method used by display to output single detailed path between two vertices. 
	Implemented using recursion.
	@pre buildGraph and findShortestPath were executed successfully. display has been executed, calling this method
	@post Outputs text to console containing vertex descriptions line by line in order from source to destination.
	*/
	void pathHelper(int src, int dest) const;

	/*-------------------------------- destroyGraph ---------------------------------
	Can be called by destructor, copy constructor, or operator=
	@pre A Graph object exists.
	@post Deallocates all dynamic memory assigned to Graph object / members of Graph object
	*/
	void destroyGraph();

public:

	/*-------------------------------- Default Constructor ---------------------------------
	@pre None
	@post Creates a Graph object with members initialized to default values.
	*/
	Graph();

	/*-------------------------------- Copy Constructor ---------------------------------
	@pre Some other Graph object exists to be copied. Does nothing if other Graph has size 0.
	@post Calls destroyGraph and copyHelper. Copies contents of other Graph object into this one (deep copy if needed).
	*/
	Graph(const Graph& other);

	/*-------------------------------- Destructor ---------------------------------
	@pre A Graph object exists
	@post Deallocates all dynamic memory assigned to Graph object / members of Graph object
	*/
	~Graph();

	/*-------------------------------- operator= ---------------------------------
	@pre: Some other Graph object exists to be copied. Does nothing if other Graph has size 0.
	@post: Calls destroyGraph and copyHelper. Deallocates left operand and copies contents of other Graph object into this one (deep copy if needed).
	*/
	Graph& operator=(const Graph& rhs);

	/*-------------------------------- buildGraph ---------------------------------
	Builds a graph by reading data from an ifstream
	@pre: infile has been successfully opened and the file contains properly formated data (according to the program specs)
	@post: One graph is read from infile and stored in the object
	*/
	void buildGraph(ifstream& infile);

	/*-------------------------------- insertEdge ---------------------------------
	Adds edge data (adjacent vertex and weight) to adjacency list, or replaces any previous edge that existed between the two vertices.
	Traverses adjacency list with pointer operations.
	@pre buildGraph method has been called and successfully opened properly formatted infile. buildGraph calls this method.
	@post Creates and adds an Edgenode object (Dynamic allocation), containing adjVertex and weight data, to vertices array. 
	If edge already exists, replaces edge with new adjVertex and weight data.
	Array index = Vertex number (Starting at index 1)
	*/
	void insertEdge(int src, int dest, int weight);

	/*-------------------------------- removeEdge ---------------------------------
	Removes an edged from an adjacency list. Does nothing if src is 0, the adjacency list is empty or if the edge does not exist.
	@pre A graph object exists.
	@post Removes an edge from the adjacency list (if it exists).
	*/
	void removeEdge(int src, int dest);

	/*-------------------------------- findShortestPath ---------------------------------
	Computes the shortest path between every pair of vertices (if path exists) using Dijkstra's algorithm.
	Implemented using a 2D array of Table, containing visited, weight, and path information.
	Executes using every vertex as a source.
	@pre Assumes buildGraph has executed successfully.
	@post Array of Table is populated with data on visited, weight, and path information
	*/
	void findShortestPath();

	void findShortestPathHeap();

	/*-------------------------------- displayAll ---------------------------------
	@pre Assumes findShortestPath has executed successfully.
	@post Outputs a table containing descriptions of all vertices, as well as shortest distance and path
	between all pairs of vertices (if path exists). If no path exists, outputs --
	Format is similar to below:

	Description                  From  To    Dist  Path
	Olson’s office
											1     2    10    1 2
											1     3	  5     1 3
											1	   4	  20    1 2 4
	Stiber's office
											2	   1	  15    2 1
											2     3	  20    2 1 3
											2	   4    10    2 4
	STEM office
											3	   1	  5     3 1
											3	   2	  15    3 1 2
											3	   4	  20    3 4
	The Commons
											4	   1	  --
											4	   2	  --
											4	   3	  --
	*/
	void displayAll() const;

	/*-------------------------------- display ---------------------------------
	Displays output for a single detailed path. Includes location descriptions in path order
	@pre Assumes findShortestPath has executed successfully.
	@post Displays output similar to below:

	2     3     20    2 1 3
	Stiber's office
	Olson’s office
	STEM office
	*/
	void display(int src, int dest) const;

	/*-------------------------------- testEdges ---------------------------------
	Used to verify of buildGraph and insertEdge were implemented correctly
	@pre buildGraph has executed and populated an adjacency list with edges.
	@post Outputs list of edges for each vertex
	*/
	void testEdges() const;	
};
