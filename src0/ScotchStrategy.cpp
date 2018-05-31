#include "ScotchStrategy.h"
namespace Scotch
{

int
ScotchStrategy::
createStrategy(int partition_number){
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

		this->buildStrategy(partition_number) ;

		return SCOTCH_SUCCESS;
	}

	cerr << "ScotchStrategy::createStrategy() failed " << endl;

	return SCOTCH_FAILED;
}

int
ScotchStrategy::
buildStrategy(int partition_number){
	// populate strategy parameters for graphMap
	const SCOTCH_Num partitionNbr = (SCOTCH_Num) partition_number;   // number of partitions/k clusters
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

SCOTCH_Strat *
ScotchStrategy::
getStrategyPtr(){
	return _strat ; 
}



} // namespace Scotch