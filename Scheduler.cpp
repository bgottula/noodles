#include "Scheduler.h"
#include <cstdio>

using namespace std;
using namespace boost;

void Scheduler::addConnection(Block *sourceBlock, Block *sinkBlock, int sourceIndex, int sinkIndex)
{
    printf("addConnection: [ source %p idx %d ] -> [ sink %p idx %d]\n",
        sourceBlock, sourceIndex, sinkBlock, sinkIndex);
    
    bool found_source = false, found_sink = false;
    vertex_t vert_source;
    vertex_t vert_sink;
    
    auto v_it = vertices(m_graph);
    for (auto it = v_it.first; it != v_it.second; ++it)
    {
        vertex_t v = *it;
        
        if (m_graph[v] == sourceBlock)
        {
            printf("source block %p is already in the graph.\n", sourceBlock);
            vert_source = v;
            found_source = true;
        }
        if (m_graph[v] == sinkBlock)
        {
            printf("sink block %p is already in the graph.\n", sinkBlock);
            vert_sink = v;
            found_sink = true;
        }
        
        if (found_source && found_sink)
        {
            break;
        }
    }
    
    if (!found_source)
    {
        printf("source block %p is not in the graph; adding now.\n",
            sourceBlock);
        vert_source = add_vertex(m_graph);
        m_graph[vert_source] = sourceBlock;
    }
    if (!found_sink)
    {
        printf("sink block %p is not in the graph; adding now.\n",
            sinkBlock);
        vert_sink = add_vertex(m_graph);
        m_graph[vert_sink] = sinkBlock;
    }
    
    bool found_edge = false;
    edge_t edge; bool b;
    
    auto e_it = edges(m_graph);
    for (auto it = e_it.first; it != e_it.second; ++it)
    {
        edge_t e = *it;
        auto conn = m_graph[e];
        
        vertex_t v1 = source(e, m_graph);
        vertex_t v2 = target(e, m_graph);
        
        if (m_graph[v1] == sourceBlock && m_graph[v2] == sinkBlock &&
            conn.first == sourceIndex && conn.second == sinkIndex)
        {
            printf("edge is already in the graph!\n");
            edge = e;
            found_edge = true;
            break;
        }
    }
    
    if (!found_edge)
    {
        printf("edge does not exist in the graph; adding now.\n");
        tie(edge, b) = add_edge(vert_source, vert_sink, m_graph);
        m_graph[edge] = pair<int, int>(sourceIndex, sinkIndex);
    }
    else
    {
        printf("why on earth do you want to add the same edge twice?\n"
            "...not adding this edge. don't be dumb.\n");
    }
    
    printf("============ m_graph summary ============\n");
    v_it = vertices(m_graph);
    for (auto it = v_it.first; it != v_it.second; ++it)
    {
        vertex_t v = *it;
        printf("vert: %p\n", m_graph[v]);
    }
    e_it = edges(m_graph);
    for (auto it = e_it.first; it != e_it.second; ++it)
    {
        edge_t e = *it;
        auto conn = m_graph[e];
        
        vertex_t v1 = source(e, m_graph);
        vertex_t v2 = target(e, m_graph);
        
        printf("edge: [ %p %d ] -> [ %p %d ]\n",
            m_graph[v1], conn.first, m_graph[v2], conn.second);
    }
    printf("========================================\n\n");
}

void Scheduler::run(void)
{
    printf("m_graph has %d vertices\n", num_vertices(m_graph));
    printf("m_graph has %d edges\n", num_edges(m_graph));
    printf("we're done here.\n");
    exit(0);
    
#if 0
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
#endif
}
