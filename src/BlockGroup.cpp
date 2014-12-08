#include "all.h"

void BlockGroup::check() const
{
	debug(AT_BLD "%s::check" AT_RST "\n", sub_name());
	
	// TODO
	
	IHasOwner::check();
	IHasName::check();
	IContainsNoodles::check();
	IContainsPorts::check();
	IContainsBlocks::check();
}

void BlockGroup::dump(int level) const
{
	debug("%*s%s " AT_BLD "%s" AT_RST "\n",
		(level * 2), "", sub_name(), get_name());
	
	IContainsPorts::dump(level + 1);
	IContainsBlocks::dump(level + 1);
	IContainsNoodles::dump(level + 1);
}
