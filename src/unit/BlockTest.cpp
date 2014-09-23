#include "all.h"
#include "boost_test.h"

class MockBlock : public Block
{
	void reset(void) {}
	void work(void) {}
};

BOOST_AUTO_TEST_CASE( blockTest )
{
	BOOST_CHECK(0);
}
