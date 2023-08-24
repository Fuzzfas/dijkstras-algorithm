/**@file Graph.cpp*/

/*
Tin Khuong
	Student ID: 0421141
	CSS 502B
	Program 3
	Prof. Clark Olson
	2/11/20
*/

/*
	This file contains all of the definitions for the Graph class.
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

#include <fstream>
#include "Graph.h"
using namespace std;

//-------------------------------- VertexNode methods ---------------------------------

/*-------------------------------- Default Constructor ---------------------------------
@pre A Graph object has been constructed.
@post Creates a new Vertex object and sets data pointer to this new Vertex object
*/
Graph::VertexNode::VertexNode()
{
	data = new Vertex;
}
//-------------------------------- End VertexNode methods ---------------------------------

/*-------------------------------- Default Constructor ---------------------------------
@pre None
@post Creates a Graph object with members initialized to default values.
*/
Graph::Graph()
{
}

/*-------------------------------- Copy Constructor ---------------------------------
@pre Some other Graph object exists to be copied. Does nothing if other Graph has size 0.
@post Calls destroyGraph and copyHelper. Copies contents of other Graph object into this one (deep copy if needed).
*/
Graph::Graph(const Graph& other)
{
	if(other.size != 0)
	{		
		destroyGraph();
		copyHelper(other);
	}
}

/*-------------------------------- copyHelper ---------------------------------
Private helper method called by Copy Constructor and operator=
Uses insertEdge to assist with deep copy pointer operations
@pre Some other Graph object exists to be copied. Does nothing if other Graph has size 0.
	insertEdge assumed to be implemented.
@post Copies contents of other Graph object into this one (deep copy if needed)
*/
void Graph::copyHelper(const Graph& other)
{
	size = other.size;
	
	//copy array of VertexNodes, start at index 1
	for(int i = 1; i <= size; i++)
	{		
		//make temporary cur pointers for this and other Graph object
		EdgeNode * otherCur = other.vertices[i].edgeHead;
		
		while(otherCur != nullptr)
		{
			//call InsertEdge
			insertEdge(i, otherCur->adjVertex, otherCur->weight);			
			otherCur = otherCur->nextEdge;
		}
		vertices[i].data = new Vertex;
		*vertices[i].data = *other.vertices[i].data;
	}
	
	//copy table, start at index 1
	for(int s = 1; s <= size; s++)
	{
		for(int d = 1; d <= size; d++)
		{
			T[s][d].visited = other.T[s][d].visited;
			T[s][d].dist = other.T[s][d].dist;
			T[s][d].path = other.T[s][d].path;
		}
	}
}

/*-------------------------------- Destructor ---------------------------------
@pre A Graph object exists
@post Deallocates all dynamic memory assigned to Graph object / members of Graph object
*/
Graph::~Graph()
{
	destroyGraph();
}

/*-------------------------------- destroyGraph ---------------------------------
Can be called by destructor, copy constructor, or operator=
@pre A Graph object exists. 
@post Deallocates all dynamic memory assigned to Graph object / members of Graph object
*/
void Graph::destroyGraph()
{
	//Deallocate adjacency list
	for (int v = 1; v <= size; v++)
	{
		for(int d = 1; d <=size; d++)
		{
			removeEdge(v, d);
		}		
	}

	//Deallocate all vertex data in vertices array.
	for (int v = 0; v < MAX_VERTICES; v++)
	{
		delete vertices[v].data;
		vertices[v].data = nullptr;
	}

	//Reset Table, in case copy constructor or operator= is called with a Graph object that has smaller size compared to original
	for (int s = 1; s <= size; s++)
	{
		for (int d = 1; d <= size; d++)
		{
			T[s][d].visited = false;
			T[s][d].dist = INT_MAX;
			T[s][d].path = 0;
		}
	}
}

/*-------------------------------- operator= ---------------------------------
@pre: Some other Graph object exists to be copied. Does nothing if other Graph has size 0.
@post: Calls destroyGraph and copyHelper. Deallocates left operand and copies contents of other Graph object into this one (deep copy if needed).
*/
Graph& Graph::operator=(const Graph& rhs)
{

	if (rhs.size != 0 && this != &rhs)
	{
		//Deallocate left operand, then copy
		destroyGraph();
		copyHelper(rhs);
	}
	return *this;
}

/*-------------------------------- buildGraph ---------------------------------
Builds a graph by reading data from an ifstream
@pre: infile has been successfully opened and the file contains properly formated data (according to the program specs)
@post: One graph is read from infile and stored in the object
*/
void Graph::buildGraph(ifstream& infile) 
{
	infile >> size;                          // data member stores array size
	if (infile.eof())
		return;
	infile.ignore();                         // throw away '\n' to go to next line

	// get descriptions of vertices
	for (int v = 1; v <= size; v++) 
	{		
		infile >> *vertices[v].data;			//store descriptions		
	}

	//test vertices
	if(DEBUG == true)
	{
		for (int v = 1; v <= size; v++)
		{
			cout << *vertices[v].data << endl;
		}
	}

	// fill cost edge array
	int src = 1, dest = 1, weight = 1;
	for (;;) //infinite loop, only exited when condition for break is met
	{
		infile >> src >> dest >> weight;
		if (src == 0 || infile.eof())
			break;
		insertEdge(src, dest, weight);
	}
}

/*-------------------------------- insertEdge ---------------------------------
Adds edge data (adjacent vertex and weight) to adjacency list, or replaces any previous edge that existed between the two vertices.
Traverses adjacency list with pointer operations.
@pre buildGraph method has been called and successfully opened properly formatted infile. buildGraph calls this method.
@post Creates and adds an Edgenode object (Dynamic allocation), containing adjVertex and weight data, to vertices array.
If edge already exists, replaces edge with new adjVertex and weight data.
Array index = Vertex number (Starting at index 1)
*/
void Graph::insertEdge(int src, int dest, int weight)
{
	
	EdgeNode* curEdge = nullptr; //temp pointer
	curEdge = vertices[src].edgeHead; //set temp pointer to head
	EdgeNode* prevEdge = nullptr;

	//A zero for the first integer signals the end of the data.
	if(src == 0)
	{
		return;
	}

	//first edge 
	if (vertices[src].edgeHead == nullptr)
	{
		if (DEBUG == true)
		{
			cout << "First edge " << endl;
		}
		curEdge = new EdgeNode;
		curEdge->adjVertex = dest;
		curEdge->weight = weight;
		curEdge->vertexID = src;
		vertices[src].edgeHead = curEdge; //point head to first edge
	}

	//new edge after first
	else
	{
		//traverse list
		while (curEdge != nullptr)
		{
			//replace existing edge if found
			if (curEdge->adjVertex == dest)
			{
				if (DEBUG == true)
				{
					cout << "Replace existing edge " << endl;
				}

				curEdge->adjVertex = dest;
				curEdge->weight = weight;
				curEdge->vertexID = src;
				return; // exit function early if matching edge is replaced
			}

			//move onto next edge if no match found
			else
			{
				if (DEBUG == true)
				{
					cout << "Next edge " << endl;
				}
				prevEdge = curEdge;
				curEdge = curEdge->nextEdge;
			}
		}

		//reached end of adjacency list, edge does not exist, add new edge
		if (DEBUG == true)
		{
			cout << "Add new edge " << endl;
		}

		if (prevEdge!= nullptr)
		{
			prevEdge->nextEdge = new EdgeNode;
			prevEdge->nextEdge->adjVertex = dest;
			prevEdge->nextEdge->weight = weight;
			prevEdge->nextEdge->vertexID = src;
		}		
	}
	prevEdge = nullptr;
	curEdge = nullptr;
}

/*-------------------------------- removeEdge ---------------------------------
Removes an edged from an adjacency list. Does nothing if src is 0, the adjacency list is empty or if the edge does not exist.
@pre A graph object exists.
@post Removes an edge from the adjacency list (if it exists).
*/
void Graph::removeEdge(int src, int dest)
{
	EdgeNode* curEdge = nullptr; //temp pointer
	curEdge = vertices[src].edgeHead; //set temp pointer to head
	EdgeNode* prevEdge = curEdge;

	//A zero for the first integer signals the end of the data (not valid for removeEdge method).
	if (src == 0)
	{
		return;
	}

	//Empty list, do nothing
	if (vertices[src].edgeHead == nullptr)
	{
		return;
	}

	else
	{
		//traverse list
		while (curEdge != nullptr)
		{
			//replace existing edge if found
			if (curEdge->adjVertex == dest)
			{
				if (DEBUG == true)
				{
					cout << "Removed existing edge " << endl;
				}

				if(prevEdge != nullptr)
				{
					//if target is at the head of the list
					if(curEdge == vertices[src].edgeHead)
					{
						if (DEBUG == true)
						{
							cout << "Removed edge from head " << endl;
						}

						vertices[src].edgeHead = curEdge->nextEdge; //re-assign head						
					}

					//if the target edge is not the last in the list
					else if(curEdge->nextEdge != nullptr)
					{
						if (DEBUG == true)
						{
							cout << "Removed edge from middle " << endl;
						}

						prevEdge->nextEdge = curEdge->nextEdge; //connect to node after target
					}
					
					else
					{
						if (DEBUG == true)
						{
							cout << "Removed edge from tail " << endl;
						}
						prevEdge->nextEdge = nullptr;						
					}
					//Deallocate target edge, set pointers to nullptr
					delete curEdge;
					curEdge = nullptr;
					prevEdge = nullptr;
				}				
				return; // exit function early if matching edge is removed
			}

			//move onto next edge if no match found
			else
			{
				if (DEBUG == true)
				{
					cout << "Next edge " << endl;
				}
				prevEdge = curEdge;
				curEdge = curEdge->nextEdge;
			}
		}//End list traversal while loop

		if (DEBUG == true)
		{
			cout << "Edge not found" << endl;
		}
		return;
	}
}

/*-------------------------------- findShortestPath ---------------------------------
Computes the shortest path between every pair of vertices (if path exists) using Dijkstra's algorithm.
Implemented using a 2D array of Table, containing visited, weight, and path information.
Executes using every vertex as a source.
@pre Assumes buildGraph has executed successfully.
@post Array of Table is populated with data on visited, weight, and path information
*/
void Graph::findShortestPath()
{
	if (DEBUG == true)
	{
		cout << "---------------------------------" << endl;
		cout << "findShortestPath() : " << endl;
	}
	//Use 2D Array Table
	//s for source, d for destination vertices

	//Visit next unvisited vertex with lowest weight from source
	//Repeat n-1 times
	for(int s = 1; s <= size; s++)
	{
		T[s][s].dist = 0; //set source distance to 0

		if (DEBUG == true)
		{
			cout << "---------------------------------" << endl;
			cout << "S = : " << s << endl;
			cout << "---------------------------------" << endl;
		}
		
		for(int d = 1; d <= size; d++)
		{
			//initialize minimum distance to infinity for each destination
			int minDist = INT_MAX;

			if (DEBUG == true)
			{
				cout << "---------------------------------" << endl;
				cout << "S = : " << s << endl;
				cout << "D = : " << d << endl;
				cout << "---------------------------------" << endl;
			}			

			//Let v be the unvisited vertex with minimum dist
			int v = 0;

			for(int i = 1; i <= size; i++)
			{
				if(T[s][i].visited == false && T[s][i].dist <= minDist)
				{
					minDist = T[s][i].dist;
				}
				
				if (DEBUG == true)
				{
					cout << "i: " << i << endl;
					cout << "minDist: " << minDist << endl;
				}
			}

			for (int i = 1; i <= size; i++)
			{
				if (T[s][i].visited == false && T[s][i].dist <= minDist)
				{
					v = i;
					
					if (DEBUG == true)
					{
						cout << "v: " << v << endl;
						cout << "i: " << i << endl;
						cout << "minDist: " << minDist << endl;
					}
				}
			}
			T[s][v].visited = true; //Mark current vertex as visited

			if (DEBUG == true)
			{
				cout << "Marked: " << v << " as visited" << endl;
			}

			//Make sure directed edge exists for source
			EdgeNode* tempEdge = vertices[s].edgeHead;
			bool edgeExists = false;
			while (tempEdge != nullptr)
			{
				if (tempEdge->adjVertex == v || s == v)
				{
					edgeExists = true;
					tempEdge = tempEdge->nextEdge;
				}
				else
				{
					tempEdge = tempEdge->nextEdge;
				}
			}

			//For each vertex adjacent to v...
			//Check adjacency list for this
			EdgeNode* curEdge = vertices[v].edgeHead;
			while(curEdge != nullptr)
			{
				//If vertex is unvisited and edge exists for it
				if(T[s][curEdge->adjVertex].visited == false && edgeExists == true)
				{
					if (DEBUG == true)
					{
						cout << "unvisited adjVertex: " << curEdge->adjVertex << endl;
						cout << "best known dist so far: " << T[s][curEdge->adjVertex].dist << endl;
						cout << "best distance from s to v: " << T[s][v].dist << endl;
						cout << "edge cost from v to w: " << curEdge->weight << endl;
					}

					//If(Best known distance so far > Best distance from s to v + edge cost from v to w)
					if(T[s][curEdge->adjVertex].dist > T[s][v].dist + curEdge->weight)
					{
						T[s][curEdge->adjVertex].dist = T[s][v].dist + curEdge->weight;
						T[s][curEdge->adjVertex].path = v;				//set path to currently visited vertex
					}
				}				
				//Move to next edge
				curEdge = curEdge->nextEdge;
			}

			if (DEBUG == true)
			{				
				cout << "best distance from s to v: " << T[s][v].dist << endl;
				
			}
		}
	}
}

void Graph::findShortestPathHeap()
{
	//Create min-heap
	priority_queue <EdgeNode, vector<EdgeNode>, myComparator> pq;

	//Populate priority queue with edges
	//for (int s = 1; s <= size; s++)
	//{
	//	EdgeNode* curEdge = nullptr; //temp pointer
	//	curEdge = vertices[s].edgeHead; //set temp pointer to head
	//	while(curEdge != nullptr)
	//	{
	//		pq.push(*curEdge);
	//		curEdge = curEdge->nextEdge;
	//	}
	//	curEdge = nullptr;
	//}


	//Use 2D Array Table
	//s for source, d for destination vertices
	//Iterate through all vertices as sources
	for (int s = 1; s <= size; s++)
	{
		//mark source vertex as visited
		T[s][s].visited = true;
		T[s][s].dist = 0; //set source distance to 0
		EdgeNode* curEdge = nullptr; //temp pointer
		curEdge = vertices[s].edgeHead; //set temp pointer to head
		while (curEdge != nullptr)
		{
			pq.push(*curEdge);
			curEdge = curEdge->nextEdge;
		}
		curEdge = nullptr;

		while(!pq.empty())
		{
			EdgeNode x = pq.top();
			pq.pop();
			//for each vertex w adjacent to x
			{
				int w = x.adjVertex;
				int v = x.vertexID;
				//if w is not visited
				if(T[s][w].visited == false)
				{
					//remember that the shortest path to w comes from x
					//the shortest path distance is __ plus the distance to x
					T[s][w].dist = T[s][v].dist + x.weight;
					//mark w as visited
					T[s][w].path = v;
					T[s][w].visited = true;
					pq.push(*x.nextEdge);
				}
			}
		}

		for (int d = 1; d <= size; d++)
		{
			

			//find unvisited node with min distance from source vertex
			EdgeNode temp = pq.top();
			int v = temp.adjVertex;

		}
	}
}

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
void Graph::displayAll() const
{	
	if(size > 0)
	{
		cout << "Description                                                  From   To   Dist   Path " << endl;
	
		for(int v = 1; v <= size; v++)
		{
			cout << *vertices[v].data << endl;

			for(int d = 1; d <= size; d++)
			{
				if (v != d)
				{
					int spacer = 7;
					string tempDist = "";
					string space = "";
					string pathresult = to_string(d);

					if (T[v][d].dist == INT_MAX)
					{
						tempDist = "--";
					}
					else
					{
						tempDist += to_string(T[v][d].dist);
					}
					spacer -= tempDist.length();
					
					for (int i = 0; i < spacer; i++)
					{
						space += " ";
					}
					cout << "                                                             " << v << "      " << d << "    " << tempDist << space << showPath(v, d, pathresult) << endl; 					
				}			
			}
		}
	}
}

/*-------------------------------- display ---------------------------------
Displays output for a single detailed path. Includes location descriptions in path order
@pre Assumes findShortestPath has executed successfully.
@post Displays output similar to below:

2     3     20    2 1 3
Stiber's office
Olson’s office
STEM office
*/
void Graph::display(int src, int dest) const
{
	int spacer = 7;
	string tempDist = "";
	string space = "";
	string pathresult = to_string(dest);

	if (T[src][dest].dist == INT_MAX)
	{
		tempDist = "--";
	}
	else
	{
		tempDist += to_string(T[src][dest].dist);
	}
	spacer -= tempDist.length();

	for (int i = 0; i < spacer; i++)
	{
		space += " ";
	}

	cout << src << "      " << dest << "    " << tempDist << space << showPath(src, dest, pathresult) << endl;
	
	pathHelper(src,dest);
}

/*-------------------------------- testEdges ---------------------------------
Used to verify of buildGraph and insertEdge were implemented correctly
@pre buildGraph has executed and populated an adjacency list with edges.
@post Outputs list of edges for each vertex
*/
void Graph::testEdges() const
{	
	cout << "List of Edges: (src, dest, weight): " << endl;

	for (int v = 1; v <= size; v++)
	{
		EdgeNode* curEdge = nullptr; //temp pointer
		curEdge = vertices[v].edgeHead; //set temp pointer to head
		int x = 1;

		while(curEdge != nullptr)
		{
			cout << v << " " << curEdge->adjVertex << " " << curEdge ->weight << endl;
			curEdge = curEdge->nextEdge;

			if (DEBUG == true)
			{
				
				cout << "Number of Edges: " << x << endl;
				x++;
			}
		}
		curEdge = nullptr;
	}
}

/*-------------------------------- showPath ---------------------------------
Helper method used by displayAll to output shortest path (if it exists) between two vertices.
Implemented using recursion.
@pre buildGraph and findShortestPath were executed successfully. displayAll has been executed, calling this method
@post Returns a string containing shortest path sequence from source to destination vertex (left to right)
*/
string Graph::showPath(int src, int dest, string& result) const
{	
	if (DEBUG == true)
	{
		cout << "current path results: "<<result <<endl;
		cout << "previous vertex: " << T[src][dest].path << endl;
	}

	//Check if path exists. Return empty string if path doesn't exist.
	if(T[src][dest].path == 0)
	{
		result = "";
		return result;
	}

	//exit when path = source
	if (src == T[src][dest].path)
	{
		if (DEBUG == true)
		{
			cout << "Exit recursion " << endl;
		}		
		return result = to_string(T[src][dest].path) + " " + result;
	}

	else
	{
		result = to_string(T[src][dest].path) + " " + result;
		result = showPath(src, T[src][dest].path, result);
		return result;
	}
}

/*-------------------------------- pathHelper ---------------------------------
Helper method used by display to output single detailed path between two vertices.
Implemented using recursion.
@pre buildGraph and findShortestPath were executed successfully. display has been executed, calling this method
@post Outputs text to console containing vertex descriptions line by line in order from source to destination.
*/
void Graph::pathHelper(int src, int dest) const
{
	////Check if path exists. Exits function if path doesn't exist.
	if (T[src][dest].path == 0)
	{
		return;
	}

	if (src == T[src][dest].path)
	{
		
		cout << *vertices[src].data << endl;
		cout << *vertices[dest].data << endl;
	}

	else
	{
		
		pathHelper(src, T[src][dest].path);		
		cout << *vertices[dest].data << endl;
	}
}
