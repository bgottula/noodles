#include "all.h"

void BlockGroup::check() const
{
	debug(AT_BLD "%s::check" AT_RST "\n", name());
	
	// TODO
	
	IHasOwner::check();
	IContainsNoodles::check();
	IContainsPorts::check();
	IContainsBlocks::check();
}
