#include "all.h"

void IContainsBlockGroups::check() const
{
	debug(AT_BLD "IContainsBlockGroups::check" AT_RST "\n");
	
	for_each(m_groups.cbegin(), m_groups.cend(),
		[](const Named<BlockGroup> ng) {
			ng.ptr->check();
		});
}

void IContainsBlockGroups::dump(int level) const
{
	for_each(m_groups.cbegin(), m_groups.cend(),
		[=](const Named<BlockGroup> ng) {
			ng.ptr->dump(level);
		});
}

BlockGroup *IContainsBlockGroups::find_group(const char *g_name)
{
	auto it = find_if(m_groups.cbegin(), m_groups.cend(),
		[&](const Named<BlockGroup>& ng) {
			return (strcmp(g_name, ng.name) == 0);
		});
	
	if (it == m_groups.cend()) throw NonexistentBlockGroupException();
	
	return (*it).ptr;
}

void IContainsBlockGroups::register_group(const char *g_name, BlockGroup *g)
{
#warning FINISHME
}
