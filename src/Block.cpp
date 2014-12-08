#include "all.h"

void Block::check() const
{
	debug(AT_BLD "%s::check" AT_RST "\n", name());
	
	// TODO
	
	IHasOwner::check();
	IContainsPorts::check();
}
