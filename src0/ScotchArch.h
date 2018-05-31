#include "ScotchGraph.h"
#include <vector>
#include <iostream>

#ifndef SCOTCHARCH_H
#define SCOTCHARCH_H



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
class ScotchArch
{

public:
	ScotchArch();
	~ScotchArch();
	int createArch() ; 

	int setBasicArch(int partitionNbr) ; 

	SCOTCH_Arch* getArchPtr() ;
private:
	SCOTCH_Arch* _arch ; 

};

} // namespace Scotch

#endif // SCOTCHARCH_H

