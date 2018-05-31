///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              Graph.h
// CLASS:             Graph
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Jose Abell, Boris Jeremic
// PROGRAMMER:        Jose Abell
// DATE:              July 2015
// UPDATE HISTORY:    Reimplements Vertex iterator class using std::vector<Vertex> as underlying container for
//                    increased efficiency. Legacy VertexIter is renamed to OldVertexIter and not compiled.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef VertexIter_h
#define VertexIter_h

#include <vector>

class Vertex;
class TaggedObjectStorage;
class TaggedObjectIter;

typedef std::vector<Vertex> VertexVector;


class VertexIter
{
public:
	VertexIter();
	VertexIter(VertexVector& vertices);
	virtual ~VertexIter();

	virtual void reset(void);
	virtual Vertex* operator()(void);

protected:

private:
	VertexVector::iterator myIter;
	VertexVector::iterator myIterEnd;
	VertexVector* myVertices;
};

#endif

