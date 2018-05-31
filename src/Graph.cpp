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
// UPDATE HISTORY:    Reimplements Graph class using std::vector<Vertex> as underlying container for
//                    increased efficiency. Legacy Graph is renamed to OldGraph and not compiled.
//
///////////////////////////////////////////////////////////////////////////////

#include <Graph.h>
#include <Vertex.h>
#include <VertexIter.h>
// #include <ArrayOfTaggedObjects.h>
// #include <Channel.h>
// #include <FEM_ObjectBroker.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>


// #include <Vector.h>
// #include <ID.h>

using namespace std;

Graph::Graph()
    :  numVertices(0), numEdges(0), nextFreeTag(START_VERTEX_NUM)
{

}

//Allocates *space* for numVertices number of vertices. For the vertices to be valid
// they have to be added through the addVertex() function. This constructor just
// saves malloc calls.
Graph::Graph(int numVertices)
    : myVertices(numVertices), numVertices(0), numEdges(0), nextFreeTag(START_VERTEX_NUM)
{

}


Graph::Graph(VertexVector &theVerticesStorage)
    : myVertices(theVerticesStorage), numVertices(0), numEdges(0), nextFreeTag(START_VERTEX_NUM)
{
    for (auto it = theVerticesStorage.begin(); it != theVerticesStorage.end(); it++)
    {
        int tag = it->getTag();
        if (tag > nextFreeTag)
        {
            nextFreeTag = it->getTag() + 1;
        }
        if (tag > 0)
        {
            numVertices++;
        }
    }
    theVertexIter = VertexIter(myVertices);
}


Graph::Graph(Graph &other)
    : myVertices(other.myVertices), numVertices(other.numVertices), numEdges(other.numEdges), nextFreeTag(other.nextFreeTag)
{
    theVertexIter = VertexIter(myVertices);
}

Graph::~Graph()
{

    // if (myVertices != 0)
    // {
    //     delete myVertices;
    // }

    // if (theVertexIter != 0)
    // {
    //     delete theVertexIter;
    // }
}


// bool addVertex(int vertexTag, int vertexRef,
//       int vertexWeight=0, int vertexColor = 0)
// Method to add a vertex to the graph. If the adjacency list
// of the vertex is not empty the graph will first check to see all
// vertices in the the the vertices adjacency list exist in the graph
// before the vertex is added.

bool
Graph::addVertex(Vertex *vertexPtr, bool checkAdjacency)
{
    // check the vertex * and its adjacency list

    if (vertexPtr == 0)
    {
        cerr << "WARNING Graph::addVertex";
        cerr << " - attempting to add a NULL vertex*\n";
        return false;
    }

    if (checkAdjacency == true)
    {
        if (vertexPtr->getDegree() != 0)
        {
            const auto &adjacency = vertexPtr->getAdjacency();
            int size = adjacency.size();

            for (int i = 0; i < size; i++)
            {
                Vertex *other = this->getVertexPtr( adjacency[i] );

                if (other == 0)
                {
                    cerr << "WARNING Graph::addVertex";
                    cerr << " - vertex with adjacent vertex not in graph\n";
                    return false;
                }
            }
        }
    }


    int tag = vertexPtr->getTag();
    bool result = true;
    try
    {

        int maxtag = myVertices.size() - 1;
        if (tag  == maxtag + 1) // If its a consecutive tag, then using push_back might be the best option
        {
            myVertices.push_back(*vertexPtr);
        }
        else if (tag > maxtag + 1) // If its not, we allocate for twice the size. Amortizing allocation costs.
        {
            myVertices.resize(2 * tag);
            if (myVertices[tag].getTag() < 0) // Check if tag is available
            {
                myVertices[tag] = *vertexPtr;
            }
            else
            {
                std::cerr << "Graph::addVertex - vertex with tag. " << tag << " is already taken.\n";
                result = false;
            }
        }
        else // If we have the space, simply assign after checking if the spot is not taken.
        {
            if (myVertices[tag].getTag() < 0)
            {
                myVertices[tag] = *vertexPtr;
            }
            else
            {
                std::cerr << "Graph::addVertex - vertex with tag. " << tag << " is already taken.\n";
                result = false;
            }
        }
    }
    catch (std::bad_alloc& ba) // Catch the possibility that push_back (or resize) fails.
    {
        std::cerr << "Graph::addVertex - bad alloc caught. " << ba.what() << '\n';
        result = false;
    }

    if (result)
    {
        numVertices++;

        // check nextFreeTag
        if (tag >= nextFreeTag)
        {
            nextFreeTag = tag + 1;
        }
    }
    else
    {
        std::cerr << "Graph::addVertex - unable to add vertex. \n";
    }


    return result;
}


// int addEdge(int vertexTag, int otherVertexTag);
// Causes the Graph to add an edge {\em (vertexTag,otherVertexTag)} to
// the Graph. A check is first made to see if vertices with tags given by
// {\em vertexTag} and {\em otherVertexTag} exist in the graph. If they
// do not exist a $-1$ is returned, otherwise the method invokes {\em
// addEdge()} on each of the corresponding vertices in the
// graph. Returns $0$ if sucessfull, a negative number if not.

int
Graph::addEdge(int vertexTag, int otherVertexTag)
{
    // get pointers to the vertices, if one does not exist return

    Vertex *vertex1 = this->getVertexPtr(vertexTag);
    Vertex *vertex2 = this->getVertexPtr(otherVertexTag);

    if ((vertex1 == 0) || (vertex2 == 0))
    {
        cerr << "WARNING Graph::addEdge() - one or both of the vertices ";
        cerr << vertexTag << " " << otherVertexTag << " not in Graph\n";
        return -1;
    }

    // add an edge to each vertex
    int result;

    if ((result = vertex1->addEdge(otherVertexTag)) == 0)
    {
        if ((result = vertex2->addEdge(vertexTag)) == 0)
        {
            numEdges++;
        }
        else
        {
            cerr << "WARNING Graph::addEdge() - " << vertexTag;
            cerr << " has not been added to " << otherVertexTag;
            cerr << " adjacency - yet vice-versa ok.\n";
            return -2;
        }
    }

    return result;
}


Vertex *
Graph::getVertexPtr(int vertexTag)
{
    // TaggedObject *res = myVertices->getComponentPtr(vertexTag);

    if ((unsigned int) vertexTag < myVertices.size())
    {
        Vertex * theVertex;
        theVertex = &(myVertices[vertexTag]);

        if (theVertex->getTag() != vertexTag)
        {
            return 0;
        }

        return theVertex;
    }
    else
    {
        return 0;
    }
}


VertexIter &
Graph::getVertices(void)
{
    // reset the iter and then return it
    theVertexIter = VertexIter(myVertices);
    theVertexIter.reset();
    return theVertexIter;
}


int
Graph::getNumVertex(void) const
{
    return numVertices;
}

int
Graph::getNumEdge(void) const
{
    return numEdges;
}

int
Graph::getFreeTag(void)
{
    return nextFreeTag;
}



























Vertex *
Graph::removeVertex(int tag, bool flag)
{
    // Vertex *dumpMe = getVertexPtr(tag);


    // if (dumpMe == 0)
    // {
    //     // No vertex. Nothing do do.
    //     return 0;
    // }

    // //Simply set tag to -1 to indicate that is no longer there,
    // (*dumpMe) = Vertex(-1, -1);
    // numVertices--;

    // if (flag == true)   // remove all edges associated with the vertex
    // {
    //     cerr << "Graph::removeVertex(int tag, bool flag = true)";
    //     cerr << " - no code to remove edges yet\n";
    //     return 0;
    // }

    return 0;
}


int
Graph::merge(Graph &other)
{

    int result = 0;
    VertexIter &otherVertices = other.getVertices();
    Vertex *vertexPtrOther;

    // loop through other creating vertices if tag not the same in this
    while ((vertexPtrOther = otherVertices()) != 0)
    {
        int vertexTag = vertexPtrOther->getTag();
        Vertex *vertexPtr = this->getVertexPtr(vertexTag);

        if (vertexPtr == 0)
        {
            int vertexRef = vertexPtrOther->getRef();
            // vertexPtr = new Vertex(vertexTag, vertexRef);
            Vertex newVertex(vertexTag, vertexRef);

            if (vertexPtr == 0)
            {
                cerr << "Graph::merge - out of memory\n";
                return -1;
            }

            // this->addVertex(vertexPtr, false);
            this->addVertex(&newVertex, false);
        }
    }


    // loop through other adding all the edges that exist in other
    VertexIter &otherVertices2 = other.getVertices();

    while ((vertexPtrOther = otherVertices2()) != 0)
    {
        int vertexTag = vertexPtrOther->getTag();
        const auto &adjacency = vertexPtrOther->getAdjacency();

        for (int i = 0; i < (int) adjacency.size(); i++)
        {
            if (this->addEdge(vertexTag, adjacency[i] ) < 0)
            {
                cerr << "Graph::merge - could not add an edge!\n";
                return -2;
            }
        }
    }

    return result;
}


void
Graph::Print(ostream &s, int flag)
{

    // VertexIter theVertices = this->getVertices();
    // Vertex *v;
    // s << "Graph\n";
    // s << "numVertices = " << numVertices << endl;
    // s << "numEdges    = " << numEdges << endl;
    // //    1234567890 1234567890 12345 12345 12345
    // s << "tag       |ref       | weight   | col | tmp | deg  | adjacency \n";
    // while ((v = theVertices()) != 0)
    // {
    //     int tag = v->getTag();
    //     int ref = v->getRef();
    //     double weight = v->getWeight();
    //     int color = v->getColor();
    //     int tmp = v->getTag();
    //     int degree = v->getDegree();
    //     const ID &adj = v->getAdjacency();
    //     s << setw(10) << tag << " "
    //       << setw(10) << ref << " "
    //       << setw(10) << weight << " "
    //       << setw(5) << color << " "
    //       << setw(5) << tmp << " "
    //       << setw(5) << degree << " | ";
    //     for (int i = 0; i < adj.Size(); i++)
    //     {
    //         s << adj(i) << " ";
    //     }
    //     s << endl;
    // }
    // myVertices->Print(s, flag);
    // s << "Graph::Print() -- not implemented.\n";
    // cout << "Graph::Print() -- not implemented.\n";
}


ostream &operator<<(ostream &s, Graph &M)
{
    M.Print(s);
    return s;
}


// int
// Graph::sendSelf(int commitTag, Channel &theChannel)
// {
//     // check not a datastore ..
//     if (theChannel.isDatastore() != 0)
//     {
//         cerr << "Graph::sendSelf() - does not at present send to a database\n";
//         return -1;
//     }

//     // send numEdges & the number of vertices

//     int numVertices = this->getNumVertex();

//     static ID idData(2);
//     idData(0) = numEdges;
//     idData(1) = numVertices;

//     if (theChannel.sendID(0, commitTag, idData) < 0)
//     {
//         cerr << "Graph::sendSelf() - failed to send the id\n";
//         return -3;
//     }

//     ID tags(numVertices + 1);  //Last one will be the size of the adjacency vector
//     ID refs(numVertices);
//     Vector weights(numVertices);
//     ID colors(numVertices);
//     ID tmps(numVertices);
//     // ID myTag(numEdges);
//     // ID myRef(numEdges);
//     // ID myWeight(numEdges);
//     // ID myColor(numEdges);
//     // ID myDegree(numEdges);
//     // ID myTmp(numEdges);
//     // ID vsize(numEdges);

//     // Send tags
//     VertexIter &theVertices = this->getVertices();
//     Vertex *vertexPtr;
//     int i = 0;
//     int adjsize = 0;
//     while ((vertexPtr = theVertices()) != 0)
//     {
//         tags[i] = vertexPtr->getTag();
//         refs[i] = vertexPtr->getRef();
//         weights[i] = vertexPtr->getWeight();
//         colors[i] = vertexPtr->getColor();
//         tmps[i] = vertexPtr->getTmp();
//         i++;
//         adjsize += 1;
//         adjsize += vertexPtr->getDegree();
//     }
//     tags[i] = adjsize;
//     // cout << "sending tags to " << theChannel.getTag() << endl;
//     theChannel.sendID(0, commitTag, tags);
//     // cout << "sending refs to " << theChannel.getTag() << endl;
//     theChannel.sendID(0, commitTag, refs);
//     // cout << "sending weights to " << theChannel.getTag() << endl;
//     theChannel.sendVector(0, commitTag, weights);
//     // cout << "sending colors to " << theChannel.getTag() << endl;
//     theChannel.sendID(0, commitTag, colors);
//     // cout << "sending tmps to " << theChannel.getTag() << endl;
//     theChannel.sendID(0, commitTag, tmps);

//     ID adjacency_vector(adjsize);
//     theVertices.reset();
//     int pos = 0;
//     while ((vertexPtr = theVertices()) != 0)
//     {
//         ID myadj = vertexPtr->getAdjacency();
//         adjacency_vector[pos++] = myadj.Size();
//         for (int i = 0; i < myadj.Size(); i++)
//         {
//             adjacency_vector[pos++] = myadj[i];
//         }
//     }

//     theChannel.sendID(0, commitTag, adjacency_vector);

//     // send each vertex
//     // VertexIter &theVertices = this->getVertices();
//     // Vertex *vertexPtr;
//     // while ((vertexPtr = theVertices()) != 0)
//     // {
//     //     if (vertexPtr->sendSelf(commitTag, theChannel) < 0)
//     //     {
//     //         cerr << "Graph::sendSelf() - failed to send a vertex ";
//     //         //       cerr << "Graph::sendSelf() - failed to send a vertex: " << *vertexPtr;
//     //         return -3;
//     //     }
//     // }

//     return 0;
// }


// int
// Graph::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
// {
//     // check not from a datastore
//     if (theChannel.isDatastore() != 0)
//     {
//         cerr << "Graph::receiveSelf() - at present does not receive from a database\n";
//         return -1;
//     }

//     // check blank
//     if (this->getNumVertex() != 0)
//     {
//         cerr << "Graph::receiveSelf() - can only receive to an empty graph at present\n";

//         numEdges = 0;
//         // myVertices->clearAll();
//     }

//     // recv numEdges & numVertices
//     static ID idData(2);

//     if (theChannel.receiveID(0, commitTag, idData) < 0)
//     {
//         cerr << "Graph::receiveSelf() - failed to receive the id\n";
//         return -3;
//     }

//     numEdges = idData(0);
//     int numVertices = idData(1);

//     // cout << "Preparing for numVertices = " << numVertices << endl;

//     ID tags(numVertices + 1); //Last is the size of the adjacency vector
//     ID refs(numVertices);
//     Vector weights(numVertices);
//     ID colors(numVertices);
//     ID tmps(numVertices);


//     // cout << "receiving tags from " << theChannel.getTag() << endl;
//     theChannel.receiveID(0, commitTag, tags);

//     // cout << "receiving refs from " << theChannel.getTag() << endl;
//     theChannel.receiveID(0, commitTag, refs);

//     // cout << "receiving weights from " << theChannel.getTag() << endl;
//     theChannel.receiveVector(0, commitTag, weights);

//     // cout << "receiving colors from " << theChannel.getTag() << endl;
//     theChannel.receiveID(0, commitTag, colors);

//     // cout << "receiving tmps from " << theChannel.getTag() << endl;
//     theChannel.receiveID(0, commitTag, tmps);

//     //Allocate space for vertices....
//     int maxtag = 0;
//     for (int i = 0; i < numVertices; i++)
//     {
//         if (tags[i] > maxtag)
//         {
//             maxtag = tags[i];
//         }
//     }

//     myVertices.resize(maxtag + 1);


//     // for each vertex to be received, create it, receive it and then add it to the graph
//     for (int i = 0; i < numVertices; i++)
//     {
//         int tag = tags[i];
//         int ref = refs[i];
//         int weight = weights[i];
//         int color = colors[i];
//         int tmp = tmps[i];
//         // Vertex *theVertex = new Vertex( tag,  ref,  weight ,  color );
//         Vertex theVertex( tag,  ref,  weight ,  color );
//         // theVertex->setTmp(tmp);
//         theVertex.setTmp(tmp);

//         // if (theVertex == 0)
//         // {
//         //     cerr << "Graph::receiveSelf() - out of memory\n";
//         //     return -4;
//         // }

//         // if (theVertex->receiveSelf(commitTag, theChannel, theBroker) < 0)
//         // {
//         //     cerr << "Graph::receiveSelf() - vertex failed to receive itself\n";
//         //     return -5;
//         // }

//         this->addVertex(&theVertex, false);
//     }

//     int adjsize = tags[numVertices];

//     ID adjacency_vector(adjsize);

//     theChannel.receiveID(0, commitTag, adjacency_vector);

//     VertexIter &theVertices = this->getVertices();
//     Vertex *vertexPtr;
//     int pos = 0;
//     while ((vertexPtr = theVertices()) != 0)
//     {
//         int size = adjacency_vector[pos++];
//         for (int i = 0; i < size; i++)
//         {
//             vertexPtr->addEdge(adjacency_vector[pos++]);
//         }
//     }


//     // // for each vertex to be received, create it, receive it and then add it to the graph
//     // for (int i = 0; i < numVertices; i++)
//     // {
//     //     Vertex *theVertex = new Vertex(0, 0);

//     //     if (theVertex == 0)
//     //     {
//     //         cerr << "Graph::receiveSelf() - out of memory\n";
//     //         return -4;
//     //     }

//     //     if (theVertex->receiveSelf(commitTag, theChannel, theBroker) < 0)
//     //     {
//     //         cerr << "Graph::receiveSelf() - vertex failed to receive itself\n";
//     //         return -5;
//     //     }

//     //     this->addVertex(theVertex, false);
//     // }

//     return 0;
// }
