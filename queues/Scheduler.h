#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>
#include "Block.h"

class Scheduler
{
    public:
        void addBlock(Block *block);
        void run(void);

    private:
        std::list<Block *> m_blocks;
};

#endif
