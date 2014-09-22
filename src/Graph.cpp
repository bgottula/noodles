#include "all.h"

void Graph::addNoodle(Endpoint from, Endpoint to)
{
	debug("addNoodle: %s[%s](@%p) -> %s[%s](@%p)\n",
		from.block->name(), from.port, from.block,
		to.block->name(), to.port, to.block);
	
	if (m_blocks.find(from.block) == m_blocks.end())
	{
		debug("- block %s(@%p) is not in the graph; adding now.\n",
			from.block->name(), from.block);
		
		bool result = m_blocks.insert(from.block).second;
		assert(result);
	}
	else
	{
		debug("- block %s(@%p) is already in the graph.\n",
			from.block->name(), from.block);
	}
	
	if (m_blocks.find(to.block) == m_blocks.end())
	{
		debug("- block %s(@%p) is not in the graph; adding now.\n",
			to.block->name(), to.block);
		
		bool result = m_blocks.insert(to.block).second;
		assert(result);
	}
	else
	{
		debug("- block %s(@%p) is already in the graph.\n",
			to.block->name(), to.block);
	}
	
	debug("- attempting to connect noodle.\n");
	
	/* the connect functions will ensure that this is not a duplicate noodle and
	 * that inputs are not connected to multiple noodles */
	Noodle *noodle = new Noodle(from, to);
	from.block->connect_output(from.port, noodle);
	to.block->connect_input(to.port, noodle);
	
	debug("- adding noodle to the graph.\n");
	
	bool result = m_noodles.insert(noodle).second;
	assert(result);
	
	dumpGraph();
	
	m_needCheck = true;
}

int Graph::checkGraph(void)
{
	debug("checkGraph: checking graph validity\n");
	
	int errors = 0;
	
	if (m_noodles.empty())
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
	
	debug("%lu blocks\n", m_blocks.size());
	for (auto it = m_blocks.begin(); it != m_blocks.end(); ++it)
	{
		Block *b = *it;
		debug("\n+ block: %s(@%p)\n", b->name(), b);
		
		auto names = b->debug_get_input_names();
		for (auto in_it = names.first; in_it != names.second; ++in_it)
		{
			pair<const char *, int> name = *in_it;
			debug("  >> input:  \"%s\"\n", name.first);
		}
		
		names = b->debug_get_output_names();
		for (auto out_it = names.first; out_it != names.second; ++out_it)
		{
			pair<const char *, int> name = *out_it;
			debug("  << output: \"%s\"\n", name.first);
		}
	}
	
	debug("\n%lu noodles\n", m_noodles.size());
	for (auto it = m_noodles.begin(); it != m_noodles.end(); ++it)
	{
		Noodle *n = *it;
		
		const Endpoint *from = n->debug_get_from_endpoint();
		const Endpoint *to = n->debug_get_to_endpoint();
		
		debug("+ noodle: %s[%s](@%p) -> %s[%s](@%p)\n",
			from->block->name(), from->port, from->block,
			to->block->name(), to->port, to->block);
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
	
	/* call work on all blocks */
	for (auto it = m_blocks.begin(); it != m_blocks.end(); ++it)
	{
		Block *b = *it;
		debug("run: calling work on block %s(@%p)\n", b->name(), b);
		b->work();
	}
}
