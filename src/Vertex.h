
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

	Vertex(int tag_);
	Vertex(int tag_, int ref_);

	void setWeight(int w) ;
	void setColor(int c) ;

	int addEdge(int otherVerticeTag, int edgeWeight=1) ; 

	int getRef(void) ;

	int getColor() ;
	int getWeight() ;
	int getDegree();

	int getTag();

	vector<int> getAdjacentEdgeWeights();
	vector<int> getAdjacency() ; 


private:
	unordered_map<int,int> _adjacentLists2Weights;

	int _vertexWeight ; 

	int _numDegree ;
	int _color ; 

	int _tag ; 

	int _ref ;
};


#endif