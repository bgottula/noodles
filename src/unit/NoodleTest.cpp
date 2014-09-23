#include "all.h"
#include "boost_test.h"

class MockBlock : public Block
{
	void reset(void) {}
	void work(void) {}
};

BOOST_AUTO_TEST_CASE( noodleTest )
{
#if 0
	MockBlock b1;
	MockBlock b2;
	
	Noodle n1(&b1, &b2);
	BOOST_CHECK_EQUAL(n1.m_sourceBlock, &b1);
	BOOST_CHECK_EQUAL(n1.m_sinkBlock, &b2);
	BOOST_CHECK_EQUAL(n1.m_sourceIndex, 0);
	BOOST_CHECK_EQUAL(n1.m_sinkIndex, 0);
	
	Noodle n2(&b1, &b2, 1, 2);
	BOOST_CHECK_EQUAL(n2.m_sourceBlock, &b1);
	BOOST_CHECK_EQUAL(n2.m_sinkBlock, &b2);
	BOOST_CHECK_EQUAL(n2.m_sourceIndex, 1);
	BOOST_CHECK_EQUAL(n2.m_sinkIndex, 2);
#endif
}
