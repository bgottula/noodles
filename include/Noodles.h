#ifndef NOODLES_H
#define NOODLES_H

#include "Block.h"
#include "Noodle.h"

/* NoodleGraph is an adjacency list where the vertices are of type Block *, and
 * the edges are of type std::pair <int,int>; use std::vector for storage */
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
	Block *, std::pair <int, int> > NoodleGraph;

/* vertex_t and edge_t are essentially reference types for vertices and edges.
 * use m_graph[...] with one of these references to access the data */
typedef boost::graph_traits <NoodleGraph>::vertex_descriptor vertex_t;
typedef boost::graph_traits <NoodleGraph>::edge_descriptor edge_t;

class Noodles
{
    public:
        void addNoodle(Noodle *c);
        int checkGraph(void);
        void dumpGraph(void);
        void run(void);

    private:
    	bool m_needCheck = true;
        NoodleGraph m_graph;
};

#endif
