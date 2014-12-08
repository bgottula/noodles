#include "all.h"

void IContainsNoodles::check() const
{
	debug(AT_BLD "IContainsNoodles::check" AT_RST "\n");
	
	for_each(m_noodles.cbegin(), m_noodles.cend(),
		[](const NoodleBase *n) {
			n->check();
		});
}

void IContainsNoodles::dump(int level) const
{
	for_each(m_noodles.cbegin(), m_noodles.cend(),
		[=](const NoodleBase *n) {
			n->dump(level);
		});
}

void IContainsNoodles::add_noodle(NoodleBase *n)
{
#warning Graph needs a way to tell IContainsNoodles to lock up after GraphState goes to RUN
	//if (m_state != GraphState::SETUP) throw GraphModifiedAfterSetupException();
	
	if (find(m_noodles.cbegin(), m_noodles.cend(), n) != m_noodles.cend())
	{
		throw DuplicateNoodleException();
	}
	
	n->set_owner(this);
	
	m_noodles.push_back(n);
}
