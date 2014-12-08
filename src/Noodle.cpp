#include "all.h"

void NoodleBase::set_owner(IContainsNoodles *owner)
{
	if (m_owner != nullptr) throw NoodleAlreadyOwnedException();
	
	m_owner = owner;
}

IContainsNoodles *NoodleBase::get_owner(void)
{
	if (m_owner == nullptr) throw NoodleNotOwnedException();
	
	return m_owner;
}
