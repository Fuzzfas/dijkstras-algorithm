/**@file Vertex.cpp*/

/*
Tin Khuong
	Student ID: 0421141
	CSS 502B
	Program 3
	Prof. Clark Olson
	2/11/20
*/

/*
	This file contains all of the definitions for the Vertex class.
	This is a simple class that stores the description of vertices in the form of a string. 
	The VertexNode struct of the Graph class will contain a pointer to a Vertex object.

	Assumptions:
	A Graph object must exist in order to instantiate objects of this class.

*/

#include "Vertex.h"

/*-------------------------------- Default Constructor ---------------------------------
@pre A Graph object exists, constructing a VertexNode object, which in turn will construct a Vertex object.
@post A Vertex object is created with name initialized as an empty string
*/
Vertex::Vertex()
{
}

/*-------------------------------- Copy Constructor ---------------------------------
@pre Another Vertex object exists to be copied
@post Copies name of other Vertex object
*/
Vertex::Vertex(const Vertex& other)
{
	name = other.name;
}

/*-------------------------------- Destructor ---------------------------------
@pre A Vertext object exists
@post Destroys existing Vertext object
*/
Vertex::~Vertex()
{
}

/*-------------------------------- operator>> ---------------------------------
Uses ifstream as input for Vertex object name
@pre: infile has been successfully opened, ifstream object created, and the file contains
	properly formated data (according to the program specs). Graph object must also exist to construct Vertex object.
	Called by buildGraph of the Graph class.
@post: Descrptions read from ifstream object and stored in the Vertex object
*/
istream& operator>>(istream& input, Vertex& v)
{
	string str = "";
	getline(input, str, '\n');
	v.name = str;

	return input;
}

/*-------------------------------- operator<< ---------------------------------
Outputs Vertex object name
@pre: Assumes operator>> was called, otherwise will output nothing.
@post: Outputs descrptions stored in the Vertex object
*/
ostream& operator<<(ostream& output, Vertex& v)
{
	output << v.name;

	return output;
}