#include "all.h"

const char *Block::name(void)
{
	int status;
	const char *name = abi::__cxa_demangle(typeid(*this).name(),
		nullptr, nullptr, &status);
	
	assert(status == 0);
	
	/* this pointer should be free'd by the caller to avoid leaking memory */
	return name;
}
