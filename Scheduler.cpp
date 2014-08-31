#include "Scheduler.h"

using namespace std;

void Scheduler::addConnection(Connection *c)
{
    m_blocks.insert(c->m_sourceBlock);
    m_blocks.insert(c->m_sinkBlock);
    m_connections.insert(c);
}

void Scheduler::run(void)
{
    /* Call work on all blocks */
    for (set<Block *>::iterator it = m_blocks.begin(); it != m_blocks.end();
        ++it)
    {
        (*it)->work();
    }
    
    /* Move samples from source blocks to sink blocks */
    for (set<Connection *>::iterator it = m_connections.begin(); 
        it != m_connections.end(); ++it)
    {
        Connection *c = *it;
        
        /* Move all outputs from the source block to the input 
         * of the sink block */
        while (!c->m_sourceBlock->outputEmpty(c->m_sourceIndex))
        {
            c->m_sinkBlock->pushInput(
                c->m_sourceBlock->popOutput(c->m_sourceIndex), 
                c->m_sinkIndex);
        }        
    }
}
