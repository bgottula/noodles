#include "all.h"

void Port::check() const
{
	debug(AT_BLD "Port::check" AT_RST "\n");
	
	// TODO
	
	IHasOwner::check();
}

void Port::dump(int level) const
{
	debug("%*sPort " AT_BLD "???" AT_RST "\n",
		(level * 2), "");
	
	// TODO
}
