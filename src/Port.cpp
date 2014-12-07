#include "all.h"

void Port::set_owner(Block *b)
{
	if (m_owner != nullptr) throw PortAlreadyOwnedException();
	
	m_owner = b;
}

Block *Port::get_owner(void)
{
	if (m_owner == nullptr) throw PortNotOwnedException();
	
	return m_owner;
}
