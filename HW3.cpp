//---------------------------------------------------------------------------
// HW3.CPP
// Driver code for testing the Graph class.
// Authors:  Clark Olson and Carol Zander
//---------------------------------------------------------------------------
// This code tests the basic functionality of the class to
// perform Dijkstra's algorithm for CSS 502 Program 3.
// It is not meant to exhaustively test the class.
//
// Assumptions:
//   -- a text file named "HW3.txt" exists in the same directory
//      as the code and is formatted as described in the specifications
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include "Graph.h"
using namespace std;


#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


void testGraph()
{

   ifstream infile1("HW3.txt");
   if (!infile1) {
      cerr << "File could not be opened." << endl;
      return;
   }

   ifstream infile2("HW4.txt");
   if (!infile2) {
      cerr << "File could not be opened." << endl;
      return;
   }

   //for each graph, find the shortest path from every node to all other nodes
   for (;;)
   {
      //Graph G;
      //G.buildGraph(infile1);       // see supplied code	
      //if (infile1.eof())
      //   break;

   //   G.testEdges();
      /*G.removeEdge(1,2);
      G.removeEdge(1,3);
      G.testEdges();*/


      //G.findShortestPath();

      //G.displayAll();              // display shortest distance, path to cout

      //int v1 = 2;
      //int v2 = 3;
      //cout << endl << "The path from vertex " << v1 << " to vertex " << v2 << " is:" << endl;
      //G.display(v1, v2);             // display path from node 3 to 1 to cout
      //cout << endl;


    /*  Graph G2;
      G2.buildGraph(infile2);
      if (infile2.eof())
         break;*/

     

      //Test copy constructor
     /* cout << "Test copy constructor: " << endl;
      cout << "---------------------- " << endl;

      Graph G3(G2);

      cout << "G2 edges and displayAll: " << endl;
      G2.testEdges();

      G2.displayAll();
      G2 = G;*/
      /*G2.removeEdge(1,2);
      G2.removeEdge(1, 3);
      G2.removeEdge(1, 4);*/

     /* cout << "G2 edges after operator=: " << endl;
      G2.testEdges();
      cout << "G2 display all after operator=: " << endl;
      G2.displayAll();

      cout << "G3 edges copied from G2: " << endl;
      G3.testEdges();
      cout << "G3 displayAll: " << endl;
      G3.displayAll();

      cout << "---------------------- " << endl;
      cout << "Test destroy G: " << endl;
      cout << "---------------------- " << endl;*/
      //G.destroyGraph();
      //G.testEdges();
      //G.displayAll();

      Graph G4;
      G4.buildGraph(infile1);
      if (infile1.eof())
         break;

      G4.findShortestPathHeap();
      G4.displayAll();
   }
}


//-------------------------- main -------------------------------------------
// Tests the Graph class by reading data from "HW3.txt" 
// Preconditions:   If lab3.txt file exists, it must be formatted
//                  as described in the lab specifications.
// Postconditions:  The basic functionalities of the Graph class 
//                  are used.  Should compile, run to completion, and output  
//                  correct answers if the classes are implemented correctly.
int main() {

   testGraph();

   _CrtDumpMemoryLeaks();
   

   

   return 0;
}