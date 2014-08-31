#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <boost/graph/adjacency_list.hpp>
#include "Block.h"

/* NoodleGraph is an adjacency list where the vertices are of type Block *, and
 * the edges are of type std::pair <int,int> */
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
	Block *, std::pair <int, int> > NoodleGraph;

/* vertex_t and edge_t are essentially reference types for vertices and edges.
 * use m_graph[...] with one of these references to access the data */
typedef boost::graph_traits <NoodleGraph>::vertex_descriptor vertex_t;
typedef boost::graph_traits <NoodleGraph>::edge_descriptor edge_t;

class Scheduler
{
    public:
        void addConnection(Block *sourceBlock, Block *sinkBlock, int sourceIndex, int sinkIndex);
        void dumpGraph(void);
        void run(void);

    private:
        NoodleGraph m_graph;
};

#endif
