#include "ScotchGraphPartitioner.h"
namespace Scotch{
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
} // namespace Scotch
