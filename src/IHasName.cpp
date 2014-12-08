#include "all.h"

void IHasName::check() const
{
	debug(AT_BLD "IHasName::check" AT_RST "\n");
	
	if (m_name == nullptr) throw NotNamedException();
}

const char *IHasName::get_name() const
{
	if (m_name == nullptr) throw NotNamedException();
	
	return m_name;
}

void IHasName::set_name(const char *name)
{
	if (m_name != nullptr) throw AlreadyNamedException();
	
	m_name = name;
}
