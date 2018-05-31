#ifdef _USE_LIBSCOTCH


#ifndef ScotchGraphPartitioner_h
#define ScotchGraphPartitioner_h

// #include <GraphPartitioner.h>
// #include <GraphNumberer.h>

// #include <Graph.h>
// #include <Vertex.h>
// #include <VertexIter.h>
#include <cstdlib>
// #include <mpi.h>
// #include <bool.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "Vertex.h"
#include "scotch.h"
#include "ScotchUtils.h"
#include "Graph.h"


// #ifndef _bool_h
// #include <bool.h>
// #endif
using namespace std; 
namespace Scotch{

class ScotchGraphPartitioner 
// class ScotchGraphPartitioner : public GraphPartitioner
//, public GraphNumberer
{
public:
    ScotchGraphPartitioner(int numParts = 1);
     ~ScotchGraphPartitioner();

    int partition( Graph* theGraph, int numParts ) ;

    int createGraph();
    int buildGraphInScotchFormat();
    int addTestGraph();

    int getNumVertices();

    SCOTCH_Graph* getGraphPtr();
	bool partition(SCOTCH_Graph *graph, SCOTCH_Arch *arch, SCOTCH_Strat *strat, SCOTCH_Num *parttab);
	

    int buildScotchGraph(Graph* theGraph);
    int createArch();
    int buildArch(int Nproc);
    int createStrategy();
    int buildStrategy(int Nproc);
    
    // int sendSelf(int commitTag,
    //              Channel& theChannel);
    // int receiveSelf(int commitTag, Channel& theChannel,
    //                 FEM_ObjectBroker& theBroker);



	// SCOTCH_Manual: Page 85. 
    SCOTCH_Num _numEdges;    // 
    SCOTCH_Num _numVertices; // numVertices
    SCOTCH_Num _vertnbr;     // numVertices
    SCOTCH_Num _edgenbr;     // numArcs = 2 * _numEdges, 
    SCOTCH_Num _baseval ;    // baseValue
private:





    unordered_map<int, Vertex> _vertices ; 


    SCOTCH_Num* _verttab ;   // verticeAdjacentIndexArray
    SCOTCH_Num* _vendtab ;   // verticeAdjacentEndIndexArray

    SCOTCH_Num* _velotab ;   // verticeWeightArray
    SCOTCH_Num* _vlbltab ;   // verticeLabelArray
    SCOTCH_Num* _edgetab ;   // verticeAdjacentArray

    SCOTCH_Num* _edlotab ;   // ArcWeightArray

    SCOTCH_Graph*   _graph ; 
    SCOTCH_Arch*    _arch ; 
    SCOTCH_Strat *  _strat ; 

    
};


} // namespace Scotch

#endif // ScotchGraphPartitioner_h


#endif // _USE_LIBSCOTCH