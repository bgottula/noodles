#include "all.h"

void Graph::addNoodle(Endpoint from, Endpoint to)
{
	debug("addNoodle: %s[%s](%p) >>> %s[%s](%p)\n",
		from.block->name(), from.port, from.block,
		to.block->name(), to.port, to.block);
	
	if (m_blocks.find(from.block) == m_blocks.end())
	{
		debug("- block %s(%p) is not in the graph; adding now.\n",
			from.block->name(), from.block);
		
		bool result = m_blocks.insert(from.block).second;
		assert(result);
	}
	else
	{
		debug("- block %s(%p) is already in the graph.\n",
			from.block->name(), from.block);
	}
	
	if (m_blocks.find(to.block) == m_blocks.end())
	{
		debug("- block %s(%p) is not in the graph; adding now.\n",
			to.block->name(), to.block);
		
		bool result = m_blocks.insert(to.block).second;
		assert(result);
	}
	else
	{
		debug("- block %s(%p) is already in the graph.\n",
			to.block->name(), to.block);
	}
	
	debug("- attempting to connect noodle.\n");
	
	/* the connect functions will ensure that this is not a duplicate noodle and
	 * that inputs are not connected to multiple noodles */
	Noodle *noodle = new Noodle(from, to);
	from.block->outputs.connect(from.port, noodle);
	to.block->inputs.connect(to.port, noodle);
	
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
	
	debug("\n================================ graph summary "
		"=================================\n\n");
	
	debug("# %lu blocks\n", m_blocks.size());
	for (auto it = m_blocks.begin(); it != m_blocks.end(); ++it)
	{
		Block *b = *it;
		debug("\n  + block %s(%p)\n", b->name(), b);
		
		auto in_begin = b->inputs.m_names.begin();
		auto in_end = b->inputs.m_names.end();
		for (auto in_it = in_begin; in_it != in_end; ++in_it)
		{
			pair<const char *, int> name = *in_it;
			unsigned long num_noodles =
				b->inputs.m_ports.at(b->inputs.m_names.at(name.first)).size();
			
			debug("    >> input \"%s\" (%lu noodle%s)\n",
				name.first, num_noodles, (num_noodles == 1 ? "" : "s"));
		}
		
		auto out_begin = b->outputs.m_names.begin();
		auto out_end = b->outputs.m_names.end();
		for (auto out_it = out_begin; out_it != out_end; ++out_it)
		{
			pair<const char *, int> name = *out_it;
			unsigned long num_noodles =
				b->outputs.m_ports.at(b->outputs.m_names.at(name.first)).size();
			
			debug("    << output \"%s\" (%lu noodle%s)\n",
				name.first, num_noodles, (num_noodles == 1 ? "" : "s"));
		}
	}
	
	debug("\n\n# %lu noodles\n", m_noodles.size());
	for (auto it = m_noodles.begin(); it != m_noodles.end(); ++it)
	{
		Noodle *n = *it;
		
		debug("\n  + noodle(%p)\n"
			"      %s[%s](%p) >>> %s[%s](%p)\n", n,
			n->m_from.block->name(), n->m_from.port, n->m_from.block,
			n->m_to.block->name(), n->m_to.port, n->m_to.block);
	}
	
	debug("\n=================================================================="
		"==============\n\n");
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
		debug("run: calling work on block %s(%p)\n", b->name(), b);
		b->work();
	}
}
