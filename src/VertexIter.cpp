///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              Graph.cpp
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

#include "VertexIter.h"
#include <Graph.h>
#include <Vertex.h>

VertexIter::VertexIter()
    : myVertices(0)
{

}

VertexIter::VertexIter(VertexVector& vertices)
{
    myVertices = &vertices;
    myIter = myVertices->begin();
    myIterEnd = myVertices->end();
}


VertexIter::~VertexIter()
{
}

void
VertexIter::reset(void)
{
    if (myVertices != 0)
    {
        myIter = myVertices->begin();
        myIterEnd = myVertices->end();
    }
}



Vertex*
VertexIter::operator()(void)
{
    if (myVertices != 0)
    {
        // check if we still have elements in the model
        // if not return 0, indicating we are done
        if (myIter != myVertices->end())
        {
            while (myIter->getTag() < 0) //Skip negative tags. Those are not there.
            {
                ++myIter;
                if (myIter == myVertices->end())
                {
                    return 0;
                }
            }
            Vertex* returnValue = &(*myIter);  //The return pointer is the current iterator value.
            myIter++;  // Prepare for the next one
            return returnValue;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}




