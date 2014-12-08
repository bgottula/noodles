#include "all.h"

void NoodleBase::check(void) const
{
	debug(AT_BLD "NoodleBase::check" AT_RST "\n");
	
#warning FINISHME
}

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
