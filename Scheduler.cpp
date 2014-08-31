#include "Scheduler.h"
#include <cstdio>

using namespace std;
using namespace boost;

void Scheduler::addConnection(Connection *c)
{
    printf("addConnection: source [ %p idx %d ] -> sink [ %p idx %d]\n",
        c->m_sourceBlock, c->m_sourceIndex, c->m_sinkBlock, c->m_sinkIndex);
    
    vertex_t vert_source, vert_sink;
    bool found_source = false, found_sink = false;
    
    /* see if the source or sink vertex (or both) is already in the graph */
    auto v_it = vertices(m_graph);
    for (auto it = v_it.first; it != v_it.second; ++it)
    {
        vertex_t v = *it;
        
        if (m_graph[v] == c->m_sourceBlock)
        {
            printf("- source block %p is already in the graph.\n",
                c->m_sourceBlock);
            
            vert_source = v;
            found_source = true;
        }
        if (m_graph[v] == c->m_sinkBlock)
        {
            printf("- sink block %p is already in the graph.\n",
                c->m_sinkBlock);
            
            vert_sink = v;
            found_sink = true;
        }
        
        if (found_source && found_sink) break;
    }
    
    /* if source and/or sink were not found, add new vertex(es) to the graph */
    if (!found_source)
    {
        printf("- source block %p is not in the graph; creating.\n",
            c->m_sourceBlock);
        
        vert_source = add_vertex(m_graph);
        m_graph[vert_source] = c->m_sourceBlock;
    }
    if (!found_sink)
    {
        printf("- sink block %p is not in the graph; creating.\n",
            c->m_sinkBlock);
        
        vert_sink = add_vertex(m_graph);
        m_graph[vert_sink] = c->m_sinkBlock;
    }
    
    edge_t edge;
    bool found_edge = false;
    
    /* see if the edge is already in the graph */
    auto e_it = edges(m_graph);
    for (auto it = e_it.first; it != e_it.second; ++it)
    {
        edge_t e = *it;
        auto conn = m_graph[e];
        
        vertex_t v1 = source(e, m_graph);
        vertex_t v2 = target(e, m_graph);
        
        if (m_graph[v1] == c->m_sourceBlock && m_graph[v2] == c->m_sinkBlock &&
            conn.first == c->m_sourceIndex && conn.second == c->m_sinkIndex)
        {
            printf("- edge is already in the graph!\n");
            
            edge = e;
            found_edge = true;
            
            break;
        }
    }
    
    /* if edge was not found, add new edge to the graph */
    if (!found_edge)
    {
        printf("- edge does not exist in the graph; creating.\n");
        
        bool b;
        tie(edge, b) = add_edge(vert_source, vert_sink, m_graph);
        m_graph[edge] = pair<int, int>(c->m_sourceIndex, c->m_sinkIndex);
        
        /* bool b is supposed to be set false if the graph is configured to
         * disallow parallel edges (see documentation for add_edge);
         * currently this is not how our graph is set up.
         * if this can be set, then we don't need to hunt for duplicate edges */
    }
    else
    {
        /* probably want to throw an exception here instead of this nonsense */
        printf("- refusing to add a duplicate edge\n");
    }
    
    dumpGraph();
}

void Scheduler::dumpGraph(void)
{
    printf("\n====================== m_graph summary ======================\n");
    
    printf("%d vertices\n", num_vertices(m_graph));
    auto v_it = vertices(m_graph);
    for (auto it = v_it.first; it != v_it.second; ++it)
    {
        vertex_t v = *it;
        printf(" + vert: %p\n", m_graph[v]);
    }
    
    printf("\n%d edges\n", num_edges(m_graph));
    auto e_it = edges(m_graph);
    for (auto it = e_it.first; it != e_it.second; ++it)
    {
        edge_t e = *it;
        auto conn = m_graph[e];
        
        vertex_t v1 = source(e, m_graph);
        vertex_t v2 = target(e, m_graph);
        
        printf(" + edge: [ %p idx %d ] -> [ %p idx %d ]\n",
            m_graph[v1], conn.first, m_graph[v2], conn.second);
    }
    
    printf("=============================================================\n\n");
}

void Scheduler::run(void)
{
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
