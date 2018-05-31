#include "Vertex.h"

Vertex::Vertex()
: _vertexWeight{1}
, _numDegree{0}
, _color{0}
// , _label{0}
{}


void
Vertex::
setWeight(int w){
	_vertexWeight = w ; 
}

void 
Vertex::
setColor(int c ){
	_color = c ;
}


void 
Vertex::addEdge(int otherVerticeTag, int edgeWeight){
	if ( _adjacentLists2Weights.count(otherVerticeTag) == 0 )
	{
		_adjacentLists2Weights[otherVerticeTag] = edgeWeight ; 

		_numDegree ++ ;
	}
}



int 
Vertex::
getColor(){
	return _color;
}


int 
Vertex::
getWeight(){
	return _vertexWeight;
}



int
Vertex::
getDegree(){
	return _numDegree ; 
}


vector<int>  
Vertex::
getAdjacency(){
	vector<int> adjacentVertices( this->_numDegree ) ; 
	int id = 0; 
	for (auto const& item: _adjacentLists2Weights)
	{
		adjacentVertices[id++] = item.first ; 
	}
	return adjacentVertices ; 
}



vector<int>  
Vertex::
getAdjacentEdgeWeights(){
	vector<int> adjacentEdgeWeights( this->_numDegree ) ; 
	int id = 0; 
	for (auto const& item: _adjacentLists2Weights)
	{
		adjacentEdgeWeights[id++] = item.second ; 
	}
	return adjacentEdgeWeights ; 
}


