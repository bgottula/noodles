#include "std.h"
#include "debug.h"
#include "Noodles.h"

using namespace std;
using namespace boost;

void Noodles::addConnection(Connection *c)
{
    debug("addConnection: [ %p(%d) -> %p(%d) ]\n",
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
            debug("- source block %p is already in the graph.\n",
                c->m_sourceBlock);
            
            vert_source = v;
            found_source = true;
        }
        if (m_graph[v] == c->m_sinkBlock)
        {
            debug("- sink block %p is already in the graph.\n",
                c->m_sinkBlock);
            
            vert_sink = v;
            found_sink = true;
        }
        
        if (found_source && found_sink) break;
    }
    
    /* if source and/or sink were not found, add new vertex(es) to the graph */
    if (!found_source)
    {
        debug("- source block %p is not in the graph; adding now.\n",
            c->m_sourceBlock);
        
        vert_source = add_vertex(m_graph);
        m_graph[vert_source] = c->m_sourceBlock;
    }
    if (!found_sink)
    {
        debug("- sink block %p is not in the graph; adding now.\n",
            c->m_sinkBlock);
        
        vert_sink = add_vertex(m_graph);
        m_graph[vert_sink] = c->m_sinkBlock;
    }
    
    /* see if the edge is already in the graph; also, prevent outputs from
     * having multiple inputs connected to them */
    auto e_it = edges(m_graph);
    for (auto it = e_it.first; it != e_it.second; ++it)
    {
        edge_t e = *it;
        pair<int, int> indices = m_graph[e];
        
        vertex_t v1 = source(e, m_graph);
        vertex_t v2 = target(e, m_graph);
        
        bool source_match = (m_graph[v1] == c->m_sourceBlock &&
            indices.first == c->m_sourceIndex);
        bool sink_match = (m_graph[v2] == c->m_sinkBlock &&
            indices.second == c->m_sinkIndex);
        
        if (source_match && sink_match)
        {
            debug("- noodle [ %p(%d) -> %p(%d) ] is already in the graph!\n",
                c->m_sourceBlock, c->m_sourceIndex,
                c->m_sinkBlock, c->m_sinkIndex);
            throw runtime_error("Exact duplicate noodles are not allowed");
        }
        else if (sink_match)
        {      
            debug("- noodles [ %p(%d) -> %p(%d) ] and [ %p(%d) -> %p(%d) ]\n"
                "  specify the exact same sink block and input index!\n",
                c->m_sourceBlock, c->m_sourceIndex,
                c->m_sinkBlock, c->m_sinkIndex,
                m_graph[v1], indices.first, m_graph[v2], indices.second);
            throw runtime_error("Inputs can only be connected to one output");
        }
    }
    
    debug("- noodle is not in the graph; adding now.\n");
    
    edge_t edge;
    bool result;
    
    tie(edge, result) = add_edge(vert_source, vert_sink, m_graph);
    assert(result);
    m_graph[edge] = pair<int, int>(c->m_sourceIndex, c->m_sinkIndex);
    
    dumpGraph();
    
    m_needCheck = true;
}

int Noodles::checkGraph(void)
{
    debug("checkGraph: checking graph validity\n");
    
    int errors = 0;
    
    auto e_it = edges(m_graph);
    if (e_it.first == e_it.second)
    {
        debug("- error: no noodles in graph!");
        ++errors;
    }
    
    // TODO: check for nodes with no edges (error) [is this possible?]
    // TODO: warn about nodes with unconnected inputs or outputs
    
    if (errors == 0)
    {
        m_needCheck = false;
    }
    
    debug("- %d errors\n", errors);
    return errors;
}

void Noodles::dumpGraph(void)
{
    /* don't waste time accessing stuff that we won't print */
    if (!verbose) return;
    
    debug("\n======================= graph summary =======================\n");
    
    debug("%lu blocks\n", num_vertices(m_graph));
    auto v_it = vertices(m_graph);
    for (auto it = v_it.first; it != v_it.second; ++it)
    {
        vertex_t v = *it;
        debug(" + block: %p\n", m_graph[v]);
    }
    
    debug("\n%lu noodles\n", num_edges(m_graph));
    auto e_it = edges(m_graph);
    for (auto it = e_it.first; it != e_it.second; ++it)
    {
        edge_t e = *it;
        std::pair<int, int> indices = m_graph[e];
        
        vertex_t v1 = source(e, m_graph);
        vertex_t v2 = target(e, m_graph);
        
        debug(" + noodle: [ %p(%d) -> %p(%d) ]\n",
            m_graph[v1], indices.first, m_graph[v2], indices.second);
    }
    
    debug("=============================================================\n\n");
}

void Noodles::run(void)
{
    if (m_needCheck)
    {
        if (checkGraph() != 0)
        {
            throw runtime_error("Graph validation failed");
        }
    }
    
    /* Call work on all blocks */
    auto v_it = vertices(m_graph);
    for (auto it = v_it.first; it != v_it.second; ++it)
    {
        vertex_t v = *it;
        debug("run: calling work on %p\n", m_graph[v]);
        m_graph[v]->work();
    }


    /* Pass samples between blocks. As currently written, no attempt is made 
     * to handle blocks with multiple inputs or multiple outputs. */
    v_it = vertices(m_graph);
    for (auto it = v_it.first; it != v_it.second; ++it)
    {
        vertex_t v = *it;
        debug("run: on block %p\n", m_graph[v]);
        
        /* iterator over all outbound edges */
        auto e_it = out_edges(v, m_graph);

        /* This means there are no outbound edges */
        if (e_it.first == e_it.second)
        {
            continue;
        }

        while (!m_graph[v]->outputEmpty())
        {
            int sample = m_graph[v]->popOutput();
            
            /* Iterate over all outbound edges. */
            for (auto ei = e_it.first; ei != e_it.second; ++ei)
            {
                vertex_t v_sink = target(*ei, m_graph);
                auto conn = m_graph[*ei];
                debug("-- pushing sample %d to block %p input %d"
                    " (but actually input 0 for now)\n", 
                    sample, m_graph[v_sink], conn.second);
                m_graph[v_sink]->pushInput(sample/*, conn.second*/);
            }
        }   
           
    }
}
