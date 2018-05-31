
#ifndef ScotchGraph_h
#define ScotchGraph_h

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "Vertex.h"
#include "scotch.h"
#include "ScotchUtils.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::unordered_map;
using std::string;
using std::ifstream;

namespace Scotch{
class ScotchGraph
{
public:

    // ScotchGraph();

    // int addVertice( Vertex const& vertex_ );
    // int addEdge(pair<int,int> edge, int edgeWeight=1);

    int createGraph();
    int checkGraph();
    int buildGraph();

    int addGraphFromFile(string filename);

    int addTestGraph();
    // int deleteTestGraph();

    int buildGraphInScotchFormat();

    SCOTCH_Graph* getGraphPtr();

    virtual ~ScotchGraph();

    int getNumVertices();

protected:


private:

	unordered_map<int, Vertex> _vertices ; 


	// SCOTCH_Manual: Page 85. 
    SCOTCH_Num _numEdges;    // 
    SCOTCH_Num _numVertices; // numVertices
    SCOTCH_Num _vertnbr;     // numVertices
    SCOTCH_Num _edgenbr;     // numArcs = 2 * _numEdges, 
    SCOTCH_Num _baseval ;    // baseValue

    SCOTCH_Num* _verttab ;   // verticeAdjacentIndexArray
    SCOTCH_Num* _vendtab ;   // verticeAdjacentEndIndexArray

    SCOTCH_Num* _velotab ;   // verticeWeightArray
    SCOTCH_Num* _vlbltab ;   // verticeLabelArray
    SCOTCH_Num* _edgetab ;   // verticeAdjacentArray

    SCOTCH_Num* _edlotab ;   // ArcWeightArray

    SCOTCH_Graph* _graph ; 

    
};
} // namespace Scotch
#endif

