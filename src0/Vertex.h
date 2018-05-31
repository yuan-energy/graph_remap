
#ifndef SCOTCH_VERTEX_H
#define SCOTCH_VERTEX_H


#include <vector>
#include <unordered_map>
#include "ScotchUtils.h"

using std::vector ;
using std::unordered_map ;

class Vertex
{
public:

	Vertex();

	void setWeight(int w) ;
	void setColor(int c) ;
	void addEdge(int otherVerticeTag, int edgeWeight=1) ; 


	int getColor() ;
	int getWeight() ;
	int getDegree();
	vector<int> getAdjacentEdgeWeights();
	vector<int> getAdjacency() ; 


private:
	unordered_map<int,int> _adjacentLists2Weights;

	int _vertexWeight ; 

	int _numDegree ;
	int _color ; 

	// int _label ; 
};


#endif