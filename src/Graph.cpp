#include "all.h"

Graph::~Graph(void)
{
	debug(AT_BLD "Graph::dtor" AT_RST "\n");
	
	for_each(m_noodles.cbegin(), m_noodles.cend(),
		[](const NoodleBase *n) {
			delete n;
		});
}

void Graph::check(void)
{
	dump();
	
	if (m_blocks.empty()) throw GraphNoBlocksException();
	if (m_noodles.empty()) throw GraphNoNoodlesException();
	
	for_each(m_blocks.cbegin(), m_blocks.cend(),
		[](const NamedBlock& nb) {
			nb.block->check();
		});
	
#warning TODO
	// TODO: also check all noodles (write Noodle::check or whatever)
	
	m_state = GraphState::RUN;
}

void Graph::dump(bool blocks, bool noodles) const
{
#warning FINISHME
}

const char *Graph::name(void) const
{
	/* only do this the first time the function is called for this instance */
	if (m_name == nullptr)
	{
		decltype(*this) g = *this;
		m_name = demangle(typeid(g).name());
	}
	
	return m_name;
}

void Graph::register_block(const char *b_name, Block *b)
{
	if (m_state != GraphState::SETUP) throw GraphModifiedAfterSetupException();
	
	debug(AT_BLD "%s.register_block:" AT_RST " %s %s @ %p\n",
		name(), demangle(typeid(*b).name()), b_name, b);
	
	if (any_of(m_blocks.cbegin(), m_blocks.cend(),
		[&](const NamedBlock& nb) {
			return (nb.block == b);
		}))
	{
		throw GraphDuplicateBlockException();
	}
	if (any_of(m_blocks.cbegin(), m_blocks.cend(),
		[&](const NamedBlock& nb) {
			return (strcmp(b_name, nb.name) == 0);
		}))
	{
		throw GraphDuplicateBlockNameException();
	}
	
	NamedBlock nb = { .name = b_name, .block = b };
	m_blocks.push_back(nb);
}

void Graph::add_noodle(NoodleBase *n)
{
	if (m_state != GraphState::SETUP) throw GraphModifiedAfterSetupException();
	
	if (find(m_noodles.cbegin(), m_noodles.cend(), n) != m_noodles.cend())
	{
		throw GraphDuplicateNoodleException();
	}
	
	m_noodles.push_back(n);
}

#if 0
void Graph::addNoodle(Noodle *n, Endpoint from, Endpoint to)
{
	if (find(m_blocks.cbegin(), m_blocks.cend(), from.block) == m_blocks.cend())
	{
		debug("- block %s(%p) is not in the graph; adding now.\n",
			from.block->name(), from.block);
		
		m_blocks.push_back(from.block);
	}
	else
	{
		debug("- block %s(%p) is already in the graph.\n",
			from.block->name(), from.block);
	}
	
	if (find(m_blocks.cbegin(), m_blocks.cend(), to.block) == m_blocks.cend())
	{
		debug("- block %s(%p) is not in the graph; adding now.\n",
			to.block->name(), to.block);
		
		m_blocks.push_back(to.block);
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
	
	m_noodles.push_back(n);
	
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

void Graph::dumpGraph(bool blocks, bool noodles)
{
	static int count = 0;
	
	/* don't waste time accessing stuff that we won't print */
	if (!verbose) return;
	
	debug(AT_BLD AT_ULI "\nGRAPH SNAPSHOT #%d\n" AT_RST, ++count);
	
	if (blocks) dumpBlocks();
	if (noodles) dumpNoodles();
}

void Graph::dumpBlocks(void)
{
	for (auto it = m_blocks.cbegin(); it != m_blocks.cend(); ++it)
	{
		Block *b = *it;
		debug("  BLOCK %s", str_block(b));
		
		auto in_begin = b->inputs.m_names.cbegin();
		auto in_end = b->inputs.m_names.cend();
		for (auto in_it = in_begin; in_it != in_end; ++in_it)
		{
			pair<const char *, int> name = *in_it;
			auto noodles =
				b->inputs.m_ports.at(b->inputs.m_names.at(name.first));
			
			debug("\n     in " AT_ULI FG_GRN "%s" AT_RST FG_DEF, name.first);
			if (!noodles.empty())
			{
				debug(" (from %s)",
					str_endpoint(&noodles.at(0)->m_from, false));
			}
			else
			{
				debug(" (unconnected!)");
			}
		}
		
		auto out_begin = b->outputs.m_names.cbegin();
		auto out_end = b->outputs.m_names.cend();
		for (auto out_it = out_begin; out_it != out_end; ++out_it)
		{
			pair<const char *, int> name = *out_it;
			auto noodles =
				b->outputs.m_ports.at(b->outputs.m_names.at(name.first));
			
#warning TODO
			// TODO: make this match the input noodles side
			// (show a list of the from endpoints)
			
			debug("\n    out " AT_ULI FG_RED "%s" AT_RST FG_DEF, name.first);
			if (!noodles.empty())
			{
				unsigned long num_noodles = noodles.size();
				debug(" (to %lu noodle%s)",
					num_noodles, (num_noodles == 1 ? "" : "s"));
			}
			else
			{
				debug(" (unconnected!)");
			}
		}
		
		debug("\n\n");
	}
	
	/* clean up dynamically allocated strings */
	m_strpool.clear();
}

void Graph::dumpNoodles(void)
{
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
			debug("\n    reg [ " AT_BLD "%d" AT_RST " ]", r->m_reg.load());
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
#endif
