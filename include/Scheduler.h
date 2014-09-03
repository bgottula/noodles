#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Block.h"

class Connection
{
	public:
		Connection(Block *sourceBlock, Block *sinkBlock,
			int sourceIndex = 0, int sinkIndex = 0)
			: m_sourceBlock(sourceBlock),
			m_sinkBlock(sinkBlock),
			m_sourceIndex(sourceIndex),
			m_sinkIndex(sinkIndex)
		{}

		Block * const m_sourceBlock;
		Block * const m_sinkBlock;
		const int m_sourceIndex;
		const int m_sinkIndex;
};

/* NoodleGraph is an adjacency list where the vertices are of type Block *, and
 * the edges are of type std::pair <int,int>; use std::vector for storage */
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
	Block *, std::pair <int, int> > NoodleGraph;

/* vertex_t and edge_t are essentially reference types for vertices and edges.
 * use m_graph[...] with one of these references to access the data */
typedef boost::graph_traits <NoodleGraph>::vertex_descriptor vertex_t;
typedef boost::graph_traits <NoodleGraph>::edge_descriptor edge_t;

class Scheduler
{
    public:
        void addConnection(Connection *c);
        void checkGraph(void);
        void dumpGraph(void);
        void run(void);

    private:
    	bool m_needCheck = true;
        NoodleGraph m_graph;
};

#endif
