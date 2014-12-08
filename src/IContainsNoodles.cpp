#include "all.h"

IContainsNoodles::~IContainsNoodles(void)
{
	for_each(m_noodles.cbegin(), m_noodles.cend(),
		[](const NoodleBase *n) {
			delete n;
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
