#include "all.h"

void Graph::addNoodle(Noodle *n, Endpoint from, Endpoint to)
{
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
	from.block->outputs.connect(from.port, n);
	to.block->inputs.connect(to.port, n);
	
	debug("- adding noodle to the graph.\n");
	
	bool result = m_noodles.insert(n).second;
	assert(result);
	
	dumpGraph();
	
	m_needCheck = true;
}

void Graph::addQNoodle(size_t queue_max, Endpoint from, Endpoint to)
{
	debug("addQNoodle: %s[%s](%p) >>[q:%zu]>> %s[%s](%p)\n",
		from.block->name(), from.port, from.block, queue_max,
		to.block->name(), to.port, to.block);
	
	QNoodle *n = new QNoodle(queue_max, from, to);
	addNoodle(n, from, to);
}

void Graph::addRNoodle(int init, Endpoint from, Endpoint to)
{
	debug("addRNoodle: %s[%s](%p) >>[r:%d]>> %s[%s](%p)\n",
		from.block->name(), from.port, from.block, init,
		to.block->name(), to.port, to.block);
	
	RNoodle *n = new RNoodle(init, from, to);
	addNoodle(n, from, to);
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
		"=================================");
	
	debug("\n\n# %lu blocks\n", m_blocks.size());
	for (auto it = m_blocks.cbegin(); it != m_blocks.cend(); ++it)
	{
		Block *b = *it;
		debug("\n  + block %s(%p)\n", b->name(), b);
		
		auto in_begin = b->inputs.m_names.cbegin();
		auto in_end = b->inputs.m_names.cend();
		for (auto in_it = in_begin; in_it != in_end; ++in_it)
		{
			pair<const char *, int> name = *in_it;
			unsigned long num_noodles =
				b->inputs.m_ports.at(b->inputs.m_names.at(name.first)).size();
			
			debug("    >> input \"%s\" (%lu noodle%s)\n",
				name.first, num_noodles, (num_noodles == 1 ? "" : "s"));
		}
		
		auto out_begin = b->outputs.m_names.cbegin();
		auto out_end = b->outputs.m_names.cend();
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
	for (auto it = m_noodles.cbegin(); it != m_noodles.cend(); ++it)
	{
		Noodle *n = *it;
		
		QNoodle *q = dynamic_cast<QNoodle *>(n);
		RNoodle *r = dynamic_cast<RNoodle *>(n);
		
		// TODO: see if nullptr works
		assert(q != NULL || r != NULL);
		if (q != NULL)
		{
			debug("\n  + noodle(%p)\n      %s[%s](%p) >>[q:%zu]>> %s[%s](%p)\n",
				q, q->m_from.block->name(), q->m_from.port, q->m_from.block,
				q->m_max, q->m_to.block->name(), q->m_to.port, q->m_to.block);
		}
		else if (r != NULL)
		{
			debug("\n  + noodle(%p)\n      %s[%s](%p) >>[r:%d]>> %s[%s](%p)\n",
				r, r->m_from.block->name(), r->m_from.port, r->m_from.block,
				r->m_reg, r->m_to.block->name(), r->m_to.port, r->m_to.block);
		}
	}
	
	debug("\n=================================================================="
		"==============\n\n");
}

void Graph::dumpNoodles(void)
{
	/* don't waste time accessing stuff that we won't print */
	if (!verbose) return;
	
	debug("=================================== noodles "
		"====================================");
	
	for (auto it = m_noodles.cbegin(); it != m_noodles.cend(); ++it)
	{
		Noodle *n = *it;
		
		QNoodle *q = dynamic_cast<QNoodle *>(n);
		RNoodle *r = dynamic_cast<RNoodle *>(n);
		
		// TODO: see if nullptr works
		assert(q != NULL || r != NULL);
		if (q != NULL)
		{
			debug("\nnoodle(%p)\n  %s[%s](%p) >>[q:%zu]>> %s[%s](%p)\n    {",
				q, q->m_from.block->name(), q->m_from.port, q->m_from.block,
				q->m_max, q->m_to.block->name(), q->m_to.port, q->m_to.block);
			deque<int>& qq = q->m_queue;
			for (auto q_it = qq.cbegin(); q_it != qq.cend(); ++q_it)
			{
				debug(" %d", *q_it);
			}
			debug(" }");
		}
		else if (r != NULL)
		{
			debug("\nnoodle(%p)\n  %s[%s](%p) >>[r:%d]>> %s[%s](%p)",
				r, r->m_from.block->name(), r->m_from.port, r->m_from.block,
				r->m_reg, r->m_to.block->name(), r->m_to.port, r->m_to.block);
		}
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
		
		dumpNoodles();
	}
}
