#include "std.h"
#include "boost_test.h"
#include "Block.h"
#include "Connection.h"

class MockBlock : public Block
{
	void work(void) {}
};

BOOST_AUTO_TEST_CASE( connectionTest )
{
	MockBlock b1;
	MockBlock b2;
	
	Connection c1(&b1, &b2);
	BOOST_CHECK_EQUAL(c1.m_sourceBlock, &b1);
	BOOST_CHECK_EQUAL(c1.m_sinkBlock, &b2);
	BOOST_CHECK_EQUAL(c1.m_sourceIndex, 0);
	BOOST_CHECK_EQUAL(c1.m_sinkIndex, 0);

	Connection c2(&b1, &b2, 1, 2);
	BOOST_CHECK_EQUAL(c2.m_sourceBlock, &b1);
	BOOST_CHECK_EQUAL(c2.m_sinkBlock, &b2);
	BOOST_CHECK_EQUAL(c2.m_sourceIndex, 1);
	BOOST_CHECK_EQUAL(c2.m_sinkIndex, 2);
}
