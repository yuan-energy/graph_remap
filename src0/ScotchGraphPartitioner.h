#ifndef SCOTCHGRAPHPARTITIONER_H
#define SCOTCHGRAPHPARTITIONER_H


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

class ScotchGraphPartitioner
{
public:

	// ScotchGraphPartitioner();

	// ~ScotchGraphPartitioner();

	bool partition(SCOTCH_Graph *graph, SCOTCH_Arch *arch, SCOTCH_Strat *strat, SCOTCH_Num *parttab);
	
};



} // namespace Scotch


#endif // SCOTCHGRAPHPARTITIONER_H

