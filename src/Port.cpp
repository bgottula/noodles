#include "all.h"

void Port::set_owner(IContainsPorts *owner)
{
	if (m_owner != nullptr) throw PortAlreadyOwnedException();
	
	m_owner = owner;
}

IContainsPorts *Port::get_owner(void)
{
	if (m_owner == nullptr) throw PortNotOwnedException();
	
	return m_owner;
}
