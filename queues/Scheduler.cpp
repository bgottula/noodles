#include "Scheduler.h"

using namespace std;

void Scheduler::addBlock(Block *block)
{
    m_blocks.push_back(block);
}

void Scheduler::run(void)
{
    for (list<Block *>::iterator it = m_blocks.begin(), 
        prev = m_blocks.end(); it != m_blocks.end(); 
        prev = it, ++it)
    {
        Block *block = *it;
        /* First block should not need input */
        if (it != m_blocks.begin())
        {
            /* Move all outputs from the previous block to the input 
             * of the current block */
            Block *previousBlock = *prev;
            while (!previousBlock->outputEmpty())
            {
                block->pushInput(previousBlock->popOutput());
            }
        }
        block->work();
    }
}
