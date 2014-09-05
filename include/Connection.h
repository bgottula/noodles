#ifndef CONNECTION_H
#define CONNECTION_H

#include "Block.h"

class Connection
{
	public:
		Connection(Block *sourceBlock, Block *sinkBlock,
			int sourceIndex = 0, int sinkIndex = 0)
			: m_sourceBlock(sourceBlock),
			m_sinkBlock(sinkBlock),
			m_sourceIndex(sourceIndex),
			m_sinkIndex(sinkIndex)
		{}

		Block * const m_sourceBlock;
		Block * const m_sinkBlock;
		const int m_sourceIndex;
		const int m_sinkIndex;
};

#endif
