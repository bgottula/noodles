#include "all.h"

void Graph::addNoodle(Endpoint from, Endpoint to)
{
	debug("addNoodle: %s[%s](@%p) -> %s[%s](@%p)\n",
		from.block->name(), from.port, from.block,
		to.block->name(), to.port, to.block);
	
	vertex_t vert_from, vert_to;
	bool found_from = false, found_to = false;
	
	/* see if the from or to vertex (or both) is already in the graph */
	auto v_it = vertices(m_graph);
	for (auto it = v_it.first; it != v_it.second; ++it)
	{
		vertex_t v = *it;
		
		if (m_graph[v] == from.block)
		{
			debug("- block %s(@%p) is already in the graph.\n",
				from.block->name(), from.block);
			
			vert_from = v;
			found_from = true;
		}
		if (m_graph[v] == to.block)
		{
			debug("- block %s(@%p) is already in the graph.\n",
				to.block->name(), to.block);
			
			vert_to = v;
			found_to = true;
		}
		
		if (found_from && found_to) break;
	}
	
	/* add new vertex(es) to the graph if necessary */
	if (!found_from)
	{
		debug("- block %s(@%p) is not in the graph; adding now.\n",
				from.block->name(), from.block);
		
		vert_from = add_vertex(m_graph);
		m_graph[vert_from] = from.block;
	}
	if (!found_to)
	{
		debug("- block %s(@%p) is not in the graph; adding now.\n",
				to.block->name(), to.block);
		
		vert_to = add_vertex(m_graph);
		m_graph[vert_to] = to.block;
	}
	
	/* see if this noodle is already in the graph; also, prevent outputs from
	 * having multiple inputs connected to them */
	auto e_it = edges(m_graph);
	for (auto it = e_it.first; it != e_it.second; ++it)
	{
		edge_t e = *it;
		Noodle *n = m_graph[e];
		
		vertex_t v1 = source(e, m_graph);
		vertex_t v2 = target(e, m_graph);
		
		Block *b1 = m_graph[v1];
		Block *b2 = m_graph[v2];
		
		bool match_from = (b1 == from.block && n->m_fromPort == from.port);
		bool match_to = (b2 == to.block && n->m_toPort == to.port);
		
		if (match_from && match_to)
		{
			debug("- noodle is already in the graph!\n"
				"  %s[%s] -> %s[%s]\n",
				from.block->name(), from.port,
				to.block->name(), to.port);
			throw DuplicateNoodleException();
		}
		else if (match_to)
		{
			debug("- noodles specify the exact same block and input index!\n"
				"  %s[%s] -> %s[%s]\n"
				"  %s[%s] -> %s[%s]\n",
				from.block->name(), from.port, to.block->name(), to.port,
				b1->name(), n->m_fromPort, b2->name(), n->m_toPort);
			throw InputMultipleNoodleException();
		}
	}
	
	debug("- noodle is not in the graph; adding now.\n");
	
	Noodle *noodle = new Noodle(from.port, to.port);
	
	edge_t edge;
	bool result;
	
	tie(edge, result) = add_edge(vert_from, vert_to, m_graph);
	assert(result);
	m_graph[edge] = noodle;
	
	from.block->outputs.connect(from.port, noodle);
	to.block->inputs.connect(to.port, noodle);
	
	dumpGraph();
	
	m_needCheck = true;
}

int Graph::checkGraph(void)
{
	debug("checkGraph: checking graph validity\n");
	
	int errors = 0;
	
	auto e_it = edges(m_graph);
	if (e_it.first == e_it.second)
	{
		throw EmptyGraphException();
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

void Graph::dumpGraph(void)
{
	/* don't waste time accessing stuff that we won't print */
	if (!verbose) return;
	
	debug("\n======================= graph summary =======================\n");
	
	debug("%lu blocks\n", num_vertices(m_graph));
	auto v_it = vertices(m_graph);
	for (auto it = v_it.first; it != v_it.second; ++it)
	{
		vertex_t v = *it;
		Block *b = m_graph[v];
		debug("+ block: %s(@%p)\n", b->name(), b);
	}
	
	debug("\n%lu noodles\n", num_edges(m_graph));
	auto e_it = edges(m_graph);
	for (auto it = e_it.first; it != e_it.second; ++it)
	{
		edge_t e = *it;
		Noodle *n = m_graph[e];
		
		vertex_t v1 = source(e, m_graph);
		vertex_t v2 = target(e, m_graph);
		
		Block *b1 = m_graph[v1];
		Block *b2 = m_graph[v2];
		
		debug("+ noodle: %s[%s](@%p) -> %s[%s](@%p)\n",
			b1->name(), n->m_fromPort, b1,
			b2->name(), n->m_toPort, b2);
	}
	
	debug("=============================================================\n\n");
}

void Graph::run(void)
{
	if (m_needCheck)
	{
		if (checkGraph() != 0)
		{
			throw GraphInvalidException();
		}
	}
	
	/* Call work on all blocks */
	auto v_it = vertices(m_graph);
	for (auto it = v_it.first; it != v_it.second; ++it)
	{
		vertex_t v = *it;
		Block *b = m_graph[v];
		debug("run: calling work on block %s(@%p)\n", b->name(), b);
		b->work();
	}
	
#if 0
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
#endif
}
