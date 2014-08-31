#include "Scheduler.h"

using namespace std;
using namespace boost;

void Scheduler::addConnection(Connection *c)
{
    add_edge(c->m_sourceBlock, c->m_sinkBlock, m_graph);
}

void Scheduler::run(void)
{
    /* Call work on all blocks */
    for (boost::tie(i, end) = vertices(m_graph); i != end; ++i)
    {
        (*i)->work();
    }

    /* Move samples from source blocks to sink blocks */
    graph_traits < adjacency_list <> >::vertex_iterator i, end;
    for (boost::tie(i, end) = vertices(m_graph); i != end; ++i)
    {
        while (!(*i)->outputEmpty())
        {
            int sample = (*i)->popOutput();
            /* Iterate over all sinks attached to this block */
            graph_traits < adjacency_list <> >::adjacency_iterator ai, a_end;
            for (boost::tie(ai, a_end) = adjacent_vertices(*i, m_graph);
                ai != a_end; ++ai)
            {
                (*ai)->pushInput(sample);
            }
        }       
    }
}
