
#include "ScotchArch.h"
namespace Scotch{
ScotchArch::
ScotchArch()
// : ScotchGraph()
{}


ScotchArch::
~ScotchArch()
{
	if(_arch) {
		// SCOTCH_archExit(_arch); // enable this line will cause random segmentation fault.
		// delete _arch;
	}
}


int 
ScotchArch::
createArch(){
	_arch = SCOTCH_archAlloc();
	if(SCOTCH_archInit(_arch) == SCOTCH_SUCCESS) {
		// successfully initialized architecture
		return SCOTCH_SUCCESS;
	}
	return SCOTCH_FAILED;
}





int 
ScotchArch::
setBasicArch(int partitionNbr){
	if ( SCOTCH_archCmplt( _arch , (SCOTCH_Num)partitionNbr) != SCOTCH_SUCCESS) {
		cerr << " ScotchArch::setBasicArch SCOTCH_archCmplt failed! " << endl;
		return SCOTCH_FAILED ;
	}
	cerr << " ScotchArch::setBasicArch SCOTCH_archCmplt succeed! " << endl;
	return SCOTCH_SUCCESS ; 
}




SCOTCH_Arch* 
ScotchArch::
getArchPtr(){
	return _arch ; 
}


} // namespace Scotch