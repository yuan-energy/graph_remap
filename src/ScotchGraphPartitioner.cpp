#ifdef _USE_LIBSCOTCH

#include "ScotchGraphPartitioner.h"
using namespace std; 

namespace Scotch{

ScotchGraphPartitioner::
ScotchGraphPartitioner(int numParts)
{}



ScotchGraphPartitioner::
~ScotchGraphPartitioner()
{}


int 
ScotchGraphPartitioner::
partition( Graph* theGraph, int numParts )
{
	cerr << " ScotchGraphPartitioner::partition " << endl; 

	int NUMProcessors = 4 ;
	int PID = 0 ;
	// int NUMProcessors ;
	// int PID ;
	// MPI_Comm_size(MPI_COMM_WORLD, &NUMProcessors);
	// MPI_Comm_rank(MPI_COMM_WORLD, &PID);

	if (PID == 0)
	{

		this->createGraph() ;
		this->buildScotchGraph(theGraph) ;
		
		this->createArch() ;
		this->buildArch(NUMProcessors) ;
		
		this->createStrategy() ;
		this->buildStrategy(NUMProcessors) ;

		cerr << "--> ScotchGraphPartitioner::partition this->_numVertices = " << this->_numVertices << endl; 
		SCOTCH_Num * parttab = new SCOTCH_Num[this->_numVertices];

		this->partition(this->_graph, this->_arch, this->_strat, parttab);

		cerr << " Print results: " << endl ;
		for (int i = 0; i < this->_numVertices; ++i)
		{
			cerr << parttab[i] << " " ; 
		}
		cerr << endl; 

		// delete []parttab ; 

		// we set the vertex colors to correspond to the partitioned scheme
		Vertex* vertexPtr;  
		for (int vert = 0; vert < this->_numVertices ; vert++){
		    vertexPtr = theGraph->getVertexPtr( vert );
		    vertexPtr->setColor(parttab[vert] + 1); // start colors at 1
		}

		// save old partition for remapping.
		_parotab = new SCOTCH_Num[_numVertices] ; 
		_vmlotab = new SCOTCH_Num[_numVertices] ; 
		for (int i = 0; i < this->_numVertices; ++i) {
			_parotab[i] = parttab[i] ; 
			_vmlotab[i] = 1.0 ; 
		}
		// clean up the space and return
		// delete [] parttab ;



		// //Store for use with ParMETIS
		// gnvtxs = numVertex_METIS;
		// gnedge = numEdge_METIS;
		// globalXadj = xadj_METIS;
		// globalAdjncy = adjncy_METIS;

		// //Now share this info with everyone!!!
		// MPI_Bcast(&gnvtxs, 1 , MPI_INT, 0, MPI_COMM_WORLD);
		// MPI_Bcast(&gnedge, 1 , MPI_INT, 0, MPI_COMM_WORLD);
		// MPI_Bcast((void *)globalXadj, gnvtxs + 2, MPI_INT, 0, MPI_COMM_WORLD);
		// MPI_Bcast((void *)globalAdjncy, 2 * gnedge, MPI_INT, 0, MPI_COMM_WORLD);

		return SCOTCH_SUCCESS ; 
	} else {
	// PID != 0 


	}
	return SCOTCH_SUCCESS ; 
}

bool 
ScotchGraphPartitioner::
partition(SCOTCH_Graph *graph, SCOTCH_Arch *arch, SCOTCH_Strat *strat, SCOTCH_Num* parttab){

	if(graph == 0) {
		std::cout << "Graph Pointer error" << std::endl;
	}
	if(arch == 0) {
		std::cout << "Arch Pointer error" << std::endl;
	}
	if(strat == 0) {
		std::cout << "Strat Pointer error" << std::endl;
	}
	if(parttab == 0) {
		std::cout << "parttab Pointer error" << std::endl;
	}

	if(graph == 0 || arch == 0 || strat == 0 || parttab == 0) {
		std::cout << "Pointer error" << std::endl;
		return false;
	}
	cerr << " SCOTCH_graphMap start " << endl; 
	int success = SCOTCH_graphMap(graph, arch, strat, parttab);
	cerr << " SCOTCH_graphMap end " << endl; 

	if(success == 0) {
		return true;
	}
	return false;

}






int 
ScotchGraphPartitioner::
repartition( Graph* graph, int numParts ) {
	this->createGraph() ;
	this->buildScotchGraph(graph) ;


	SCOTCH_Num * parttab = new SCOTCH_Num[this->_numVertices];

	const double emraval = 1.0 ; 

	int success = this->repartition(
			_graph, 
			_arch, 
				_parotab,
				emraval,
				_vmlotab,
			_strat, 
			parttab
		);

	cerr << " repartition Print results: " << endl ;
	for (int i = 0; i < this->_numVertices; ++i)
	{
		cerr << parttab[i] << " " ; 
	}
	cerr << endl; 

	return success ;
}


bool 
ScotchGraphPartitioner::
repartition(SCOTCH_Graph *graph, SCOTCH_Arch *arch, 
	SCOTCH_Num* parotab,
	const double emraval,
	SCOTCH_Num* vmlotab,
	SCOTCH_Strat *strat, SCOTCH_Num* parttab) 
{
	if(graph == 0) {
		std::cout << "Graph Pointer error" << std::endl;
	}
	if(arch == 0) {
		std::cout << "Arch Pointer error" << std::endl;
	}
	if(strat == 0) {
		std::cout << "Strat Pointer error" << std::endl;
	}
	if(parttab == 0) {
		std::cout << "parttab Pointer error" << std::endl;
	}
	if(parotab == 0 ){
		std::cout << "parotab Pointer error" << std::endl;
	}
	if (emraval == 0 ){
		std::cout << " emraval error " << std::endl ; 
	}
	if ( vmlotab == 0 )	{
		std::cout << " vmlotab Pointer error " << std::endl ; 
	}
	if(graph == 0 || arch == 0 || strat == 0 || parttab == 0 || parotab==0 || emraval ==0 || vmlotab==0 ) {
		std::cout << "Pointer error" << std::endl;
		return false;
	}
	int success = SCOTCH_graphRemap(
			graph, 
			arch, 
				parotab,
				emraval,
				vmlotab,
			strat, 
			parttab
		);

	// int SCOTCH_graphRemap (SCOTCH_Graph * const, const SCOTCH_Arch * const, SCOTCH_Num *, const double, const SCOTCH_Num *, SCOTCH_Strat * const, SCOTCH_Num * const);

	if(success == 0) {
		return true;
	}
	return false;

}




int 
ScotchGraphPartitioner::
createGraph(){
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








int
ScotchGraphPartitioner::
buildScotchGraph(Graph *theGraph)
{
	cerr << " debug 1 buildScotchGraph() " << endl ;

	this->_numEdges = (SCOTCH_Num) theGraph->getNumEdge() ; 
	this->_numVertices = (SCOTCH_Num) theGraph->getNumVertex() ; 
	this->_vertnbr = this->_numVertices ; 
	this->_edgenbr = this->_numEdges * 2 ; 


	this->_verttab = new int[this->_numVertices + 1 ];
	this->_velotab = new int[this->_numVertices  ];

	this->_edgetab = new int[this->_numEdges * 2 ];
	this->_edlotab = new int[this->_numEdges * 2 ];
	int vTabID = 0 ; 
	int id = 0 ; 

	Vertex *vertexPtr ;

	for (int vert = 0; vert < this->_numVertices; vert++)
	{
	    vertexPtr = theGraph->getVertexPtr(vert + START_VERTEX_NUM);

	    if (vertexPtr == 0)
	    {
	        cerr << " WARNING ScotchGraphPartitioner::buildScotchGraph - No mapping done \n";
	        cerr << " ScotchGraph requires consecutive Vertex Numbering\n ";

	        delete [] _verttab;
	        delete [] _velotab;
	        delete [] _edgetab;
	        delete [] _edlotab;

	        return -2;
	    }

	    this->_verttab[id] = vTabID ; 
	    this->_velotab[id] = vertexPtr->getWeight() ;

	    // TODO: add edge-weights
	    // auto AdjacentEdgeWeights =  ver.getAdjacentEdgeWeights(); 

	    int numDegree = vertexPtr->getDegree() ; 
	    auto adjacency = vertexPtr->getAdjacency() ;

	    for (int i = 0; i < numDegree; ++i){
	    	this->_edgetab[ vTabID + i ] = adjacency[i] ; 
	    	this->_edlotab[ vTabID + i ] = 1 ; // TODO: AdjacentEdgeWeights[i] ; 
	    }

	    vTabID += numDegree ; 
	    id++ ; 
	}
	this->_verttab[id] = vTabID ; 



	// printing

	cerr << " print _edgetab " << endl;
	for(int i = 0; i < this->_numEdges * 2 ; i++) {
	  std::cout << this->_edgetab[i] << ", ";
	}
	cerr << endl << endl ;
	_baseval = 0 ; 
	
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

	// printing end


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
		cerr << " ERROR buildScotchGraph SCOTCH_graphBuild failed ! " << endl ; 
		return SCOTCH_FAILED ; 
	}

	cerr << " buildScotchGraph build SCOTCH_graphBuild success ! " << endl ; 

	return SCOTCH_SUCCESS ; 
}
















int
ScotchGraphPartitioner::
createArch(){
	cerr << " debug 2 createArch() " << endl ;
	
	_arch = SCOTCH_archAlloc();
	if( SCOTCH_archInit(_arch) != SCOTCH_SUCCESS ) {
		cerr << " createArch() failed ! " << endl ; 
		return SCOTCH_FAILED;
	}

	cerr << " createArch() Done ! " << endl ; 
	return SCOTCH_SUCCESS ; 
}

int
ScotchGraphPartitioner::
buildArch(int Nproc){
	cerr << " debug 3 buildArch() " << endl ;

	int partitionNbr = Nproc - 1 ; 

	if ( SCOTCH_archCmplt( _arch , (SCOTCH_Num)partitionNbr ) != SCOTCH_SUCCESS ) /*may cause memory error, affect the private variables in another class*/ 
	{
		cerr << " ScotchArch::buildArch SCOTCH_archCmplt failed! " << endl;
		return SCOTCH_FAILED ;
	}

	cerr << " ScotchArch::setBasicArch SCOTCH_archCmplt succeed! " << endl << endl;
	return SCOTCH_SUCCESS ; 
}

int
ScotchGraphPartitioner::
createStrategy(){
	cerr << " debug 4 createStrategy() " << endl ;

	_strat = SCOTCH_stratAlloc();

	if ( SCOTCH_stratInit(_strat) == SCOTCH_SUCCESS )
	{
		// switch( _strat ){
			// 
		// }

		// std::string strategyStr = "f";
		// if( SCOTCH_stratGraphMap( _strat, strategyStr.c_str() )  == 0 ){
			// cerr << "ScotchStrategy::createStrategy() succeed " << endl;
			// return 0 ; 
		// }

		cerr << "ScotchStrategy::createStrategy() SCOTCH_stratGraphMap default " << endl;


		return SCOTCH_SUCCESS;
	}


	cerr << "ScotchStrategy::createStrategy() failed " << endl;

	return SCOTCH_FAILED;

}

int
ScotchGraphPartitioner::
buildStrategy(int Nproc){
	cerr << " debug 5 buildStrategy() " << endl ;

	// populate strategy parameters for graphMap
	const SCOTCH_Num partitionNbr = (SCOTCH_Num) (Nproc-1) ;   // number of partitions/k clusters
	SCOTCH_Num straval = (SCOTCH_Num) 0;                             // default strategy value: see pg. 60 SCOTCH for other flag values
	double kbalval = 0.1;                                          // default imbalance tolerance value

	cerr << " SCOTCH_stratGraphMapBuild begin " << endl; 
	if( SCOTCH_stratGraphMapBuild(_strat, straval, partitionNbr, kbalval) != SCOTCH_SUCCESS ) {
		cerr << "Could not create strategy for Graph Map Build" << endl ; 
		return SCOTCH_FAILED ; 
	}
	cerr << " SCOTCH_stratGraphMapBuild good " << endl; 

	// const char * straptr = 0; 
	// if(straptr != 0) {
	// 	// use the strategy flags, on the strategy
	// 	SCOTCH_stratGraphMap(_strat, straptr);
	// }

	return SCOTCH_SUCCESS ; 

}



// for test:
int 
ScotchGraphPartitioner::
buildGraphInScotchFormat(){


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
		this->_velotab[id] = 1; // ver.getWeight() ; // TODO: vertex weight

		int count = 0 ; 
		int numDegree = ver.getDegree();
		// auto AdjacentEdgeWeights =  ver.getAdjacentEdgeWeights(); // TODO: add edge-weights
		auto Adjacency =  ver.getAdjacency();
		for (int i = 0 ; i < numDegree ; i++ )
		{
			this->_edgetab[ vTabID + count ] = Adjacency[i] ; 
			this->_edlotab[ vTabID + count ] = 1 ; // TODO: AdjacentEdgeWeights[i] ; 

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

	
	cerr << "buildGraphInScotchFormat() this->_numVertices  = " << this->_numVertices   << endl ;
	
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

	cerr << "buildGraphInScotchFormat() this->_numVertices  = " << this->_numVertices   << endl ;
	
	if( status != SCOTCH_SUCCESS )
	{
		cerr << " ERROR SCOTCH_graphBuild failed ! " << endl ; 
		return SCOTCH_FAILED ; 
	}

	cerr << " SCOTCH_graphBuild build success ! " << endl ; 

	return SCOTCH_SUCCESS ; 

}


int 
ScotchGraphPartitioner::
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





SCOTCH_Graph* 
ScotchGraphPartitioner::
getGraphPtr(){
	return _graph ; 
}



int 
ScotchGraphPartitioner::
getNumVertices(){
	return (int)(this->_numVertices) ; 
}



// int
// ScotchGraphPartitioner::
// sendSelf(int cTag, Channel &theChannel){
// 	return 0 ; 
// }

// int
// ScotchGraphPartitioner::
// receiveSelf(int cTag, Channel &theChannel, FEM_ObjectBroker &theBroker){
//     return 0;
// }




} // namespace Scotch


#endif
