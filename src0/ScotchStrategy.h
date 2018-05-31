#ifndef SCOTCHSTRATEGY_H
#define SCOTCHSTRATEGY_H

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

namespace Scotch
{
	

class ScotchStrategy
{
public:
	// ScotchStrategy();
	// ~ScotchStrategy();
	
	int createStrategy(int TEST_PARTITION_NUMBER) ;
	int buildStrategy(int TEST_PARTITION_NUMBER) ;
	SCOTCH_Strat * getStrategyPtr() ;

private:
	SCOTCH_Strat * 	_strat ; 

};


}


#endif // SCOTCHSTRATEGY_H