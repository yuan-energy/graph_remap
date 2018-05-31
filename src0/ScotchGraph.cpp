#include "ScotchGraph.h"
namespace Scotch{

// ScotchGraph::ScotchGraph()
// // : 
// {

// }

ScotchGraph::
~ScotchGraph(){
	if(_graph) {
	  SCOTCH_graphExit(_graph);
	  // delete _graph;
	}

	// if( this->_verttab ){ delete [] this->_verttab ; }
	// if( this->_velotab ){ delete [] this->_velotab ; }
	// if( this->_edgetab ){ delete [] this->_edgetab ; }
	// if( this->_edlotab ){ delete [] this->_edlotab ; }
	// if( v1 ){ delete v1; }
	// if( v2 ){ delete v2; }
	// if( v3 ){ delete v3; }
	// if( v4 ){ delete v4; }
	// if( v5 ){ delete v5; }
	// if( v6 ){ delete v6; }
	cerr << " SCOTCH_graphBuild delete memory success ! " << endl ; 

}

int ScotchGraph::createGraph()
{
	// Create
	cerr <<  "ScotchGraph::createGraph start " << endl;
	_graph = SCOTCH_graphAlloc();
	if ( SCOTCH_SUCCESS != SCOTCH_graphInit(_graph) )
	{
		cerr << " ERROR SCOTCH_graphInit failed ! " << endl; 
		return SCOTCH_FAILED ; 
	}
	cerr <<  "ScotchGraph::createGraph end " << endl;
	return SCOTCH_SUCCESS ; 
}

// ScotchGraph::ScotchGraph(int numVertice)
// : _verticesAdjacLists(numVertice)
// , _verticeHasWeight{false}
// , _edgeHasWeight{false}
// , _numVertices{numVertice}
// , _numEdges{0}
// {}




// int 
// ScotchGraph::addVertice( Vertex const& vertex_ )
// {
// 	_vertices.insert(vertex_) ;
// 	return 1 ; 
// }


// int 
// ScotchGraph::addEdge(pair<int,int> edge, int edgeWeight){
// 	auto firstVertex = _vertices[edge.first] ; 
// 	auto firstVertex = _vertices[edge.first] ; 

// }

int 
ScotchGraph::
addGraphFromFile(string filename){
	// ifstream file;
	// file.open(filename, std::ifstream::in) ; 
	// string line 
	// while( getline(file, line) ){


	// }

	return SCOTCH_SUCCESS ; 
}

int 
ScotchGraph::
addTestGraph(){
	auto v1 = new Vertex(); v1->addEdge(1); v1->addEdge(2);
	auto v2 = new Vertex(); v2->addEdge(0); v2->addEdge(3); v2->addEdge(4);
	auto v3 = new Vertex(); v3->addEdge(0); v3->addEdge(3); v3->addEdge(4);
	auto v4 = new Vertex(); v4->addEdge(1); v4->addEdge(2);
	auto v5 = new Vertex(); v5->addEdge(1); v5->addEdge(2); v5->addEdge(5);
	auto v6 = new Vertex(); v6->addEdge(4);

	_vertices[0] = *v1 ;
	_vertices[1] = *v2 ;
	_vertices[2] = *v3 ;
	_vertices[3] = *v4 ;
	_vertices[4] = *v5 ;
	_vertices[5] = *v6 ;


	this->_baseval = 0 ; 
	this->_numEdges = 7 ;
	this->_numVertices = 6 ;
	this->_vertnbr = this->_numVertices ;
	this->_edgenbr = this->_numEdges * 2 ;
	

	return SCOTCH_SUCCESS ; 
}

// int ScotchGraph::
// deleteTestGraph(){
	// delete v1;
	// delete v2;
	// delete v3;
	// delete v4;
	// delete v5;
	// delete v6;
	// return 1 ; 
// }

int 
ScotchGraph::
buildGraphInScotchFormat()
{

	this->_verttab = new int[this->_numVertices + 1 ];
	this->_velotab = new int[this->_numVertices  ];

	this->_edgetab = new int[this->_numEdges * 2 ];
	this->_edlotab = new int[this->_numEdges * 2 ];
	int vTabID = 0 ; 
	int id = 0 ; 
	for (auto it = _vertices.begin(); it != _vertices.end(); it++ )
	{
		auto ver = it->second ; 

		this->_verttab[id] = vTabID ; 
		this->_velotab[id] = ver.getWeight() ; 

		int count = 0 ; 
		int numDegree = ver.getDegree();
		auto AdjacentEdgeWeights =  ver.getAdjacentEdgeWeights();
		auto Adjacency =  ver.getAdjacency();
		for (int i = 0 ; i < numDegree ; i++ )
		{
			this->_edgetab[ vTabID + count ] = Adjacency[i] ; 
			this->_edlotab[ vTabID + count ] = AdjacentEdgeWeights[i] ; 

			count++ ; 
		}

		vTabID += numDegree ; 
		id++;

	}
	this->_verttab[id] = vTabID ; 


	this->createGraph();


	cerr << " print _edgetab " << endl;
	for(int i = 0; i < this->_numEdges * 2 ; i++) {
	  std::cout << this->_edgetab[i] << ", ";
	}
	cerr << endl << endl ;

	cerr << " _baseval = " << _baseval << endl ;
	cerr << " _vertnbr = " << _vertnbr << endl ;
	cerr << " _edgenbr = " << _edgenbr << endl ;

	cerr << " print _edlotab " << endl;
	for(int i = 0; i < this->_numEdges * 2 ; i++) {
	  cerr << _edlotab[i] << ", " ;
	}
	cerr << endl << endl ;

	cerr << " print _verttab " << endl;
	for(int i = 0; i < this->_numVertices+1 ; i++) {
	  cerr << _verttab[i] << ", " ;
	}
	cerr << endl << endl ;
	
	cerr << " print _velotab " << endl;
	for(int i = 0; i < this->_numVertices ; i++) {
	  cerr << _velotab[i] << ", " ;
	}
	cerr << endl << endl ;

	
	// Build
	int status = SCOTCH_graphBuild(
					_graph, 
					this->_baseval, 
					this->_vertnbr, 
					this->_verttab, 
					0, 
					this->_velotab, 
					0, 
					this->_edgenbr, 
					this->_edgetab, 
					this->_edlotab
				) ; 
	if( status != SCOTCH_SUCCESS )
	{
		cerr << " ERROR SCOTCH_graphBuild failed ! " << endl ; 
		return SCOTCH_FAILED ; 
	}

	cerr << " SCOTCH_graphBuild build success ! " << endl ; 

	return SCOTCH_SUCCESS ; 
}




SCOTCH_Graph* 
ScotchGraph::
getGraphPtr(){
	return _graph ; 
}



int 
ScotchGraph::
getNumVertices(){
	return (int)_numVertices ; 
}

} // namespace Scotch