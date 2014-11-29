#include "all.h"

void RoundRobinScheduler::run(void)
{
	m_graph.checkGraph();
	
	for (int i = 0; i < 10; ++i)
	{
		run_once();
	}
}

void RoundRobinScheduler::run_once(void)
{
	/* call work on all blocks */
	auto m_blocks = m_graph.m_blocks;
	for (auto it = m_blocks.begin(); it != m_blocks.end(); ++it)
	{
		Block *b = *it;
		debug("RoundRobinScheduler: calling work on block %s(%p)\n",
			b->name(), b);
		b->work();
		
		m_graph.dumpGraph();
	}
}

void ThreadedScheduler::run(void)
{
	
}

void InteractiveScheduler::run(void)
{
	
}
