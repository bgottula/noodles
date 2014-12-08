#include "all.h"

void RoundRobinScheduler::run(void)
{
	debug(AT_BLD "RoundRobinScheduler::run" AT_RST "\n");
	
	m_graph->check();
	
	// TODO: get rid of this idiom for scheduler finish state
	for (int i = 0; i < 10; ++i)
	{
		run_once();
	}
}

void RoundRobinScheduler::run_once(void)
{
	debug(AT_BLD "RoundRobinScheduler::run_once" AT_RST "\n");
	
	auto it_begin = m_graph->blocks_begin();
	auto it_end   = m_graph->blocks_end();
	
	/* call work on all blocks */
	for (auto it = it_begin; it != it_end; ++it)
	{
		NamedBlock nb = *it;
		Block *b = nb.block;
		
		bool can_work = b->can_work();
		debug(AT_BLD "RoundRobinScheduler::run_once:" AT_RST
			" block %s(%p) can%s do work\n",
			b->name(), b, (can_work ? "" : "not"));
		
		if (can_work)
		{
			debug(AT_BLD "RoundRobinScheduler::run_once:" AT_RST
			" calling work on block %s(%p)\n", b->name(), b);
			b->work();
		}
		
		m_graph->dump(false, true);
	}
}

void ThreadedScheduler::run(void)
{
	
}

void InteractiveScheduler::run(void)
{
	
}
