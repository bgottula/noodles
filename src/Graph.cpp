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

void Graph::checkGraph(void)
{
	if (!m_needCheck) return;
	
	debug("checkGraph: checking graph validity\n");
	
	if (m_noodles.empty())
	{
		throw EmptyGraphException();
	}
	
	// TODO: check for nodes with no edges (error) [is this possible?]
	
	m_needCheck = false;
}

void Graph::dumpGraph(void)
{
	static int count = 0;
	
	/* don't waste time accessing stuff that we won't print */
	if (!verbose) return;
	
	debug(AT_BLD AT_ULI "\nGRAPH SNAPSHOT #%d\n" AT_RST, ++count);
	
	for (auto it = m_blocks.cbegin(); it != m_blocks.cend(); ++it)
	{
		Block *b = *it;
		debug("  BLOCK " AT_BLD "%s" AT_RST, str_block(b));
		
		auto in_begin = b->inputs.m_names.cbegin();
		auto in_end = b->inputs.m_names.cend();
		for (auto in_it = in_begin; in_it != in_end; ++in_it)
		{
			pair<const char *, int> name = *in_it;
			unsigned long num_noodles =
				b->inputs.m_ports.at(b->inputs.m_names.at(name.first)).size();
			
			debug("\n     in " AT_ULI FG_GRN "%s" AT_RST FG_DEF, name.first);
			if (num_noodles > 0)
			{
				debug(" (" AT_BLD "%lu" AT_RST " noodle%s)",
					num_noodles, (num_noodles == 1 ? "" : "s"));
			}
			else
			{
				debug(" (" FG_RED "unconnected!" FG_DEF ")");
			}
		}
		
		auto out_begin = b->outputs.m_names.cbegin();
		auto out_end = b->outputs.m_names.cend();
		for (auto out_it = out_begin; out_it != out_end; ++out_it)
		{
			pair<const char *, int> name = *out_it;
			unsigned long num_noodles =
				b->outputs.m_ports.at(b->outputs.m_names.at(name.first)).size();
			
			debug("\n    out " AT_ULI FG_RED "%s" AT_RST FG_DEF, name.first);
			if (num_noodles > 0)
			{
				debug(" (" AT_BLD "%lu" AT_RST " noodle%s)",
					num_noodles, (num_noodles == 1 ? "" : "s"));
			}
			else
			{
				debug(" (" AT_BLD "unconnected!" AT_RST ")");
			}
		}
		
		debug("\n\n");
	}
	
	for (auto it = m_noodles.cbegin(); it != m_noodles.cend(); ++it)
	{
		Noodle *n = *it;
		
		debug(" NOODLE %s: %s >>> %s", str_noodle(n),
			str_endpoint(&n->m_from, false), str_endpoint(&n->m_to, true));
		
		if (n->is_qnoodle())
		{
			auto q = dynamic_cast<QNoodle *>(n);
			debug("\n   fill " AT_BLD "%zu" AT_RST "/" AT_BLD "%zu" AT_RST
				"\n  queue {" AT_BLD, q->count(), q->m_max);
			
			deque<int>& qq = q->m_queue;
			for (auto q_it = qq.cbegin(); q_it != qq.cend(); ++q_it)
			{
				debug(" %d", *q_it);
			}
			debug(AT_RST " }");
		}
		else
		{
			auto r = dynamic_cast<RNoodle *>(n);
			debug("\n    reg " AT_BLD "%d" AT_RST, r->m_reg.load());
		}
		
		debug("\n\n");
	}
	
	/* clean up dynamically allocated strings */
	m_strpool.clear();
}

const char *Graph::str_noodle(const Noodle *n)
{
	char *str = m_strpool.alloc(m_strlen);
	if (n->is_qnoodle())
	{
		snprintf(str, m_strlen,
			AT_BLD FG_YLW "QNoodle" AT_RST FG_DEF "@" AT_BLD "%04"
			PRIxPTR AT_RST, ((uintptr_t)n & 0xffff));
	}
	else
	{
		snprintf(str, m_strlen,
			AT_BLD FG_MGT "RNoodle" AT_RST FG_DEF "@" AT_BLD "%04"
			PRIxPTR AT_RST, ((uintptr_t)n & 0xffff));
	}
	return str;
}

const char *Graph::str_block(const Block *b)
{
	char *str = m_strpool.alloc(m_strlen);
	snprintf(str, m_strlen,
		AT_BLD FG_CYN "%s" AT_RST FG_DEF "@" AT_BLD "%04" PRIxPTR AT_RST,
		b->name(), ((uintptr_t)b & 0xffff));
	return str;
}

const char *Graph::str_endpoint(const Endpoint *e, bool input)
{
	char *str = m_strpool.alloc(m_strlen);
	snprintf(str, m_strlen,
		AT_BLD FG_CYN "%s" AT_RST FG_DEF "@" AT_BLD "%04" PRIxPTR AT_RST "["
		AT_ULI "%s%s" AT_RST FG_DEF "]",
		e->block->name(), ((uintptr_t)e->block & 0xffff),
		(input ? FG_GRN : FG_RED), e->port);
	return str;
}
