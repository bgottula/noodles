#include "all.h"

void NoodleBase::check() const
{
	debug(AT_BLD "NoodleBase::check" AT_RST "\n");
	
	// TODO
	
	IHasOwner::check();
}

void NoodleBase::dump(int level) const
{
	debug("%*sNoodle " AT_BLD "???" AT_RST "\n",
		(level * 2), "");
	
	// TODO
}
