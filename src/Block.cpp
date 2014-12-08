#include "all.h"

void Block::check() const
{
	debug(AT_BLD "%s::check" AT_RST "\n", sub_name());
	
	// TODO
	
	IHasOwner::check();
	IContainsPorts::check();
}

void Block::dump(int level) const
{
	debug("%*sBlock " AT_BLD "%s" AT_RST "\n",
		(level * 2), "", sub_name());
	
	IContainsPorts::dump(level + 1);
}
