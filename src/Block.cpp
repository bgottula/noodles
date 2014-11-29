#include "all.h"

Block::~Block(void)
{
	if (m_name != nullptr)
	{
		free(m_name);
	}
}

const char *Block::name(void) const
{
	/* only do this the first time the function is called for this instance */
	if (m_name == nullptr)
	{
		int status;
		m_name = abi::__cxa_demangle(typeid(*this).name(),
			nullptr, nullptr, &status);
		assert(status == 0);
	}
	
	return m_name;
}
