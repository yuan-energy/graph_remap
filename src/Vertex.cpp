#include "Vertex.h"

Vertex::Vertex()
: _vertexWeight{1}
, _numDegree{0}
, _color{0}
, _tag{0}
, _ref{0}
{}

Vertex::Vertex(int tag_)
: _vertexWeight{1}
, _numDegree{0}
, _color{0}
, _tag{tag_}
, _ref{tag_}
{}

Vertex::Vertex(int tag_, int ref_)
: _vertexWeight{1}
, _numDegree{0}
, _color{0}
, _tag{tag_}
, _ref{ref_}
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


int 
Vertex::addEdge(int otherVerticeTag, int edgeWeight){
	if ( _adjacentLists2Weights.count(otherVerticeTag) == 0 )
	{
		_adjacentLists2Weights[otherVerticeTag] = edgeWeight ; 

		_numDegree ++ ;
		return 0 ;
	}

	return 1 ; 
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

int
Vertex::
getTag(){
	return _tag ; 
}

int 
Vertex::
getRef(void){
	return _ref ; 
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


