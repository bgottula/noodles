#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <boost/graph/adjacency_list.hpp>
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

class Scheduler
{
    public:
        void addConnection(Connection *c);
        void run(void);

    private:
        boost::adjacency_list <> m_graph;
};

#endif
