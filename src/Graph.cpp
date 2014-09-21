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
	
	debug("- attempting to connect noodle.\n");
	
	/* the connect functions will ensure that this is not a duplicate noodle and
	 * that inputs are not connected to multiple noodles */
	Noodle *noodle = new Noodle(from.port, to.port);
	from.block->outputs.connect(from.port, noodle);
	to.block->inputs.connect(to.port, noodle);
	
	debug("- adding noodle to the graph.\n");
	
	edge_t edge;
	bool result;
	
	tie(edge, result) = add_edge(vert_from, vert_to, m_graph);
	assert(result);
	m_graph[edge] = noodle;
	
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
}
