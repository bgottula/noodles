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
        pair<int, int> indices = m_graph[e];
        
        vertex_t v1 = source(e, m_graph);
        vertex_t v2 = target(e, m_graph);
        
        if (m_graph[v1] == c->m_sourceBlock && m_graph[v2] == c->m_sinkBlock &&
            indices.first == c->m_sourceIndex &&
            indices.second == c->m_sinkIndex)
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
    
    m_needCheck = true;
}

void Scheduler::checkGraph(void)
{
    printf("checkGraph: checking graph validity\n");
    
    int errors = 0;
    
    /* ensure that no input is connected to more than one output
     * (no two edges should have the same sink block AND sink index) */
    auto e_it = edges(m_graph);
    for (auto it1 = e_it.first; it1 != e_it.second; ++it1)
    {
        edge_t e1 = *it1;
        
        Block *sinkBlock1 = m_graph[target(e1, m_graph)];
        int sinkIndex1 = m_graph[e1].second;
        
        for (auto it2 = e_it.first; it2 != e_it.second; ++it2)
        {
            if (it1 == it2) continue;
            
            edge_t e2 = *it2;
            
            Block *sinkBlock2 = m_graph[target(e2, m_graph)];
            int sinkIndex2 = m_graph[e2].second;
            
            if (sinkBlock1 == sinkBlock2 && sinkIndex1 == sinkIndex2)
            {
                printf("- sink [ %p idx %d ] has more than one source "
                    "connected!\n", sinkBlock1, sinkIndex1);
                ++errors;
            }
        }
    }
    
    printf("- %d errors\n", errors);
    if (errors != 0)
    {
        throw runtime_error("Graph validation failed");
    }
    
    m_needCheck = false;
}

void Scheduler::dumpGraph(void)
{
    printf("\n====================== m_graph summary ======================\n");
    
    printf("%lu vertices\n", num_vertices(m_graph));
    auto v_it = vertices(m_graph);
    for (auto it = v_it.first; it != v_it.second; ++it)
    {
        vertex_t v = *it;
        printf(" + vert: %p\n", m_graph[v]);
    }
    
    printf("\n%lu edges\n", num_edges(m_graph));
    auto e_it = edges(m_graph);
    for (auto it = e_it.first; it != e_it.second; ++it)
    {
        edge_t e = *it;
        std::pair<int, int> indices = m_graph[e];
        
        vertex_t v1 = source(e, m_graph);
        vertex_t v2 = target(e, m_graph);
        
        printf(" + edge: [ %p idx %d ] -> [ %p idx %d ]\n",
            m_graph[v1], indices.first, m_graph[v2], indices.second);
    }
    
    printf("=============================================================\n\n");
}

void Scheduler::run(void)
{
    if (m_needCheck)
    {
        checkGraph();
    }
    
#if 0
    /* Call work on all blocks */
    for (boost::tie(i, end) = vertices(m_graph); i != end; ++i)
    {
        printf("run: calling work on %p\n", *i);
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
                printf("");
                (*ai)->pushInput(sample);
            }
        }       
    }
#endif
}
