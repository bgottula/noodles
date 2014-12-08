#include "all.h"

Block *IContainsBlocks::find_block(const char *b_name)
{
	auto it = find_if(m_blocks.cbegin(), m_blocks.cend(),
		[&](const NamedBlock& nb) {
			return (strcmp(b_name, nb.name) == 0);
		});
	
	if (it == m_blocks.cend()) throw NonexistentBlockException();
	
	return (*it).block;
}

void IContainsBlocks::register_block(const char *b_name, Block *b)
{
#warning Graph needs a way to tell IContainsBlocks to lock up after GraphState goes to RUN
	//if (m_state != GraphState::SETUP) throw GraphModifiedAfterSetupException();
	
	debug(AT_BLD "%s::register_block:" AT_RST " %s %s @ %p\n",
		demangle(typeid(*this).name()), demangle(typeid(*b).name()), b_name, b);
	
	if (any_of(m_blocks.cbegin(), m_blocks.cend(),
		[&](const NamedBlock& nb) {
			return (nb.block == b);
		}))
	{
		throw DuplicateBlockException();
	}
	if (any_of(m_blocks.cbegin(), m_blocks.cend(),
		[&](const NamedBlock& nb) {
			return (strcmp(b_name, nb.name) == 0);
		}))
	{
		throw DuplicateBlockNameException();
	}
	
	NamedBlock nb = { b_name, b };
	m_blocks.push_back(nb);
}
