#include "ScotchGraphPartitioner.h"
// #include "ScotchGraph.h"
// #include "ScotchArch.h"
// #include "ScotchStrategy.h"

#include "Graph.h"


using namespace std ;

const int TEST_PARTITION_NUMBER = 2 ; 

void addTestGraph(Graph& graph){
	int numRow = 4 , numCol = 4 ; 
	int numOfVertices = numRow * numCol ; 

	std::vector<Vertex> vertices(numOfVertices) ; 
	for(int i = 0 ; i < numOfVertices ; i++ ){
		vertices[i] = Vertex(i,i);
		graph.addVertex( &vertices[i]) ; 
	}


	for(int i = 0 ; i < numCol-1 ; i++ ){
		graph.addEdge(i, i + 1 ) ; 
		graph.addEdge(numOfVertices-i-1, numOfVertices-i-2) ; 
	}
	for (int i = 0; i < numRow-1 ; ++i)	{
		graph.addEdge(i, i + numCol ) ; 
		graph.addEdge(numOfVertices-1-i*numCol, numOfVertices-1-(i+1)*numCol ) ; 
	}

	for (int i = 1; i < numCol-1; ++i){
		for (int j = 1; j < numRow-1; ++j){
			int id = i * numCol + j ; 
			graph.addEdge(id, id + 1 );
			graph.addEdge(id, id - 1 );
			graph.addEdge(id, id - numCol );
			graph.addEdge(id, id + numCol );
		}
	}
}

int main(int argc, char const *argv[])
{
	cerr << " main hello " << endl; 
	

	Graph graph ; 
	addTestGraph( graph ); 







	// // step 4: mapping
	int numVertices = graph.getNumVertex() ;
	// // int numVertices = 2 * graph.getNumVertices() + 10 ; 
	cout << " numVertices  = " << numVertices  << endl ;
	// SCOTCH_Num * parttab = new SCOTCH_Num[numVertices];
	auto partitioner = Scotch::ScotchGraphPartitioner() ; 
	partitioner.partition(
			&graph,
			TEST_PARTITION_NUMBER
		);
	// cerr << " Print results: " << endl ;
	// for (int i = 0; i < numVertices; ++i)
	// {
	// 	cerr << parttab[i] << " " ; 
	// }
	// cerr << endl; 

	// delete []parttab ; 

	// cerr << " main end " << endl; 
	return 0;
}

