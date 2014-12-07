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
#warning FINISHME
#if 0
	/* call work on all blocks */
	auto m_blocks = m_graph->m_blocks;
	for (auto it = m_blocks->begin(); it != m_blocks->end(); ++it)
	{
		Block *b = *it;
		
		bool can_work = b->can_work();
		debug("RoundRobinScheduler: block %s(%p) can%s do work\n",
			(can_work ? "" : "not"), b->name(), b);
		
		if (can_work)
		{
			debug("RoundRobinScheduler: calling work on block %s(%p)\n",
				b->name(), b);
			b->work();
		}
		
		m_graph->dump(false, true);
	}
#endif
}

void ThreadedScheduler::run(void)
{
	
}

void InteractiveScheduler::run(void)
{
	
}
