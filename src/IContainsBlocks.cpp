#include "all.h"

void IContainsBlocks::check() const
{
	debug(AT_BLD "IContainsBlocks::check" AT_RST "\n");
	
#warning FINISHME
}

Block *IContainsBlocks::find_block(const char *b_name)
{
	auto it = find_if(m_blocks.cbegin(), m_blocks.cend(),
		[&](const Named<Block>& nb) {
			return (strcmp(b_name, nb.name) == 0);
		});
	
	if (it == m_blocks.cend()) throw NonexistentBlockException();
	
	return (*it).ptr;
}

void IContainsBlocks::register_block(const char *b_name, Block *b)
{
#warning Graph needs a way to tell IContainsBlocks to lock up after GraphState goes to RUN
	//if (m_state != GraphState::SETUP) throw GraphModifiedAfterSetupException();
	
	debug(AT_BLD "%s::register_block:" AT_RST " %s %s @ %p\n",
		demangle(typeid(*this).name()), demangle(typeid(*b).name()), b_name, b);
	
	if (any_of(m_blocks.cbegin(), m_blocks.cend(),
		[&](const Named<Block>& nb) {
			return (nb.ptr == b);
		}))
	{
		throw DuplicateBlockException();
	}
	if (any_of(m_blocks.cbegin(), m_blocks.cend(),
		[&](const Named<Block>& nb) {
			return (strcmp(b_name, nb.name) == 0);
		}))
	{
		throw DuplicateBlockNameException();
	}
	
	b->set_owner(this);
	
	Named<Block> nb = { b_name, b };
	m_blocks.push_back(nb);
}
