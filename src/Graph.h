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
// UPDATE HISTORY:    Reimplements Graph class using std::vector<Vertex> as underlying container for
//                    increased efficiency. Legacy Graph is renamed to OldGraph and not compiled.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef Graph_h
#define Graph_h

#include <vector>
#include <iostream>
#include <Vertex.h>
#include <VertexIter.h>
using namespace std;

// class Vertex;
// class VertexIter;
// class Channel;
// class FEM_ObjectBroker;

#define START_VERTEX_NUM 0 

class Graph
{
public:
    Graph();
    Graph(int numVertices);
    Graph(VertexVector &otherVertices);
    Graph(Graph &other);
    virtual ~Graph();

    virtual bool addVertex(Vertex *vertexPtr, bool checkAdjacency = true);
    virtual int addEdge(int vertexTag, int otherVertexTag);

    virtual Vertex *getVertexPtr(int vertexTag);
    virtual VertexIter &getVertices(void);
    virtual int getNumVertex(void) const;
    virtual int getNumEdge(void) const;
    virtual int getFreeTag(void);
    virtual Vertex *removeVertex(int tag, bool removeEdgeFlag = true);

    virtual int merge(Graph &other);

    virtual void Print(ostream &s, int flag = 0);
    // int sendSelf(int commitTag, Channel &theChannel);
    // int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    friend ostream &operator<<(ostream &s, Graph &M);

protected:

private:
    VertexVector myVertices;
    VertexIter theVertexIter;
    int numVertices;
    int numEdges;
    int nextFreeTag;
};

#endif

