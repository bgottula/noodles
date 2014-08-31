#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <boost/graph/adjacency_list.hpp>
#include "Block.h"

typedef boost::adjacency_list <boost::listS, boost::vecS, boost::directedS, Block *, std::pair<int, int> > NoodleGraph;
typedef boost::graph_traits <NoodleGraph>::vertex_descriptor vertex_t;
typedef boost::graph_traits <NoodleGraph>::edge_descriptor edge_t;

class Scheduler
{
    public:
        void addConnection(Block *sourceBlock, Block *sinkBlock, int sourceIndex, int sinkIndex);
        void run(void);

    private:
        NoodleGraph m_graph;
};

#endif
