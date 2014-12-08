#include "all.h"

void Port::check() const
{
	debug(AT_BLD "Port::check" AT_RST "\n");
	
#warning FINISHME
}

void Port::set_owner(IContainsPorts *owner)
{
	if (m_owner != nullptr) throw PortAlreadyOwnedException();
	
	m_owner = owner;
}

IContainsPorts *Port::get_owner() const
{
	if (m_owner == nullptr) throw PortNotOwnedException();
	
	return m_owner;
}
