#include "all.h"

void Block::check() const
{
	debug(AT_BLD "%s::check" AT_RST "\n", name());
	
#warning FINISHME
}

void Block::set_owner(IContainsBlocks *owner)
{
	if (m_owner != nullptr) throw BlockAlreadyOwnedException();
	
	m_owner = owner;
}

IContainsBlocks *Block::get_owner()
{
	if (m_owner == nullptr) throw BlockNotOwnedException();
	
	return m_owner;
}
